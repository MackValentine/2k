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
#include "scene_menu.h"
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
#include <game_map.h>
#include "game_switches.h"


constexpr int menu_command_width = 88;
constexpr int gold_window_width = 88;
constexpr int gold_window_height = 32;

namespace SceneMenu {
	std::string menuLabels[MaxItems];
	int menuBehaviour[MaxItems];

	bool replaceCommands = false;

	bool showMap = true;
	bool noBackground = false;

	int sortItemType = -1;

	void Reset() {
		Replace(false);
		for (int i = 0; i < SceneMenu::MaxItems; i++) {
			SceneMenu::menuLabels[i] = " ";
			SceneMenu::menuBehaviour[i] = 0;
		}
	}

	void Replace(bool b) {
		replaceCommands = b;
	}

	std::vector<lcf::DBString> GetSaveData() {
		std::vector<lcf::DBString> v;

		// Add type menu
		std::string s = std::to_string(replaceCommands);
		v.push_back(lcf::DBString(s));

		int m = MaxItems;
		for (int i = 0; i < m ;i++) {
			std::string cmd = std::to_string(SceneMenu::menuBehaviour[i]);
			std::string s = SceneMenu::menuLabels[i] + "\n" + cmd;
			v.push_back(lcf::DBString(s));
		}
		return v;
	}

	void SetSaveData(std::vector<lcf::DBString> datas) {
		for (int i = 0; i <datas.size();i++) {
			if (i == 0) {
				bool b = false;
				auto s = datas[i];
				if (s == "1")
					b = true;
				Replace(b);
			}
			else {
				auto s = ToString(datas[i]);
				auto t = split(s, '\n');

				SceneMenu::menuLabels[i - 1] = t[0];
				SceneMenu::menuBehaviour[i - 1] = std::stoi(t[1]);
			}
		}
	}

	std::vector<std::string> split(const std::string& s, char seperator)
	{
		std::vector<std::string> output;
		std::string::size_type prev_pos = 0, pos = 0;
		while ((pos = s.find(seperator, pos)) != std::string::npos)
		{
			std::string substring(s.substr(prev_pos, pos - prev_pos));
			output.push_back(substring);
			prev_pos = ++pos;
		}

		output.push_back(s.substr(prev_pos, pos - prev_pos));
		return output;
	}
}

Scene_Menu::Scene_Menu(int menu_index) :
	menu_index(menu_index) {
	type = Scene::Menu;
	if (SceneMenu::showMap) {
		SetUseSharedDrawables(true);
	}
}

void Scene_Menu::Start() {
	CreateCommandWindow();

	// Gold Window
	gold_window.reset(new Window_Gold(Player::menu_offset_x, (Player::screen_height - gold_window_height - Player::menu_offset_y), gold_window_width, gold_window_height));

	// Status Window
	menustatus_window.reset(new Window_MenuStatus(Player::menu_offset_x + menu_command_width, Player::menu_offset_y, (MENU_WIDTH - menu_command_width), MENU_HEIGHT));
	menustatus_window->SetActive(false);


	if (SceneMenu::showMap) {
		spriteset.reset(new Spriteset_Map());

		MapUpdateAsyncContext t;
		Game_Map::UpdateCommonEvents(t);

		Main_Data::game_screen->Update();
		Main_Data::game_pictures->Update(false);
	}

	if (SceneMenu::noBackground) {
		gold_window->SetBackOpacity(0);
		menustatus_window->SetBackOpacity(0);
		command_window->SetBackOpacity(0);

		gold_window->SetFrameOpacity(0);
		menustatus_window->SetFrameOpacity(0);
		command_window->SetFrameOpacity(0);

	}

}


void Scene_Menu::Continue(SceneType /* prev_scene */) {
	menustatus_window->Refresh();
	gold_window->Refresh();
}

void Scene_Menu::vUpdate() {

	command_window->Update();
	gold_window->Update();
	menustatus_window->Update();

	if (command_window->GetActive()) {
		UpdateCommand();
	}
	else if (menustatus_window->GetActive()) {
		UpdateActorSelection();
	}

	if (SceneMenu::showMap) {

		MapUpdateAsyncContext actx;
		if (!actx.IsActive() || actx.IsParallelCommonEvent()) {
			Game_Map::UpdateCommonEvents(actx);
			UpdateGraphics();
		}

		Main_Data::game_screen->Update();
		Main_Data::game_pictures->Update(false);
	}
}

void Scene_Menu::CreateCommandWindow() {
	// Create Options Window
	std::vector<std::string> options;

	if (!SceneMenu::replaceCommands) {
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
	
		// Change command behaviour
		if (SceneMenu::menuLabels->size() > 0) {
			int m = SceneMenu::MaxItems;
			for (int i = 0; i < m; i++) {
				if (SceneMenu::menuBehaviour[i] != 0) {
					Scene_Menu::CommandOptionType type = Item;
					switch (SceneMenu::menuBehaviour[i]) {
					case -1:
						type = Item;
						break;
					case -2:
						type = Skill;
						break;
					case -3:
						type = Equipment;
						break;
					case -4:
						type = Save;
						break;
					case -5:
						type = Status;
						break;
					case -6:
						type = Row;
						break;
					case -7:
						type = Order;
						break;
					case -8:
						type = Wait;
						break;
					case -9:
						type = Quit;
						break;
					default:
						type = Item;
					}
					if (i< command_options.size())
						command_options[i] = type;
				}
			}
		}
	}
	else {
		if (SceneMenu::menuLabels->size() > 0) {
			int m = SceneMenu::MaxItems;
			for (int i = 0; i < m; i++) {
				if (SceneMenu::menuBehaviour[i] != 0) {
					Scene_Menu::CommandOptionType type = Item;
					switch (SceneMenu::menuBehaviour[i]) {
					case -1:
						type = Item;
						break;
					case -2:
						type = Skill;
						break;
					case -3:
						type = Equipment;
						break;
					case -4:
						type = Save;
						break;
					case -5:
						type = Status;
						break;
					case -6:
						type = Row;
						break;
					case -7:
						type = Order;
						break;
					case -8:
						type = Wait;
						break;
					case -9:
						type = Quit;
						break;
					default:
						if (SceneMenu::menuBehaviour[i] > 0) {
							type = CommonEvent;
						} else
							type = Item;
					}

					command_options.push_back(type);
				}
			}
		}
	}

	// Add all menu items
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
			options.push_back(ToString(lcf::Data::terms.menu_quit));
			break;
		}
	}


	if (SceneMenu::menuLabels->size() > 0) {
		int m = SceneMenu::MaxItems;
		for (int i = 0; i < m; i++) {
			
			if (SceneMenu::menuBehaviour[i] != 0 && i < options.size()) {
				auto s = SceneMenu::menuLabels[i];
				options[i] = s;
			}
		}
	}

	command_window.reset(new Window_Command(options, menu_command_width));
	command_window->SetX(Player::menu_offset_x);
	command_window->SetY(Player::menu_offset_y);
	command_window->SetIndex(menu_index);

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

void Scene_Menu::UpdateCommand() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
		Scene::Pop();
	} else if (Input::IsTriggered(Input::DECISION)) {
		menu_index = command_window->GetIndex();

		switch (command_options[menu_index]) {
		case Item:
			if (Main_Data::game_party->GetActors().empty()) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
			} else {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
				Scene::Push(std::make_shared<Scene_Item>());
			}
			break;
		case Skill:
		case Equipment:
		case Status:
		case Row:
			if (Main_Data::game_party->GetActors().empty()) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
			} else {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
				command_window->SetActive(false);
				menustatus_window->SetActive(true);
				menustatus_window->SetIndex(0);
			}
			break;
		case Save:
			if (!Main_Data::game_system->GetAllowSave()) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
			} else {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
				Scene::Push(std::make_shared<Scene_Save>());
			}
			break;
		case Order:
			if (Main_Data::game_party->GetActors().size() <= 1) {
				Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
			} else {
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
		case CommonEvent:
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));
			int i = command_window->GetIndex();
			Main_Data::game_switches->Set(SceneMenu::menuBehaviour[i], true);
			Scene::PopUntil(Scene::Map);
			Game_Map::SetNeedRefresh(true);
			//Output::Debug("Switch : {}", SceneMenu::menuBehaviour[i]);
			break;
		}
	}
}

void Scene_Menu::UpdateActorSelection() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
		command_window->SetActive(true);
		menustatus_window->SetActive(false);
		menustatus_window->SetIndex(-1);
	} else if (Input::IsTriggered(Input::DECISION)) {
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

int Scene_Menu::GetItemID() {
	if (Main_Data::game_party->GetActor(menustatus_window->GetIndex()))
		return Main_Data::game_party->GetActor(menustatus_window->GetIndex())->GetId();
	return 0;
}
