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
#include "scene_item_custom.h"
#include "game_map.h"
#include "game_party.h"
#include "game_player.h"
#include "game_switches.h"
#include "game_system.h"
#include "game_targets.h"
#include "input.h"
#include <lcf/reader_util.h>
#include "scene_actortarget.h"
#include "scene_map.h"
#include "scene_teleport.h"
#include "output.h"
#include "transition.h"
#include <cache.h>


namespace CustomItem {
	std::map<std::string, MenuCustomWindow> customWindows;

	bool showMap;
	std::string backName;

	int commandIndex = -1;

	int parallax_x = 0;
	int parallax_y = 0;

	bool usePockets = false;

	std::vector<int> pockets_list;
}

Scene_Item_Custom::Scene_Item_Custom(int item_index) :
	item_index(item_index) {
	Scene::type = Scene::Item;
	if (CustomItem::showMap)
		SetUseSharedDrawables(true);
}

void Scene_Item_Custom::Start() {
	// Create the windows
	int menu_help_height = 32;

	std::string win_name = "Help";
	auto it = CustomItem::customWindows.find(win_name);

	if (it != CustomItem::customWindows.end()) {
		menu_help_height = CustomItem::customWindows[win_name].h;
		int x = 0;
		int y = 0;
		int w = MENU_WIDTH;

		x = CustomItem::customWindows[win_name].x;
		y = CustomItem::customWindows[win_name].y;
		w = CustomItem::customWindows[win_name].w;

		help_window.reset(new Window_Help(x, y, w, menu_help_height));
	}
	else {
		help_window.reset(new Window_Help(Player::menu_offset_x, Player::menu_offset_y, MENU_WIDTH, menu_help_height));
	}

	win_name = "Items";
	it = CustomItem::customWindows.find(win_name);

	if (it != CustomItem::customWindows.end()) {
		int x = 0;
		int y = 0;
		int w = MENU_WIDTH;
		int h = MENU_HEIGHT;

		x = CustomItem::customWindows[win_name].x;
		y = CustomItem::customWindows[win_name].y;
		w = CustomItem::customWindows[win_name].w;
		h = CustomItem::customWindows[win_name].h;

		item_window.reset(new Window_Item_Custom(x,y,w,h));
		item_window->SetHelpWindow(help_window.get());
		item_window->SetColumnMax(CustomItem::customWindows[win_name].column);
		item_window->Refresh();
		item_window->SetIndex(item_index);
	}
	else {
		item_window.reset(new Window_Item(Player::menu_offset_x, Player::menu_offset_y + menu_help_height, MENU_WIDTH, MENU_HEIGHT - menu_help_height));
		item_window->SetHelpWindow(help_window.get());
		item_window->Refresh();
		item_window->SetIndex(item_index);
	}

	win_name = "Pockets";
	it = CustomItem::customWindows.find(win_name);

	if (it != CustomItem::customWindows.end()) {

		CustomItem::usePockets = true;

		int x = 0;
		int y = 0;
		int w = MENU_WIDTH;
		int h = MENU_HEIGHT;

		x = CustomItem::customWindows[win_name].x;
		y = CustomItem::customWindows[win_name].y;
		w = CustomItem::customWindows[win_name].w;
		h = CustomItem::customWindows[win_name].h;


		std::vector<std::string> options;
		// TODO Find a way to traduce them
		for (auto i : CustomItem::pockets_list) {
			std::string s = "All";
			switch(i) {
			case 0:
				s = "Good";
				break;
			case 1:
				s = "Weapon";
				break;
			case 2:
				s = "Shield";
				break;
			case 3:
				s = "Armor";
				break;
			case 4:
				s = "Head";
				break;
			case 5:
				s = "Accessory";
				break;
			case 6:
				s = "Medecine";
				break;
			case 7:
				s = "Book";
				break;
			case 8:
				s = "Seed";
				break;
			case 9:
				s = "Special";
				break;
			case 10:
				s = "Switch";
				break;
			case -2:
				s = "Equipment";
				break;
			case -3:
				s = "Consumables";
				break;
			default:
				s = "All";
				break;
			}

			options.push_back(s);
		}
		if (options.size() == 0)
			options.push_back("All");

		pocket_window.reset(new Window_Command_Custom(options, w, CustomItem::customWindows[win_name].column));
		pocket_window->SetX(x);
		pocket_window->SetY(y);
		pocket_window->SetHeight(h);
		pocket_window->SetIndex(0);
		item_window->Refresh();
		pocket_window->SetIndex(0);

		item_window->SetPocket(GetPocket());

		help_window->SetText("");
	}

	if (CustomItem::showMap) {
		spriteset.reset(new Spriteset_Map());
		Main_Data::game_screen->Update();
		Main_Data::game_pictures->Update(false);
	}

	CreateBackGraphic();
}

void Scene_Item_Custom::Continue(SceneType /* prev_scene */) {
	item_window->Refresh();
}

void Scene_Item_Custom::vUpdate() {
	help_window->Update();

	wait_parallax++;
	if (wait_parallax > 2) {
		int rx = backSprite->GetRenderOx() + CustomItem::parallax_x;
		backSprite->SetRenderOx(rx);
		int ry = backSprite->GetRenderOy() + CustomItem::parallax_y;
		backSprite->SetRenderOy(ry);
		wait_parallax = 0;
	}


	if (CustomItem::usePockets && pocket_window->GetActive()) {
		item_window->SetActive(false);

		if (pocket_window->GetIndex() != old_pocket_index) {
			item_window->SetPocket(GetPocket());
			item_window->Refresh();
			item_window->SetIndex(0);
			old_pocket_index = pocket_window->GetIndex();
		}
		help_window->SetText("");

		pocket_window->Update();
		if (Input::IsTriggered(Input::CANCEL)) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			Scene::Pop();
		}
		else if (Input::IsTriggered(Input::DECISION)) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			pocket_window->SetActive(false);
			item_window->SetActive(true);
		}
	}
	else {
		item_window->Update();

		if (Input::IsTriggered(Input::CANCEL)) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
			if (!CustomItem::usePockets)
				Scene::Pop();
			else
				pocket_window->SetActive(true);
		}
		else if (Input::IsTriggered(Input::DECISION)) {
			int item_id = item_window->GetItem() == NULL ? 0 : item_window->GetItem()->ID;

			if (item_id > 0 && item_window->CheckEnable(item_id)) {
				// The party only has valid items
				const lcf::rpg::Item& item = *item_window->GetItem();

				if (item.type == lcf::rpg::Item::Type_switch) {
					Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
					Main_Data::game_party->ConsumeItemUse(item_id);
					Main_Data::game_switches->Set(item.switch_id, true);
					Scene::PopUntil(Scene::Map);
					Game_Map::SetNeedRefresh(true);
				}
				else if (item.type == lcf::rpg::Item::Type_special && item.skill_id > 0) {
					const lcf::rpg::Skill* skill = lcf::ReaderUtil::GetElement(lcf::Data::skills, item.skill_id);
					if (!skill) {
						Output::Warning("Scene Item: Item references invalid skill ID {}", item.skill_id);
						return;
					}

					if (skill->type == lcf::rpg::Skill::Type_teleport) {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
						Scene::Push(std::make_shared<Scene_Teleport>(item, *skill));
					}
					else if (skill->type == lcf::rpg::Skill::Type_escape) {
						Main_Data::game_party->ConsumeItemUse(item_id);
						Main_Data::game_system->SePlay(skill->sound_effect);

						Main_Data::game_player->ForceGetOffVehicle();
						Main_Data::game_player->ReserveTeleport(Main_Data::game_targets->GetEscapeTarget());

						Scene::PopUntil(Scene::Map);
					}
					else if (skill->type == lcf::rpg::Skill::Type_switch) {
						Main_Data::game_party->ConsumeItemUse(item_id);
						Main_Data::game_system->SePlay(skill->sound_effect);
						Main_Data::game_switches->Set(skill->switch_id, true);
						Scene::PopUntil(Scene::Map);
						Game_Map::SetNeedRefresh(true);
					}
					else {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
						Scene::Push(std::make_shared<Scene_ActorTarget>(item_id));
						item_index = item_window->GetIndex();
					}
				}
				else {
					Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
					Scene::Push(std::make_shared<Scene_ActorTarget>(item_id));
					item_index = item_window->GetIndex();
				}
			}
			else {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
			}
		}
	}
}

void Scene_Item_Custom::TransitionOut(Scene::SceneType next_scene) {
	const auto* item = item_window->GetItem();
	const lcf::rpg::Skill* skill = nullptr;
	if (item && item->type == lcf::rpg::Item::Type_special && item->skill_id > 0) {
		skill = lcf::ReaderUtil::GetElement(lcf::Data::skills, item->skill_id);
	}

	if (next_scene == Map && skill && skill->type == lcf::rpg::Skill::Type_escape) {
		Transition::instance().InitErase(Transition::TransitionFadeOut, this);
	}
	else {
		Scene::TransitionOut(next_scene);
	}
}

void Scene_Item_Custom::CreateBackGraphic() {
	// Load Title Graphic
	if (!CustomItem::backName.empty()) {
		backSprite.reset(new Plane());
		FileRequestAsync* request = AsyncHandler::RequestFile("Picture", CustomItem::backName);
		request->SetGraphicFile(true);
		request_id = request->Bind(&Scene_Item_Custom::OnBackSpriteReady, this);
		request->Start();
	}
	else {
		backSprite.reset(new Plane());
		backSprite->SetBitmap(Bitmap::Create(Player::screen_width, Player::screen_height, Color(0, 0, 0, 255)));
	}
}

void Scene_Item_Custom::OnBackSpriteReady(FileRequestResult* result) {
	BitmapRef bitmapRef = Cache::Picture(result->file, true);

	backSprite->SetBitmap(bitmapRef);

	auto priority = Drawable::GetPriorityForMapLayer(8);

	if (priority > 0) {
		// Small offset (10) to ensure there is space for graphics that are
		// drawn at the image position (e.g. DynRPG Text Plugin)
		backSprite->SetZ(priority);
	}
}


int Scene_Item_Custom::GetPocket() {
	if (pocket_window->GetIndex() < CustomItem::pockets_list.size()) {
		return CustomItem::pockets_list[pocket_window->GetIndex()];
	}
	return -1;
}
