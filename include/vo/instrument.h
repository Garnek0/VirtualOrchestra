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

	// Max number of textures that can be loaded before the textures and
	// textureDraw arrays are reallocated and their size doubled. This
	// field only exists so that the renderer can keep track of when to
	// do said reallocations.
	int maxTexturesBeforeRealloc;
	
	int textureCount;
	SDL_Texture** textures;

	// This tells the renderer which textures in the textures array to draw.
	// For example, if textureDraw[3] = false, then textures[3] will not
	// be drawn. By default, after a texture is loaded, its corresponding
	// textureDraw field is set to true.
	bool* textureDraw;
	
	void* rendererData; // Renderer private data
};

struct instrument_new_args {
	float x, y;
	int (*init)(struct instrument* instr);
	int (*fini)(struct instrument* instr);
};

int instrument_init();

struct instrument* instrument_new(struct instrument_new_args args);
void instrument_set_position(struct instrument* instr, float x, float y);
void instrument_destroy(struct instrument* instr);

struct list* instrument_get_list();
