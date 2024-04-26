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

#ifndef EP_WINDOW_MENUSTATUS_CUSTOM_H
#define EP_WINDOW_MENUSTATUS_CUSTOM_H

// Headers
#include "window_menustatus.h"
#include <regex>
#include "output.h"

/**
 * Window MenuStatus Class
 */
class Window_MenuStatus_Custom : public Window_MenuStatus {
public:
	Window_MenuStatus_Custom(int ix, int iy, int iwidth, int iheight);
	void Refresh();
	void UpdateCursorRect() override;

	std::string ParserText(std::string text, int index, int tx, int ty, Text::Alignment align);
	std::string replaceFunction(const std::smatch& match, int index, int tx, int ty, Text::Alignment align);

	Game_Actor* GetActor() const;

	bool customised = false;

protected:
	int text_offset = 0;
};

#endif
