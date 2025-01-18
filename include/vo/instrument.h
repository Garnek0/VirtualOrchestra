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

#include <stdbool.h>
#include <vo/list.h>
#include <SDL2/SDL.h>

struct instrument {
	int id; // Instrument ID

	float x, y; // Base coordinates
	
	int (*init)(struct instrument* instr); // Instrument init function
	int (*fini)(struct instrument* instr); // Instrument fini function
	int (*play_note)(struct instrument* instr, int note, int octave); // Start playing note
	int (*release_note)(struct instrument* instr, int note, int octave); // Stop playing note

	// Max number of textures that can be loaded before the texture
	// array is reallocated as double the size.
	int maxTexturesBeforeRealloc;

	int textureCount;
	struct renderer_instrument_texture* textures;
};

struct instrument_new_args {
	float x, y;
	int (*init)(struct instrument* instr);
	int (*fini)(struct instrument* instr);
	int (*play_note)(struct instrument* instr, int note, int octave);
	int (*release_note)(struct instrument* instr, int note, int octave);
};

int instrument_init();

struct instrument* instrument_new(struct instrument_new_args args);
void instrument_set_position(struct instrument* instr, float x, float y);
void instrument_destroy(struct instrument* instr);

struct list* instrument_get_list();
