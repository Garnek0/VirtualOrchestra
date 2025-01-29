/*	
 *	SPDX-License-Identifier: GPL-3.0-only
 *
 *  Virtual Orchestra - Musical Instrument Simulation
 *  Copyright (C) 2024 Garnek0 (Popa Vlad) and Contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>

struct soundfont_instance {
	;
};

struct soundfont_riff_chunk {
	char chunkID[4];
	uint32_t chunkSize;
	uint8_t chunkData[0];
} __attribute__((packed));

struct soundfont_version_tag {
	uint16_t major;
	uint16_t minor;
};

struct soundfont_preset_header {
	char presetName[20];
	uint16_t preset;
	uint16_t bank;
	uint16_t presetBagIndex;
	uint32_t library;
	uint32_t genre;
	uint32_t morphology;
} __attribute__((packed));

struct soundfont_riff_chunk* soundfont_find_chunk(struct soundfont_riff_chunk* root, const char* fourcc, const char* firstFourBytes);
int soundfont_load(const char* path);
