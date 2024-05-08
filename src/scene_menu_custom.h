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

#ifndef EP_SCENE_MENU_CUSTOM_H
#define EP_SCENE_MENU_CUSTOM_H

// Headers
#include "scene.h"
#include "window_command.h"
#include "window_gold.h"
#include "window_menustatus_custom.h"
#include "window_custom.h"

#include "spriteset_map.h"
#include "game_screen.h"
#include "game_pictures.h"


struct MenuCustomWindowPage {
public:
	int align;
	int condition;
	std::string text;
};

struct MenuCustomStatus {
public:
	int x;
	int y;
	bool unique;
	std::string text;
	int align;
};

struct MenuCustomWindow {
public:
	int x;
	int y;
	int w;
	int h;
	bool hide;
	int column = 1;
	int opacity;
	std::string name;
	std::vector<MenuCustomWindowPage> pages;

	std::vector<MenuCustomStatus> stats;

	int itemHeight = 58;

	int eventID = 0;
	int actorIndex = 0;
	int varID = 0;
	int stringVarID = 0;
};

namespace CustomMenu {
	extern std::map<std::string, MenuCustomWindow> customWindows;

	extern bool showMap;

	extern std::string backName;

	extern int commandIndex;

	extern int parallax_x;
	extern int parallax_y;

	extern bool used;

	extern std::string commands[16];

	extern bool customCommands;

}
/**
 * Scene Menu class.
 */
class Scene_Menu_Custom : public Scene {
public:
	/**
	 * Constructor.
	 *
	 * @param menu_index selected index in the menu.
	 */
	Scene_Menu_Custom(int menu_index = 0);

	void Start() override;
	void Continue(SceneType prev_scene) override;
	void CreateCustomWindows();
	void vUpdate() override;

	/**
	 * Creates the window displaying the options.
	 */
	void CreateCommandWindow();

	/**
	 * Update function if command window is active.
	 */
	void UpdateCommand();

	/**
	 * Update function if status window is active.
	 */
	void UpdateActorSelection();

	/**
	 * Creates the background graphic of the scene.
	 */
	void CreateBackGraphic();

	/** Options available in a Rpg2k3 menu. */
	enum CommandOptionType {
		Item = 1,
		Skill,
		Equipment,
		Save,
		Status,
		Row,
		Order,
		Wait,
		Quit,
		// EasyRPG extra
		Debug = 100,
		Settings = 101,
	};

private:
	void OnBackSpriteReady(FileRequestResult* result);

	/** Selected index on startup. */
	int menu_index;

	int wait_parallax = 0;

	/** Window displaying the commands. */
	std::unique_ptr<Window_Command> command_window;

	/** Window displaying the gold amount. */
	std::unique_ptr<Window_Gold> gold_window;

	/** Window displaying the heros and their status. */
	std::unique_ptr<Window_MenuStatus_Custom> menustatus_window;

	std::vector<Window_Custom*> custom_windows;

	/** Options available in the menu. */
	std::vector<CommandOptionType> command_options;

	std::unique_ptr<Spriteset_Map> spriteset;

	/** Background graphic. */
	std::unique_ptr<Plane> backSprite;


	FileRequestBinding request_id;

	void OnSystem2Ready(FileRequestResult* result);
};

#endif
