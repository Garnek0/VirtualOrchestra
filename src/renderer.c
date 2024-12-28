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

#include <vo/debug.h>
#include <vo/ver.h>
#include <vo/renderer.h>
#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;

int renderer_init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		debug_log(LOGLEVEL_FATAL, "Renderer: SDL video subsystem init failed: %s\n", SDL_GetError());
		return -1;
	}

	char* windowTitle = malloc(50);
	sprintf(windowTitle, "Virtual Orchestra %d.%d.%d", VO_VER_MAJOR, VO_VER_MINOR, VO_VER_PATCH);

	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

	if (!window) {
		debug_log(LOGLEVEL_FATAL, "Renderer: Failed to create SDL window: %s\n", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer) {
		debug_log(LOGLEVEL_FATAL, "Renderer: Failed to create SDL renderer: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

void renderer_fini() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void renderer_iteration() {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

