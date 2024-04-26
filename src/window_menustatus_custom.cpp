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
#include "window_menustatus_custom.h"
#include "cache.h"
#include "game_party.h"
#include "player.h"
#include "bitmap.h"
#include "feature.h"
#include "scene_menu_custom.h"
#include "game_strings.h"

Window_MenuStatus_Custom::Window_MenuStatus_Custom(int ix, int iy, int iwidth, int iheight) :
	Window_MenuStatus(ix, iy, iwidth, iheight) {

	if (Player::IsRPG2k3()) {
		SetContents(Bitmap::Create(width - 12, height - 16));
		SetBorderX(4);
		text_offset = 4;
	} else {
		SetContents(Bitmap::Create(width - 16, height - 16));
	}

	Refresh();
}

void Window_MenuStatus_Custom::Refresh() {
	contents->Clear();

	item_max = Main_Data::game_party->GetActors().size();

	auto it3 = CustomMenu::customWindows.find("Status");
	customised = it3 != CustomMenu::customWindows.end();
	if (!customised) {
		for (int i = 0; i < item_max; ++i) {
			// The party always contains valid battlers
			const Game_Actor& actor = *(Main_Data::game_party->GetActors()[i]);

			int face_x = 0;
			if (Player::IsRPG2k3()) {
				if (!Feature::HasRow()) {
					face_x = 4;
				}
				else {
					face_x = actor.GetBattleRow() == Game_Actor::RowType::RowType_back ? 8 : 0;
				}
			}
			DrawActorFace(actor, face_x, i * menu_item_height + y);

			DrawActorName(actor, 48 + 8 + text_offset, i * menu_item_height + 2 + y);
			DrawActorTitle(actor, 48 + 8 + 88 + text_offset, i * menu_item_height + 2 + y);
			DrawActorLevel(actor, 48 + 8 + text_offset, i * menu_item_height + 2 + 16 + y);
			DrawActorState(actor, 48 + 8 + 42 + text_offset, i * menu_item_height + 2 + 16 + y);
			DrawActorExp(actor, 48 + 8 + text_offset, i * menu_item_height + 2 + 16 + 16 + y);
			int digits = (actor.MaxHpValue() >= 1000 || actor.MaxSpValue() >= 1000) ? 4 : 3;
			DrawActorHp(actor, 48 + 8 + 106 + text_offset - (digits == 3 ? 0 : 12), i * menu_item_height + 2 + 16 + y, digits);
			DrawActorSp(actor, 48 + 8 + 106 + text_offset - (digits == 3 ? 0 : 12), i * menu_item_height + 2 + 16 + 16 + y, digits);

		}
	} else {
		MenuCustomWindow win = CustomMenu::customWindows["Status"];

		int y = 0;
		for (int i = 0; i < item_max; ++i) {
			if (win.stats.empty()) {
				// The party always contains valid battlers
				const Game_Actor& actor = *(Main_Data::game_party->GetActors()[i]);

				int face_x = 0;
				if (Player::IsRPG2k3()) {
					if (!Feature::HasRow()) {
						face_x = 4;
					}
					else {
						face_x = actor.GetBattleRow() == Game_Actor::RowType::RowType_back ? 8 : 0;
					}
				}
				DrawActorFace(actor, face_x, i * menu_item_height + y);

				DrawActorName(actor, 48 + 8 + text_offset, i * menu_item_height + 2 + y);
				DrawActorTitle(actor, 48 + 8 + 88 + text_offset, i * menu_item_height + 2 + y);
				DrawActorLevel(actor, 48 + 8 + text_offset, i * menu_item_height + 2 + 16 + y);
				DrawActorState(actor, 48 + 8 + 42 + text_offset, i * menu_item_height + 2 + 16 + y);
				DrawActorExp(actor, 48 + 8 + text_offset, i * menu_item_height + 2 + 16 + 16 + y);
				int digits = (actor.MaxHpValue() >= 1000 || actor.MaxSpValue() >= 1000) ? 4 : 3;
				DrawActorHp(actor, 48 + 8 + 106 + text_offset - (digits == 3 ? 0 : 12), i * menu_item_height + 2 + 16 + y, digits);
				DrawActorSp(actor, 48 + 8 + 106 + text_offset - (digits == 3 ? 0 : 12), i * menu_item_height + 2 + 16 + 16 + y, digits);

				//y += 10;

			}
			else {
				for (auto stat : CustomMenu::customWindows["Status"].stats) {
					if (!stat.unique) {

						auto align = Text::AlignLeft;
						if (stat.align == 1)
							align = Text::AlignCenter;
						else if (stat.align == 2)
							align = Text::AlignRight;
						int tx = stat.x + (i % column_max) * (GetWidth() - 24) / column_max;
						int ty = stat.y + (i / column_max - GetTopRow()) * (menu_item_height)+2 + y;

						Output::Debug("{} {} {} {} {} {}", i, column_max, GetWidth(), (i % column_max) * (GetWidth() - 24) / column_max, (i % column_max), (GetWidth() - 24) / column_max);

						// cursor_rect = { 4 + text_offset + (index % column_max) * (GetWidth() - 24) / column_max, (index / column_max - GetTopRow()) * (menu_item_height + 10), (GetWidth() - 24) / column_max, menu_item_height };

						contents->TextDraw(tx, ty, Font::ColorDefault, ParserText(stat.text, i, tx, ty, align), align);
					}
				}
			}
		}
		for (auto stat : CustomMenu::customWindows["Status"].stats) {
			if (stat.unique) {
				//DrawActorName(actor, 48 + 8 + text_offset, i * 48 + 2 + y);
				auto align = Text::AlignLeft;
				if (stat.align == 1)
					align = Text::AlignCenter;
				else if (stat.align == 2)
					align = Text::AlignRight;
				int tx = stat.x;
				int ty = stat.y;
				std::string text = Game_Strings::Extract(stat.text, false);
				text = ParserText(text, 0, tx, ty, align);
				contents->TextDraw(tx, ty, Font::ColorDefault, text, align);
			}
		}
	}

}
std::string Window_MenuStatus_Custom::replaceFunction(const std::smatch& match, int index, int tx, int ty, Text::Alignment align) {
	// Le premier groupe de capture (match[1]) contient la valeur entre crochets
	std::string variable = match[1].str();
	int i = 0;
	if (variable == "@id") {
		i = index;
	}
	else {
		i = atoi(variable.c_str());
	}
	const Game_Actor& actor = *(Main_Data::game_party->GetActors()[i]);

	// Le second groupe de capture (match[2]) contient la valeur après le point
	std::string type = match[2].str();

	// Output::Debug("Regex : {} {}", variable, type);

	// Exemple de traitement en fonction des valeurs capturées
	// Vous pouvez mettre votre propre logique ici
	if (type == "name") {
		auto s = actor.GetName();
		std::string t = { s.begin(), s.end() };
		return t;
	} else 	if (type == "hp") {
		std::string t = std::to_string(actor.GetHp());
		return t;
	} else if (type == "mhp") {
		std::string t = std::to_string(actor.GetMaxHp());
		return t;
	}
	else if (type == "mp") {
		std::string t = std::to_string(actor.GetSp());
		return t;
	}
	else if (type == "mmp") {
		std::string t = std::to_string(actor.GetMaxSp());
		return t;
	}
	else if (type == "exp") {
		std::string t = std::to_string(actor.GetExp());
		return t;
	}
	else if (type == "mexp") {
		std::string t = std::to_string(actor.GetNextExp());
		return t;
	}
	else if (type == "level") {
		std::string t = std::to_string(actor.GetLevel());
		return t;
	}
	else if (type == "class") {
		auto s = actor.GetClassName();
		std::string t = { s.begin(), s.end() };
		return t;
	}
	else if (type == "title") {
		auto s = actor.GetTitle();
		std::string t = { s.begin(), s.end() };
		return t;
	}
	else if (type == "state") {
		const lcf::rpg::State* state = actor.GetSignificantState();
		std::string t;
		if (!state) {
			auto s = lcf::Data::terms.normal_status;
			t = { s.begin(), s.end() };
		}
		else {
			auto s = state->name;
			t = { s.begin(), s.end() };
		}

		return t;
	}
	else if (type == "face") {
		if (align == Text::AlignCenter)
			tx -= 24;
		else if (align == Text::AlignRight)
			tx -= 48;

		DrawActorFace(actor, tx, ty);
		return "";
	}
	else if (type == "jhp") {
		if (align == Text::AlignCenter)
			tx -= 12 + 16;
		else if (align == Text::AlignRight)
			tx -= 25 + 16;

		DrawCustomGauge(actor, 0, actor.GetHp(), actor.GetMaxHp(), tx, ty);
		return "";
	}
	else if (type == "jmp") {
		if (align == Text::AlignCenter)
			tx -= 12 + 16;
		else if (align == Text::AlignRight)
			tx -= 25 + 16;

		DrawCustomGauge(actor, 1, actor.GetSp(), actor.GetMaxSp(), tx, ty);
		return "";
	}
	else {
		return "";
	}
}


std::string Window_MenuStatus_Custom::ParserText(std::string text, int index, int tx, int ty, Text::Alignment align) {
	std::regex pattern("\\\\a\\[(.*?)\\]\\.([^\\s]*)");

	// Utilisation de std::regex_iterator pour parcourir les correspondances
	std::sregex_iterator it(text.begin(), text.end(), pattern);
	std::sregex_iterator end;

	// Résultat final
	std::string resultat;

	// Parcours des correspondances
	size_t lastPos = 0;
	while (it != end) {
		// Ajout du texte entre les correspondances précédentes et actuelles
		resultat += it->prefix();

		// Appel de la fonction de remplacement et ajout du résultat au texte final
		resultat += replaceFunction(*it, index, tx, ty, align);

		// Déplacement au prochain match
		lastPos = it->position() + it->length();
		++it;
	}

	// Ajout du texte après la dernière correspondance
	resultat += text.substr(lastPos);



	return resultat;
}


void Window_MenuStatus_Custom::UpdateCursorRect()
{
	if (index < 0) {
		cursor_rect = { 0, 0, 0, 0 };
	} else if (customised){
		if (CustomMenu::customWindows["Status"].stats.empty())
			cursor_rect = { 48 + 4 + text_offset, (index - GetTopRow()) * (menu_item_height), 168, menu_item_height };
		else
			cursor_rect = { 4 + text_offset + (index % column_max) * (GetWidth() - 24) / column_max, (index / column_max - GetTopRow()) * (menu_item_height), (GetWidth() - 24)/column_max, menu_item_height };
		// cursor_rect = { 4 + text_offset, (index - GetTopRow()) * (menu_item_height + 10), (GetWidth() - 24)/column_max, menu_item_height };
	}
	else {
		cursor_rect = { 48 + 4 + text_offset, (index - GetTopRow()) * (menu_item_height), 168, menu_item_height };
	}
}

Game_Actor* Window_MenuStatus_Custom::GetActor() const {
	return &(*Main_Data::game_party)[GetIndex()];
}
