#include "chatui.h"
#include <memory>
#include <vector>
#include <utility>
#include <regex>
#include "../window_base.h"
#include "../scene.h"
#include "../bitmap.h"
#include "../output.h"
#include "../drawable_mgr.h"
#include "../font.h"
#include "../cache.h"
#include "../input.h"
#include "../utils.h"
#include "../player.h"
#include "../compiler.h"

class DrawableOnlineStatus : public Drawable {
	Rect bounds;

	//design parameters
	const unsigned int padding_horz = 8; // padding between box edges and content (left)
	const unsigned int padding_vert = 6; // padding between box edges and content (top)

	BitmapRef conn_status;
	BitmapRef room_status;
public:
	DrawableOnlineStatus(int x, int y, int w, int h)
		: Drawable(Priority::Priority_Maximum, Drawable::Flags::Global),
		bounds(x, y, w, h)
	{
		DrawableMgr::Register(this);

		//initialize
		SetConnectionStatus(false);
		SetRoomStatus(0);
	}

	void Draw(Bitmap& dst) {
		dst.Blit(bounds.x+padding_horz, bounds.y+padding_vert,
			*conn_status, conn_status->GetRect(), Opacity::Opaque());
		auto r_rect = room_status->GetRect();
		dst.Blit(bounds.x+bounds.width-padding_horz-r_rect.width, bounds.y+padding_vert,
			*room_status, r_rect, Opacity::Opaque());
	};

	void RefreshTheme() { }

	void SetConnectionStatus(bool status) {
		std::string conn_label = "";
		conn_label = status ? "Connected" : "Disconnected";

		auto c_rect = Text::GetSize(*Font::Default(), conn_label);
		conn_status = Bitmap::Create(c_rect.width+1, c_rect.height+1, true);
		Text::Draw(*conn_status, 0, 0, *Font::Default(), *Cache::SystemOrBlack(), 2, conn_label);
	}

	void SetRoomStatus(unsigned int room_id) {
		std::string room_label = "";
		room_label = "Room #"+std::to_string(room_id);
		auto r_rect = Text::GetSize(*Font::Default(), room_label);
		room_status = Bitmap::Create(r_rect.width+1, r_rect.height+1, true);
		Text::Draw(*room_status, 0, 0, *Font::Default(), *Cache::SystemOrBlack(), 2, room_label);
	}
};

enum VisibilityType {
	CV_LOCAL = 1,
	CV_GLOBAL = 2
};

struct ChatEntry {
	std::string color_a;
	std::string color_b;
	std::string color_c;
	VisibilityType visibility;
	ChatEntry(std::string a, std::string b, std::string c, VisibilityType v) {
		color_a = a;
		color_b = b;
		color_c = c;
		visibility = v;
	}
};

class DrawableChatLog : public Drawable {
	struct DrawableChatEntry {
		ChatEntry* message_data;
		BitmapRef render_graphic;
		bool dirty; // need to redraw? (for when UI skin changes)
		DrawableChatEntry(ChatEntry* msg) {
			message_data = msg;
			render_graphic = nullptr;
			dirty = true;
		}
	};

	Rect bounds;
	//design parameters
	const unsigned int message_margin = 3; // horizontal margin between panel edges and content
	const unsigned int scroll_frame = 4; // width of scroll bar's visual frame (on right side)
	const unsigned int scroll_bleed = 16; // how much to stretch right edge of scroll box offscreen (so only left frame shows)

	Window_Base scroll_box; // box used as rendered design for a scrollbar
	std::vector<DrawableChatEntry> messages;
	int scroll_position = 0;
	unsigned int scroll_content_height = 0; // total height of scrollable message log
	unsigned short visibility_flags = CV_LOCAL | CV_GLOBAL;
	BitmapRef current_theme; // system graphic for the current theme

	void BuildMessageGraphic(DrawableChatEntry& msg) {
		struct Glyph {
			Utils::TextRet data;
			Rect dims;
			unsigned short color;
		};
		using GlyphLine = std::vector<Glyph>;
		auto ExtractGlyphs = [](StringView str, unsigned short color, GlyphLine& line, unsigned int& width) {
			const auto* iter = str.data();
			const auto* end = str.data() + str.size();
			while(iter != end) {
				auto resp = Utils::TextNext(iter, end, 0);
				iter = resp.next;

				Rect ch_rect;
				if(resp.is_exfont)
					ch_rect = Text::GetSize(*Font::exfont, " ");
				else
					ch_rect = Font::Default()->GetSize(resp.ch);

				line.push_back({resp, ch_rect, color});
				width += ch_rect.width;
			}
		};
		auto FindFirstGlyphOf = [](GlyphLine& line, char32_t ch) -> int {
			unsigned int n_glyphs = line.size();
			for(int i = 0; i < n_glyphs; i++) {
				if(line[i].data.ch == ch) {
					return i;
				}
			}
			return -1;
		};
		auto FindLastGlyphOf = [](GlyphLine& line, char32_t ch) -> int {
			unsigned int n_glyphs = line.size();
			for(int i = n_glyphs-1; i >= 0; i--) {
				if(line[i].data.ch == ch) {
					return i;
				}
			}
			return -1;
		};
		auto MoveGlyphsToNext = [](GlyphLine& curr, GlyphLine& next,
				unsigned int& curr_width, unsigned int& next_width, unsigned int amount) {
			for(int i = 0; i < amount; i++) {
				auto& glyph = curr.back();
				unsigned int delta_width = glyph.dims.width;
				next.insert(next.begin(), glyph);
				next_width += delta_width;
				curr.pop_back();
				curr_width -= delta_width;
			}
		};
		auto GetLineHeight = [](GlyphLine& line) -> unsigned int {
			unsigned int height = 0;
			unsigned int n_glyphs = line.size();
			for(int i = 0; i < n_glyphs; i++) {
				height = std::max<unsigned int>(height, line[i].dims.height);
			}
			return height;
		};

		// manual text wrapping
		const unsigned int max_width = bounds.width-scroll_frame-message_margin;

		// individual lines saved so far, along with their y offset
		std::vector<std::pair<GlyphLine, unsigned int>> lines;
		unsigned int total_width = 0; // maximum width between all lines
		unsigned int total_height = 0; // accumulated height from all lines

		GlyphLine glyphs_current; // list of glyphs and their dimensions on current line
		GlyphLine glyphs_next; // stores glyphs moved down to line below
		unsigned int width_current = 0; // total width of glyphs on current line
		unsigned int width_next = 0; // total width of glyphs on next line

		// break down whole message string into glyphs for processing.
		// glyph lookup is performed only at this stage, and their dimensions are saved for subsequent line width recalculations.
		ExtractGlyphs(msg.message_data->color_a, 1, glyphs_current, width_current);
		ExtractGlyphs(msg.message_data->color_b, 2, glyphs_current, width_current);
		ExtractGlyphs(msg.message_data->color_c, 0, glyphs_current, width_current);

		// break down message into fitting lines
		do {
			while(width_current > max_width) {
				// as long as current line exceeds maximum width,
				// move one word from this line down to the next one
				int last_space = FindLastGlyphOf(glyphs_current, ' ');
				if(last_space != -1 && last_space < glyphs_current.size()-1) {
					// there is a word that can be moved down
					MoveGlyphsToNext(glyphs_current, glyphs_next,
						width_current, width_next, glyphs_current.size()-last_space-1);
				} else {
					// there is not a whole word that can be moved down, so move individual characters.
					// this case happens when last character in current line is a space,
					// or when there are no spaces in the current line
					MoveGlyphsToNext(glyphs_current, glyphs_next, width_current, width_next, 1);
				}
			}
			// once line fits, check for line breaks
			int line_break = FindFirstGlyphOf(glyphs_current, '\n');
			if(line_break != -1) {
				MoveGlyphsToNext(glyphs_current, glyphs_next,
					width_current, width_next, glyphs_current.size()-line_break-1);
			}
			// save line
			lines.push_back(std::make_pair(glyphs_current, total_height));
			total_width = std::max<unsigned int>(total_width, width_current);
			total_height += GetLineHeight(glyphs_current);
			// repeat this work on the exceeding portion moved down to the line below
			glyphs_current = glyphs_next;
			glyphs_next.clear();
			width_current = width_next;
			width_next = 0;
		} while(glyphs_current.size() > 0);

		// once all lines have been saved
		// render them into a bitmap
		BitmapRef text_img = Bitmap::Create(total_width+1, total_height+1, true);
		int n_lines = lines.size();
		for(int i = 0; i < n_lines; i++) {
			auto& line = lines[i];
			int glyph_offset = 0;
			for(int j = 0; j < line.first.size(); j++) {
				auto& glyph = line.first[j];
				auto ret = glyph.data;
				if(EP_UNLIKELY(!ret)) continue;
				glyph_offset += Text::Draw(*text_img, glyph_offset, line.second,
					*Font::Default(), *current_theme, glyph.color, ret.ch, ret.is_exfont).x;
			}
		}
		msg.render_graphic = text_img;
		msg.dirty = false;
	}

	void AssertScrollBounds() {
		// maximum value for scroll_position (= amount of height escaping from the top)
		const unsigned int max_scroll = std::max<int>(0, scroll_content_height-bounds.height);
		scroll_position = std::max<int>(scroll_position, 0);
		scroll_position = std::min<int>(scroll_position, max_scroll);
	}

	void UpdateScrollBar() {
		if(scroll_content_height <= bounds.height) {
			// hide scrollbar if content isn't large enough for scroll
			scroll_box.SetX(bounds.x+bounds.width);
			return;
		}
		scroll_box.SetX(bounds.x+bounds.width-scroll_frame); // show scrollbar
		// position scrollbar
		const float ratio = bounds.height/float(scroll_content_height);
		const unsigned int bar_height = bounds.height*ratio;
		const unsigned int bar_y = scroll_position*ratio;
		scroll_box.SetHeight(bar_height);
		scroll_box.SetY(bounds.y+bounds.height-bar_y-bar_height);
	}

	// called when:
	// - scroll position changes
	// - content height changes
	// - visible height (bounds.height) changes
	void RefreshScroll() {
		AssertScrollBounds();
		UpdateScrollBar();
	}

	bool MessageVisible(DrawableChatEntry& msg, unsigned short v) {
		return (msg.message_data->visibility & v) > 0;
	}
public:
	DrawableChatLog(int x, int y, int w, int h)
		: Drawable(Priority::Priority_Maximum, Drawable::Flags::Global),
		bounds(x, y, w, h),
		scroll_box(0, 0, scroll_frame+scroll_bleed, 0, Drawable::Flags::Global)
	{
		DrawableMgr::Register(this);

		scroll_box.SetZ(Priority::Priority_Maximum);
		scroll_box.SetVisible(false);

		current_theme = Cache::SystemOrBlack();
	}

	void SetHeight(unsigned int h) {
		bounds.height = h;
		RefreshScroll();
	}

	void Draw(Bitmap& dst) {
		// y offset to draw next message, from bottom of log panel
		int next_height = -scroll_position;
		unsigned int n_messages = messages.size();
		for(int i = n_messages-1; i >= 0; i--) {
			DrawableChatEntry& dmsg = messages[i];
			//skip drawing hidden messages
			if(!MessageVisible(dmsg, visibility_flags))
				continue;
			auto rect = dmsg.render_graphic->GetRect();
			// accumulate y offset
			next_height += rect.height;
			// skip drawing offscreen messages, but still accumulate y offset (bottom offscreen)
			if(next_height <= 0)
				continue;
			// cutoff message graphic so text does not bleed out of bounds
			const unsigned int top_offscreen = std::max<int>(next_height-bounds.height, 0);
			Rect cutoff_rect = Rect(rect.x, rect.y+top_offscreen,
				rect.width, std::min<unsigned int>(rect.height, next_height)-top_offscreen);
			// rebuild message graphic if needed
			if(messages[i].dirty)
				BuildMessageGraphic(dmsg);
			//draw
			dst.Blit(bounds.x+message_margin, bounds.y+bounds.height-next_height+top_offscreen,
				*(dmsg.render_graphic), cutoff_rect, Opacity::Opaque());
			// stop drawing offscreen messages (top offscreen)
			if(next_height > bounds.height)
				break;
		}
	};

	void RefreshTheme() {
		auto new_theme = Cache::SystemOrBlack();

		// do nothing if theme hasn't changed
		if(new_theme == current_theme)
			return;

		current_theme = new_theme;
		scroll_box.SetWindowskin(current_theme);
		for(int i = 0; i < messages.size(); i++) {
			// all messages now need to be redrawn with different UI skin
			messages[i].dirty = true;
		}
	}

	void AddChatEntry(ChatEntry* message_data) {
		DrawableChatEntry d_msg = DrawableChatEntry(message_data);
		BuildMessageGraphic(d_msg);
		messages.push_back(d_msg);

		if(MessageVisible(d_msg, visibility_flags)) {
			scroll_content_height += d_msg.render_graphic->GetRect().height;
			RefreshScroll();
		}
	}

	void RemoveChatEntry(ChatEntry* message_data) {
		unsigned int n_msgs = messages.size();
		for(int i = 0; i < n_msgs; i++) {
			DrawableChatEntry& d_msg = messages[i];
			if(d_msg.message_data == message_data) {
				if(MessageVisible(d_msg, visibility_flags)) {
					scroll_content_height -= d_msg.render_graphic->GetRect().height;
					RefreshScroll();
				}
				messages.erase(messages.begin()+i);
				break;
			}
		}
	}

	void SetScroll(int s) {
		scroll_position = s;
		RefreshScroll();
	}

	void Scroll(int delta) {
		SetScroll(scroll_position+delta);
	}

	void ShowScrollBar(bool v) {
		scroll_box.SetVisible(v);
	}

	void ToggleVisibilityFlag(VisibilityType v) {
		// Expands/collapses messages in-place,
		// so you don't get lost if you've scrolled far up.
		//
		// Finds the bottommost (before the change) message that is visible both before and after changing visibility flags, and
		// anchors it into place so it stays at the same visual location before and
		// after expanding/collapsing.

		unsigned short new_visibility_flags = visibility_flags ^ v;
		// calculate new total content height for new visibility flags.
		unsigned int new_content_height = 0;
		// save anchor position before and after
		int pre_anchor_y = -scroll_position;
		int post_anchor_y = -scroll_position;
		// if true, anchor has been found, so stop accumulating message heights
		bool anchored = false;

		for(int i = messages.size()-1; i >= 0; i--) {
			// is message visible with previous visibility mask?
			bool pre_vis = MessageVisible(messages[i], visibility_flags);
			// is message visible with new visibility mask?
			bool post_vis = MessageVisible(messages[i], new_visibility_flags);
			unsigned int msg_height = messages[i].render_graphic->GetRect().height;
			// accumulate total content height for new visibility flags
			if(post_vis) new_content_height += msg_height;

			if(!anchored) {
				if(pre_vis)
					pre_anchor_y += msg_height;
				if(post_vis)
					post_anchor_y += msg_height;
				// this message is an anchor candidate since it is visible both before and after
				bool valid_anchor = pre_vis && post_vis;
				if(valid_anchor && pre_anchor_y > 0) {
					// this is the bottommost anchorable message
					anchored = true;
				}
			}
		}
		// updates scroll content height
		scroll_content_height = new_content_height;
		// adjusts scroll position so anchor stays in place
		int scroll_delta = post_anchor_y-pre_anchor_y;
		scroll_position += scroll_delta;
		RefreshScroll();
		// set new visibility mask
		visibility_flags = new_visibility_flags;
	}
};

class DrawableTypeBox : public Drawable {
	Rect bounds;
	//design parameters
	// amount that is visible outside the padded bounds (so text can be seen beyond a left or rightmost placed caret)
	const unsigned int type_bleed = 3;
	const unsigned int type_padding_horz = 9; // padding between type box edges and content (left)
	const unsigned int type_padding_vert = 6; // padding between type box edges and content (top)
	const unsigned int label_padding_horz = 8; // left margin between label text and bounds
	const unsigned int label_padding_vert = 6; // top margin between label text and bounds
	const unsigned int label_margin = 40; // left margin for type box to make space for the label

	BitmapRef type_text;
	BitmapRef label;
	BitmapRef caret;
	unsigned int caret_index_tail = 0;
	unsigned int caret_index_head = 0;
	// cumulative x offsets for each character in the type box. Used for rendering the caret
	std::vector<unsigned int> type_char_offsets;
	unsigned int scroll = 0; // horizontal scrolling of type box

	unsigned int get_label_margin() {
		return (label->GetRect().width>1) ? label_margin : 0;
	}
public:
	DrawableTypeBox(int x, int y, int w, int h)
		: Drawable(Priority::Priority_Maximum, Drawable::Flags::Global),
		bounds(x, y, w, h)
	{
		DrawableMgr::Register(this);

		// create caret (type cursor) graphic
		std::string caret_char = "｜";
		const unsigned int caret_left_kerning = 6;
		auto c_rect = Text::GetSize(*Font::Default(), caret_char);
		caret = Bitmap::Create(c_rect.width+1-caret_left_kerning, c_rect.height+1, true);
		Text::Draw(*caret, -caret_left_kerning, 0, *Font::Default(), *Cache::SystemOrBlack(), 0, caret_char);

		//initialize
		UpdateTypeText(std::u32string());
		SetLabel("");
	}

	void Draw(Bitmap& dst) {
		const unsigned int label_pad = get_label_margin();
		const unsigned int type_visible_width = bounds.width-type_padding_horz*2-label_pad;
		auto rect = type_text->GetRect();
		// crop type text to stay within padding
		Rect cutoff_rect = Rect(scroll-type_bleed, rect.y,
			std::min<int>(type_visible_width, rect.width-scroll)+type_bleed*2, rect.height);

		// draw contents
		dst.Blit(bounds.x+label_pad+type_padding_horz-type_bleed, bounds.y+type_padding_vert,
			*type_text, cutoff_rect, Opacity::Opaque());
		// draw caret
		dst.Blit(bounds.x+label_pad+type_padding_horz+type_char_offsets[caret_index_head]-scroll,
			bounds.y+type_padding_vert, *caret, caret->GetRect(), Opacity::Opaque());
		// draw label
		dst.Blit(bounds.x+label_padding_horz, bounds.y+label_padding_vert,
			*label, label->GetRect(), Opacity::Opaque());
		// draw selection
		const unsigned int caret_start = std::min<unsigned int>(caret_index_tail, caret_index_head);
		const unsigned int caret_end = std::max<unsigned int>(caret_index_tail, caret_index_head);
		const unsigned int select_start = bounds.x+label_pad+type_padding_horz
			+std::max<int>(type_char_offsets[caret_start]-scroll, -type_bleed);
		const unsigned int select_end = bounds.x+label_pad+type_padding_horz
			+std::min<int>(type_char_offsets[caret_end]-scroll, type_visible_width+type_bleed);
		Rect selected_rect = Rect(select_start, bounds.y+type_padding_vert,
			select_end-select_start, bounds.height-type_padding_vert*2);
		dst.FillRect(selected_rect, Color(255, 255, 255, 100));
	};

	void RefreshTheme() { }

	void UpdateTypeText(std::u32string text) {
		// get char offsets for each character in type box, for caret positioning
		type_char_offsets.clear();
		Rect accumulated_rect;
		const unsigned int n_chars = text.size();
		for(int k = 0; k <= n_chars; k++) {
			// for every substring of sizes 0 to N, inclusive, starting at the left
			std::u32string text_so_far = text.substr(0, k);
			// absolute offset of character at this point (considering kerning of all previous ones)
			accumulated_rect = Text::GetSize(*Font::Default(), Utils::EncodeUTF(text_so_far));
			type_char_offsets.push_back(accumulated_rect.width);
		}

		// final value assigned to accumulated_rect is whole type string
		// create Bitmap graphic for text
		type_text = Bitmap::Create(accumulated_rect.width+1, accumulated_rect.height+1, true);
		Text::Draw(*type_text, 0, 0, *Font::Default(), *Cache::SystemOrBlack(), 0, Utils::EncodeUTF(text));
	}

	void SeekCaret(unsigned int seek_tail, unsigned int seek_head) {
		caret_index_tail = seek_tail;
		caret_index_head = seek_head;
		// adjust type box horizontal scrolling based on caret position (always keep it in-bounds)
		const unsigned int label_pad = get_label_margin();
		const unsigned int type_visible_width = bounds.width-type_padding_horz*2-label_pad;
		// absolute offset of caret in relation to type text contents
		const unsigned int caret_offset = type_char_offsets[caret_index_head];
		// caret's position relative to viewable portion of type box
		const int relative_offset = caret_offset-scroll;
		if(relative_offset < 0) {
			// caret escapes from left side. adjust
			scroll += relative_offset;
		} else if(relative_offset >= type_visible_width) {
			// caret escapes from right side. adjust
			scroll += relative_offset-type_visible_width;
		}
	}

	void SetLabel(std::string l) {
		std::string prompt = (l!="")?l+":":"";
		auto l_rect = Text::GetSize(*Font::Default(), prompt);
		label = Bitmap::Create(l_rect.width+1, l_rect.height+1, true);
		Text::Draw(*label, 0, 0, *Font::Default(), *Cache::SystemOrBlack(), 1, prompt);
	}

	Rect GetFormBounds() {
		const unsigned int label_pad = get_label_margin();
		return Rect(bounds.x+label_pad, bounds.y, bounds.width-label_pad, bounds.height);
	}
};

class DrawableChatUi : public Drawable {
	//design parameters
	const unsigned int panel_bleed = 16; // how much to stretch top, bottom and right edges of panel offscreen (so only left frame shows)
	const unsigned int panel_frame = 4; // width of panel's visual frame (on left side)
	const unsigned int status_height = 24; // height of status region on top of chatlog
	const unsigned int type_height = 24; // height of type box
	const unsigned int fullscreen_icon_margin = 10; // right margin for status text to make space for fullscreen toggle icon
	const unsigned int chat_width = 160;

	DrawableOnlineStatus d_status;
	DrawableChatLog d_log;
	DrawableTypeBox d_type;
	Window_Base back_panel; // background pane

	void UpdateTypePanel() {
		if(d_type.IsVisible()) {
			// SetCursorRect for some reason already has a padding of 8px relative to the window, so we fix it
			const unsigned int f = -8;
			const auto form_rect = d_type.GetFormBounds();
			back_panel.SetCursorRect(
				Rect(f+form_rect.x, f+form_rect.y+panel_bleed,
					form_rect.width, form_rect.height));
		} else {
			back_panel.SetCursorRect(Rect(0, 0, 0, 0));
		}
	}
public:
	DrawableChatUi()
		: Drawable(Priority::Priority_Maximum, Drawable::Flags::Global),
		// stretch 16px at right, top and bottom sides so the edge frame only shows on left
		back_panel(0, -panel_bleed, chat_width+panel_bleed,
			SCREEN_TARGET_HEIGHT+panel_bleed*2, Drawable::Flags::Global),
		d_log(0+panel_frame, status_height, chat_width-panel_frame,
			SCREEN_TARGET_HEIGHT-status_height),
		d_type(0+panel_frame,
			SCREEN_TARGET_HEIGHT-type_height, chat_width-panel_frame, type_height),
		d_status(0+panel_frame, 0, chat_width-panel_frame-fullscreen_icon_margin, status_height)
	{
		DrawableMgr::Register(this);

		back_panel.SetZ(Priority::Priority_Maximum-1);
		d_type.SetVisible(false);
	}

	void Draw(Bitmap& dst) { }

	void AddLogEntry(ChatEntry* msg) {
		d_log.AddChatEntry(msg);
	}

	void RemoveLogEntry(ChatEntry* msg) {
		d_log.RemoveChatEntry(msg);
	}

	void SetStatusConnection(bool conn) {
		d_status.SetConnectionStatus(conn);
	}

	void SetStatusRoom(unsigned int room_id) {
		d_status.SetRoomStatus(room_id);
	}

	void RefreshTheme() {
		back_panel.SetWindowskin(Cache::SystemOrBlack());
		d_log.RefreshTheme();
		d_type.RefreshTheme();
		d_status.RefreshTheme();
	}

	void UpdateTypeText(std::u32string text, unsigned int caret_seek_tail, unsigned int caret_seek_head) {
		d_type.UpdateTypeText(text);
		d_type.SeekCaret(caret_seek_tail, caret_seek_head);
	}

	void UpdateTypeCaret(unsigned int caret_seek_tail, unsigned int caret_seek_head) {
		d_type.SeekCaret(caret_seek_tail, caret_seek_head);
	}

	void ShowTypeLabel(std::string label) {
		d_type.SetLabel(label);
		UpdateTypePanel();
	}

	void ScrollUp() {
		d_log.Scroll(+4);
	}

	void ScrollDown() {
		d_log.Scroll(-4);
	}

	void SetFocus(bool focused) {
		d_type.SetVisible(focused);
		UpdateTypePanel();
		d_log.ShowScrollBar(focused);
		if(focused) {
			d_log.SetHeight(SCREEN_TARGET_HEIGHT-status_height-type_height);
		} else {
			d_log.SetHeight(SCREEN_TARGET_HEIGHT-status_height);
			d_log.SetScroll(0);
		}
	}

	void ToggleVisibilityFlag(VisibilityType v) {
		d_log.ToggleVisibilityFlag(v);
	}
};

bool chat_focused = false;

const unsigned int MAXCHARSINPUT_NAME = 8;
const unsigned int MAXCHARSINPUT_TRIPCODE = 256;
const unsigned int MAXCHARSINPUT_MESSAGE = 200;

const unsigned int MAXMESSAGES = 100;

std::string cache_name = ""; // name and tripcode are input in separate steps. Save it to send them together.
std::u32string preload_trip; // saved tripcode preference to load into trip type box once name has been sent.
std::unique_ptr<DrawableChatUi> chat_box; // chat renderer
std::vector<std::unique_ptr<ChatEntry>> chat_log;

void AddLogEntry(std::string a, std::string b, std::string c, VisibilityType v) {
	chat_log.push_back(std::make_unique<ChatEntry>(a, b, c, v));
	chat_box->AddLogEntry(chat_log.back().get());
	if(chat_log.size() > MAXMESSAGES) {
		chat_box->RemoveLogEntry(chat_log.front().get());
		chat_log.erase(chat_log.begin());
	}
}

void SetTypeText(std::u32string text) {
}

void SetTypeMaxChars(unsigned int c) {
}

void ProcessAndSendMessage(std::string utf8text) {
}

void Initialize() {
	chat_box = std::make_unique<DrawableChatUi>();
	chat_box->ShowTypeLabel("Name");

	// load saved user profile preferences from JS side (name)
	std::string cfg_name_str = "Test";
	SetTypeText(Utils::DecodeUTF32(cfg_name_str));
	SetTypeMaxChars(MAXCHARSINPUT_NAME);
	// load saved user profile preferences from JS side (trip)
	std::string cfg_trip_str = "Trip";
	preload_trip = Utils::DecodeUTF32(cfg_trip_str);

	AddLogEntry("", "!! • IME input now supported!", "", CV_LOCAL);
	AddLogEntry("", "!!   (for Japanese, etc.)", "", CV_LOCAL);
	AddLogEntry("", "!! • You can now copy and", "", CV_LOCAL);
	AddLogEntry("", "!!   paste from type box.", "", CV_LOCAL);
	AddLogEntry("", "!! • SHIFT+[←, →] to select text.", "", CV_LOCAL);
	AddLogEntry("", "", "―――", CV_LOCAL);

	AddLogEntry("[TAB]: ", "focus/unfocus.", "", CV_LOCAL);
	AddLogEntry("[↑, ↓]: ", "scroll.", "", CV_LOCAL);
	AddLogEntry("[F8]: ", "hide/show global chat.", "", CV_LOCAL);
	AddLogEntry("", "", "―――", CV_LOCAL);
	AddLogEntry("• Type /help to list commands.", "", "", CV_LOCAL);
	AddLogEntry("• Use '!' at the beginning of", "", "", CV_LOCAL);
	AddLogEntry("  message for global chat.", "", "", CV_LOCAL);
	AddLogEntry("", "", "―――", CV_LOCAL);
	AddLogEntry("• Set a nickname", "", "", CV_LOCAL);
	AddLogEntry("  for chat.", "", "", CV_LOCAL);
	AddLogEntry("• Max 8 characters.", "", "", CV_LOCAL);
	AddLogEntry("• Alphanumeric only.", "", "", CV_LOCAL);
}

void SetFocus(bool focused) {
	chat_focused = focused;
	//Input::set_game_focus(!focused);
	chat_box->SetFocus(focused);
}

//void InputsFocusUnfocus() {
//	if(Input::IsTriggered(Input::InputButton::CHAT_FOCUS)) {
//		SetFocus(true);
//	} else if(Input::IsExternalTriggered(Input::InputButton::CHAT_UNFOCUS)) {
//		SetFocus(false);
//	}
//}
//
//void InputsLog() {
//	if(Input::IsExternalPressed(Input::InputButton::CHAT_UP)) {
//		chat_box->ScrollUp();
//	}
//	if(Input::IsExternalPressed(Input::InputButton::CHAT_DOWN)) {
//		chat_box->ScrollDown();
//	}
//	if(Input::IsExternalTriggered(Input::InputButton::CHAT_TOGGLE_GLOBAL)) {
//		chat_box->ToggleVisibilityFlag(CV_GLOBAL);
//	}
//}
//
//void ProcessInputs() {
//	InputsFocusUnfocus();
//	InputsLog();
//}

/**
 * External access
 */

static ChatUi _instance;

ChatUi& ChatUi::Instance() {
	return _instance;
}

bool init_wait_scene = false;

void ChatUi::Refresh() {
	if(chat_box == nullptr) {
		if (init_wait_scene)
			return;
		init_wait_scene = true;
		std::thread thread([]() {
			while(true) {
				auto scene_title = Scene::Find(Scene::SceneType::Title);
				auto scene_map = Scene::Find(Scene::SceneType::Map);
				if(scene_title != nullptr || scene_map != nullptr) {
					Initialize();
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		});
		thread.detach();
	} else {
		chat_box->RefreshTheme();
	}
}

void ChatUi::Update() {
	if(chat_box == nullptr)
		return;
	//ProcessInputs();
}

void ChatUi::GotMessage(std::string name, std::string trip, std::string msg, std::string src) {
	if(chat_box == nullptr)
		return;
	AddLogEntry(
		(src=="G"?"G← ":"")+name,
		"•"+trip+":\n",
		"\u00A0"+msg,
		src=="G"?CV_GLOBAL:CV_LOCAL
	);
}

void ChatUi::GotInfo(std::string msg) {
	if(chat_box == nullptr)
		return;
	// remove leading spaces. TO-DO: fix leading spaces being sent by server on info messages.
	auto leading_spaces = msg.find_first_not_of(' ');
	auto trim = msg.substr(leading_spaces != std::string::npos ? leading_spaces : 0);
	AddLogEntry("", trim, "", CV_LOCAL);
}

void ChatUi::SetStatusConnection(bool status) {
	if(chat_box == nullptr)
		return;
	chat_box->SetStatusConnection(status);
}

void ChatUi::SetStatusRoom(unsigned int room_id) {
	if(chat_box == nullptr)
		return;
	chat_box->SetStatusRoom(room_id);
}