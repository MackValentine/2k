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
#include "window_item_custom.h"
#include "game_party.h"
#include "bitmap.h"
#include "font.h"
#include <lcf/reader_util.h>
#include "game_battle.h"
#include "output.h"

Window_Item_Custom::Window_Item_Custom(int ix, int iy, int iwidth, int iheight) :
	Window_Item(ix, iy, iwidth, iheight) {
	column_max = 2;
}

void Window_Item_Custom::Refresh() {
		std::vector<int> party_items;

		data.clear();
		Main_Data::game_party->GetItems(party_items);

		for (size_t i = 0; i < party_items.size(); ++i) {
			if (this->CheckInclude(party_items[i]) && IsValidPocket(party_items[i])) {
				data.push_back(party_items[i]);
			}
		}

		if (Game_Battle::IsBattleRunning()) {
			// Include equipped accessories that invoke skills in sorted order.
			if (actor) {
				for (int i = 1; i <= 5; ++i) {
					const lcf::rpg::Item* item = actor->GetEquipment(i);
					if (item && item->use_skill && item->skill_id > 0) {
						auto iter = std::lower_bound(data.begin(), data.end(), item->ID);
						if (iter == data.end() || *iter != item->ID) {
							data.insert(iter, item->ID);
						}
					}
				}
			}
		}

		if (CheckInclude(0)) {
			data.push_back(0);
		}

		item_max = data.size();

		CreateContents();

		SetIndex(index);

		contents->Clear();

		for (int i = 0; i < item_max; ++i) {
			DrawItem(i);
		}
	}

bool Window_Item_Custom::IsValidPocket(int i) {
	const lcf::rpg::Item* item = lcf::ReaderUtil::GetElement(lcf::Data::items, i);
	//(pocket == -1 || pocket == item->type)
	if (pocket == -1)
		return true;

	if (pocket == -2)
		return item->type == 1 || item->type == 2 || item->type == 3 || item->type == 4 || item->type == 5;

	if (pocket == -3)
		return item->type == 6 || item->type == 7 || item->type == 8 || item->type == 9 || item->type == 10 || item->use_skill;

	return pocket == item->type;
}
