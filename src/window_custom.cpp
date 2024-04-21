/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include <iomanip>
#include <sstream>
#include "window_custom.h"
#include "game_party.h"
#include "main_data.h"
#include "bitmap.h"
#include "game_strings.h"

#include <scene_menu_custom.h>

Window_Custom::Window_Custom(int ix, int iy, int iwidth, int iheight, std::string t) :
	Window_Base(ix, iy, iwidth, iheight) {
	name = t;

	SetContents(Bitmap::Create(width - 16, height - 16));

	Refresh();
}

void Window_Custom::Refresh() {
	contents->Clear();
	std::string t = "";
	for (auto p : CustomMenu::customWindows[name].pages) {
		if ((p.condition >= 0 && CustomMenu::commandIndex == p.condition) || p.condition == -1) {
			t = p.text;
			align = p.align;
			break;
		}
	}
	t = Game_Strings::Extract(t, false);

	auto a = Text::AlignLeft;
	if (align == 1)
		a = Text::AlignCenter;
	else if (align == 2)
		a = Text::AlignRight;

	Rect r = Rect(0, 2, width - 16, height - 16);
	contents->TextDraw(r, Font::ColorDefault, t, a);
	// DrawCurrencyValue(Main_Data::game_party->GetGold(), contents->GetWidth(), 2);
}
