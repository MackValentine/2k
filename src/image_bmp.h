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

#ifndef EP_IMAGE_BMP_H
#define EP_IMAGE_BMP_H

#include "system.h"
#include "filesystem.h"

#include <istream>

namespace ImageBMP {
	bool ReadBMP(const uint8_t* data, unsigned len, bool transparent, int& width, int& height, void*& pixels);
	bool ReadBMP(Filesystem::InputStream& stream, bool transparent, int& width, int& height, void*& pixels);
}

#endif
