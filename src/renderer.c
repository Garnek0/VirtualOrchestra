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
#include <SDL2/SDL_image.h>

// Renderer data struct for instruments
struct renderer_instr_data {
	SDL_Texture* loadedGraphic;	
};

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

	// NOTE: Meh, this good enough for testing i guess. PNG may not be the best option 
	// for what i'm trying to do... A vector-based format would be way better.
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		debug_log(LOGLEVEL_FATAL, "Renderer: SDL_image init failed: %s\n", IMG_GetError());
		return -1;
	}

	return 0;
}

int renderer_init_instrument(struct instrument* instr) {	
	struct renderer_instr_data* rendererData = (struct renderer_instr_data*)malloc(sizeof(struct renderer_instr_data));
	memset((void*)rendererData, 0, sizeof(struct renderer_instr_data));

	SDL_Surface* graphicSurface = IMG_Load(instr->graphic);
	if (!graphicSurface) {
		debug_log(LOGLEVEL_ERROR, "Renderer: Instrument graphic file \"%s\" for instrument with ID=%d could not be loaded: %s\n", instr->graphic, instr->id, IMG_GetError());
		return -1;
	}

	rendererData->loadedGraphic = SDL_CreateTextureFromSurface(renderer, graphicSurface);

	SDL_FreeSurface(graphicSurface);

	if (!rendererData->loadedGraphic) {
		debug_log(LOGLEVEL_ERROR, "Renderer: Could not create texture from surface for instrument with ID=%d: %s\n", instr->id, SDL_GetError());	
		return -1;
	}

	instr->rendererData = rendererData;

	return 0;
}

void renderer_fini_instrument(struct instrument* instr) {
	struct renderer_instr_data* rendererData = (struct renderer_instr_data*)instr->rendererData;

	SDL_DestroyTexture(rendererData->loadedGraphic);
	free((void*)rendererData);

	instr->rendererData = NULL;
}

void renderer_render_instrument(struct instrument* instr, int screenX, int screenY) {
	struct renderer_instr_data* rendererData = (struct renderer_instr_data*)instr->rendererData;

	SDL_Rect rect;

	rect.x = screenX;
	rect.y = screenY;

	SDL_QueryTexture(rendererData->loadedGraphic, NULL, NULL, &rect.w, &rect.h);		

	SDL_RenderCopy(renderer, rendererData->loadedGraphic, NULL, &rect);
}

void renderer_fini() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void renderer_iteration() {
	instrument_render_all();

	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
}

