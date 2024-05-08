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
#include <cassert>
#include "scene_menu_custom.h"
#include "audio.h"
#include "cache.h"
#include "game_party.h"
#include "game_system.h"
#include "input.h"
#include "player.h"
#include "scene_settings.h"
#include "scene_debug.h"
#include "scene_end.h"
#include "scene_equip.h"
#include "scene_item.h"
#include "scene_skill.h"
#include "scene_order.h"
#include "scene_save.h"
#include "scene_status.h"
#include "bitmap.h"
#include "feature.h"
#include <output.h>
#include <window_custom.h>
#include <window_command_custom.h>
#include "game_switches.h"
#include "game_map.h"
#include "scene_item_custom.h"

constexpr int menu_command_width = 88;
constexpr int gold_window_width = 88;
constexpr int gold_window_height = 32;

namespace CustomMenu {
	std::map<std::string, MenuCustomWindow> customWindows;

	bool showMap;
	std::string backName;

	int commandIndex = -1;

	int parallax_x = 0;
	int parallax_y = 0;

	bool used = false;

	bool customCommands = false;

	std::string commands[16] = {""};
}

Scene_Menu_Custom::Scene_Menu_Custom(int menu_index) :
	menu_index(menu_index) {
	type = Scene::Menu;
	if (CustomMenu::showMap)
		SetUseSharedDrawables(true);
}

void Scene_Menu_Custom::Start() {
	CreateCommandWindow();

	BitmapRef system2 = Cache::System2();
	if (!system2) {
		if (!Cache::System2() && Main_Data::game_system->HasSystem2Graphic()) {
			FileRequestAsync* request = AsyncHandler::RequestFile("System2", Main_Data::game_system->GetSystem2Name());
			request->SetGraphicFile(true);
			request_id = request->Bind(&Scene_Menu_Custom::OnSystem2Ready, this);
			request->Start();
		} 
	}


	// Gold Window
	gold_window.reset(new Window_Gold(Player::menu_offset_x, (Player::screen_height - gold_window_height - Player::menu_offset_y), gold_window_width, gold_window_height));

	std::string win_name = "Gold";
	auto it2 = CustomMenu::customWindows.find(win_name);

	if (it2 != CustomMenu::customWindows.end()) {

		gold_window->SetX(CustomMenu::customWindows[win_name].x);
		gold_window->SetY(CustomMenu::customWindows[win_name].y);
		gold_window->SetWidth(CustomMenu::customWindows[win_name].w);
		gold_window->SetHeight(CustomMenu::customWindows[win_name].h);
		gold_window->SetOpacity(CustomMenu::customWindows[win_name].opacity);
		gold_window->SetBackOpacity(CustomMenu::customWindows[win_name].opacity);

		gold_window->SetVisible(!CustomMenu::customWindows[win_name].hide);
	}

	win_name = "Status";
	auto it3 = CustomMenu::customWindows.find(win_name);

	if (it3 != CustomMenu::customWindows.end()) {

		// Status Window
		int h = CustomMenu::customWindows[win_name].itemHeight * (Main_Data::game_party->GetBattlerCount() + 1) / CustomMenu::customWindows[win_name].column;
		menustatus_window.reset(new Window_MenuStatus_Custom(Player::menu_offset_x + menu_command_width, Player::menu_offset_y, CustomMenu::customWindows[win_name].w, h));
		menustatus_window->SetActive(false);

		menustatus_window->SetX(CustomMenu::customWindows[win_name].x);
		menustatus_window->SetY(CustomMenu::customWindows[win_name].y);
		menustatus_window->SetWidth(CustomMenu::customWindows[win_name].w);
		menustatus_window->SetHeight(CustomMenu::customWindows[win_name].h);

		menustatus_window->SetMenuItemHeight(CustomMenu::customWindows[win_name].itemHeight);

		menustatus_window->SetOpacity(CustomMenu::customWindows[win_name].opacity);
		menustatus_window->SetBackOpacity(CustomMenu::customWindows[win_name].opacity);

		menustatus_window->SetColumnMax(CustomMenu::customWindows[win_name].column);

		menustatus_window->SetVisible(!CustomMenu::customWindows[win_name].hide);

		menustatus_window->Refresh();
	}
	else {
		// Status Window
		menustatus_window.reset(new Window_MenuStatus_Custom(Player::menu_offset_x + menu_command_width, Player::menu_offset_y, (MENU_WIDTH - menu_command_width), MENU_HEIGHT));
		menustatus_window->SetMenuItemHeight(58);
		menustatus_window->SetActive(false);
		menustatus_window->Refresh();
	}

	CreateCustomWindows();

	if (CustomMenu::showMap) {
		spriteset.reset(new Spriteset_Map());
		Main_Data::game_screen->Update();
		Main_Data::game_pictures->Update(false);
	}

	CreateBackGraphic();

}

void Scene_Menu_Custom::CreateBackGraphic() {
	// Load Title Graphic
	if (!CustomMenu::backName.empty()) {
		backSprite.reset(new Plane());
		FileRequestAsync* request = AsyncHandler::RequestFile("Picture", CustomMenu::backName);
		request->SetGraphicFile(true);
		request_id = request->Bind(&Scene_Menu_Custom::OnBackSpriteReady, this);
		request->Start();
	}
	else {
		backSprite.reset(new Plane());
		backSprite->SetBitmap(Bitmap::Create(Player::screen_width, Player::screen_height, Color(0, 0, 0, 255)));
	}
}


void Scene_Menu_Custom::OnSystem2Ready(FileRequestResult* result) {
	Cache::SetSystem2Name(result->file);

	//SetupSystem2Graphics();
}

void Scene_Menu_Custom::OnBackSpriteReady(FileRequestResult* result) {
	BitmapRef bitmapRef = Cache::Picture(result->file, true);

	backSprite->SetBitmap(bitmapRef);

	auto priority = Drawable::GetPriorityForMapLayer(8);

	if (priority > 0) {
		// Small offset (10) to ensure there is space for graphics that are
		// drawn at the image position (e.g. DynRPG Text Plugin)
		backSprite->SetZ(priority);
	}
}

bool in_array(const std::string& value, const std::vector<std::string>& array)
{
	return std::find(array.begin(), array.end(), value) != array.end();
}

void Scene_Menu_Custom::CreateCustomWindows() {
	CustomMenu::commandIndex = command_window->GetIndex();

	std::vector<std::string> basic = {"Commands", "Gold", "Status"};
	for (auto p : CustomMenu::customWindows) {
		if (!in_array(p.first, basic)) {
			auto w = p.second;
			Window_Custom* win = new Window_Custom(w.x, w.y, w.w, w.h, w.name);
			win->SetVisible(!w.hide);
			win->SetOpacity(w.opacity);
			win->SetBackOpacity(w.opacity);
			win->SetZ(command_window->GetZ() - 1);
			custom_windows.push_back(win);
		}
	}
}

void Scene_Menu_Custom::Continue(SceneType /* prev_scene */) {
	menustatus_window->Refresh();
	gold_window->Refresh();
}

void Scene_Menu_Custom::vUpdate() {
	command_window->Update();
	gold_window->Update();
	menustatus_window->Update();

	wait_parallax++;
	if (wait_parallax > 2) {
		int rx = backSprite->GetRenderOx() + CustomMenu::parallax_x;
		backSprite->SetRenderOx(rx);
		int ry = backSprite->GetRenderOy() + CustomMenu::parallax_y;
		backSprite->SetRenderOy(ry);
		wait_parallax = 0;
	}

	if (CustomMenu::commandIndex != command_window->GetIndex()) {
		CustomMenu::commandIndex = command_window->GetIndex();
		for (auto w : custom_windows) {
			w->Refresh();
		}
	}

	if (command_window->GetActive()) {
		UpdateCommand();
	}
	else if (menustatus_window->GetActive()) {
		UpdateActorSelection();
	}

	if (CustomMenu::showMap) {
		Main_Data::game_screen->Update();
		Main_Data::game_pictures->Update(false);
	}
}

void Scene_Menu_Custom::CreateCommandWindow() {
	// Create Options Window
	std::vector<std::string> options;

	if (!CustomMenu::customCommands) {
		if (Player::IsRPG2k()) {
			command_options.push_back(Item);
			command_options.push_back(Skill);
			command_options.push_back(Equipment);
			command_options.push_back(Save);
			if (Player::player_config.settings_in_menu.Get()) {
				command_options.push_back(Settings);
			}
			if (Player::debug_flag) {
				command_options.push_back(Debug);
			}
			command_options.push_back(Quit);
		}
		else {
			for (std::vector<int16_t>::iterator it = lcf::Data::system.menu_commands.begin();
				it != lcf::Data::system.menu_commands.end(); ++it) {
				switch (*it) {
				case Row:
					if (Feature::HasRow()) {
						command_options.push_back((CommandOptionType)*it);
					}
					break;
				case Wait:
					if (Feature::HasRpg2k3BattleSystem()) {
						command_options.push_back((CommandOptionType)*it);
					}
					break;
				default:
					command_options.push_back((CommandOptionType)*it);
					break;
				}
			}
			if (Player::player_config.settings_in_menu.Get()) {
				command_options.push_back(Settings);
			}
			if (Player::debug_flag) {
				command_options.push_back(Debug);
			}
			command_options.push_back(Quit);
		}
	}
	else {
		for (std::vector<int16_t>::iterator it = lcf::Data::system.menu_commands.begin();
			it != lcf::Data::system.menu_commands.end(); ++it) {

			command_options.push_back((CommandOptionType)*it);
		}
		if (command_options.size() == 0)
			command_options.push_back(CommandOptionType::Item);
	}

	// Add all menu items
	int i = 0;
	std::vector<CommandOptionType>::iterator it;
	for (it = command_options.begin(); it != command_options.end(); ++it) {
		switch(*it) {
		case Item:
			options.push_back(ToString(lcf::Data::terms.command_item));
			break;
		case Skill:
			options.push_back(ToString(lcf::Data::terms.command_skill));
			break;
		case Equipment:
			options.push_back(ToString(lcf::Data::terms.menu_equipment));
			break;
		case Save:
			options.push_back(ToString(lcf::Data::terms.menu_save));
			break;
		case Status:
			options.push_back(ToString(lcf::Data::terms.status));
			break;
		case Row:
			options.push_back(ToString(lcf::Data::terms.row));
			break;
		case Order:
			options.push_back(ToString(lcf::Data::terms.order));
			break;
		case Wait:
			options.push_back(ToString(Main_Data::game_system->GetAtbMode() == lcf::rpg::SaveSystem::AtbMode_atb_wait ? lcf::Data::terms.wait_on : lcf::Data::terms.wait_off));
			break;
		case Settings:
			options.push_back("Settings");
			break;
		case Debug:
			options.push_back("Debug");
			break;
		default:
			int d = *it;
			if (d > 0) {
				options.push_back(ToString(lcf::Data::terms.menu_quit));
			}
			else {
				options.push_back(CustomMenu::commands[i]);
			}
			break;
		}
		i++;
	}
	std::string win_name = "Commands";
	auto it2 = CustomMenu::customWindows.find(win_name);

	if (it2 != CustomMenu::customWindows.end()) {
		command_window.reset(new Window_Command_Custom(options, CustomMenu::customWindows[win_name].w, CustomMenu::customWindows[win_name].column));
		command_window->SetIndex(menu_index);

		command_window->SetX(CustomMenu::customWindows[win_name].x);
		command_window->SetY(CustomMenu::customWindows[win_name].y);
		command_window->SetHeight(CustomMenu::customWindows[win_name].h);

		command_window->SetVisible(!CustomMenu::customWindows[win_name].hide);
	}
	else {
		command_window.reset(new Window_Command(options, menu_command_width));
		command_window->SetX(Player::menu_offset_x);
		command_window->SetY(Player::menu_offset_y);
		command_window->SetIndex(menu_index);
	}

	// Disable items
	for (it = command_options.begin(); it != command_options.end(); ++it) {
		switch(*it) {
		case Save:
			// If save is forbidden disable this item
			if (!Main_Data::game_system->GetAllowSave()) {
				command_window->DisableItem(it - command_options.begin());
			}
		case Wait:
		case Quit:
		case Settings:
		case Debug:
			break;
		case Order:
			if (Main_Data::game_party->GetActors().size() <= 1) {
				command_window->DisableItem(it - command_options.begin());
			}
			break;
		default:
			if (Main_Data::game_party->GetActors().empty()) {
				command_window->DisableItem(it - command_options.begin());
			}
			break;
		}
	}

}

void Scene_Menu_Custom::UpdateCommand() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
		Scene::Pop();
	} else if (Input::IsTriggered(Input::DECISION)) {
		menu_index = command_window->GetIndex();
		if (menu_index >= 0 && menu_index < command_options.size())
			if (command_options[menu_index] >= 0) {
				switch (command_options[menu_index]) {
				case Item:
					if (Main_Data::game_party->GetActors().empty()) {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
					}
					else {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
						Scene::Push(std::make_shared<Scene_Item_Custom>());
					}
					break;
				case Skill:
				case Equipment:
				case Status:
				case Row:
					if (Main_Data::game_party->GetActors().empty()) {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
					}
					else {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
						command_window->SetActive(false);
						menustatus_window->SetActive(true);
						menustatus_window->SetIndex(0);
					}
					break;
				case Save:
					if (!Main_Data::game_system->GetAllowSave()) {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
					}
					else {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
						Scene::Push(std::make_shared<Scene_Save>());
					}
					break;
				case Order:
					if (Main_Data::game_party->GetActors().size() <= 1) {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
					}
					else {
						Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
						Scene::Push(std::make_shared<Scene_Order>());
					}
					break;
				case Wait:
					Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
					Main_Data::game_system->ToggleAtbMode();
					command_window->SetItemText(menu_index,
						Main_Data::game_system->GetAtbMode() == lcf::rpg::SaveSystem::AtbMode_atb_wait ? lcf::Data::terms.wait_on : lcf::Data::terms.wait_off);
					break;
				case Settings:
					Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Game_System::SFX_Decision));
					Scene::Push(std::make_shared<Scene_Settings>());
					break;
				case Debug:
					Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
					Scene::Push(std::make_shared<Scene_Debug>());
					break;
				case Quit:
					Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
					Scene::Push(std::make_shared<Scene_End>());
					break;
				}
			}
			else {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
				Main_Data::game_switches->Set(-command_options[menu_index], true);
				Game_Map::SetNeedRefresh(true);
				Scene::Pop();
			}
	}
}

void Scene_Menu_Custom::UpdateActorSelection() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
		command_window->SetActive(true);
		menustatus_window->SetActive(false);
		menustatus_window->SetIndex(-1);
	} else if (Input::IsTriggered(Input::DECISION) && command_window->GetIndex() < command_options.size() && command_window->GetIndex() >= 0 && menustatus_window->GetIndex() >= 0) {
		switch (command_options[command_window->GetIndex()]) {
		case Skill:
			if (!menustatus_window->GetActor()->CanAct()) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
				return;
			}
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			Scene::Push(std::make_shared<Scene_Skill>(menustatus_window->GetIndex()));
			break;
		case Equipment:
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			Scene::Push(std::make_shared<Scene_Equip>(*menustatus_window->GetActor()));
			break;
		case Status:
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			Scene::Push(std::make_shared<Scene_Status>(menustatus_window->GetIndex()));
			break;
		case Row:
		{
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			// Don't allow entire party in the back row.
			const auto& actors = Main_Data::game_party->GetActors();
			int num_in_back = 0;
			for (auto* actor: actors) {
				if (actor->GetBattleRow() == Game_Actor::RowType::RowType_back) {
					++num_in_back;
				}
			}
			Game_Actor* actor = actors[menustatus_window->GetIndex()];
			if (actor->GetBattleRow() == Game_Actor::RowType::RowType_front) {
				if (num_in_back < int(actors.size() - 1)) {
					actor->SetBattleRow(Game_Actor::RowType::RowType_back);
				}
			} else {
				actor->SetBattleRow(Game_Actor::RowType::RowType_front);
			}
			menustatus_window->Refresh();
			break;
		}
		default:
			assert(false);
			break;
		}

		command_window->SetActive(true);
		menustatus_window->SetActive(false);
		menustatus_window->SetIndex(-1);
	}
}
