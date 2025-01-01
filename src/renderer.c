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
#include <vo/event.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Renderer data struct for instruments
struct renderer_instr_data {
	SDL_Texture* loadedGraphic;	
};

static SDL_Window* window;
static SDL_Renderer* renderer;

static int cameraX = 0;
static int cameraY = 0;

int renderer_init() {
	char* windowTitle = malloc(50);
	sprintf(windowTitle, "Virtual Orchestra %d.%d.%d", VO_VER_MAJOR, VO_VER_MINOR, VO_VER_PATCH);

	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);

	if (!window) {
		debug_log(LOGLEVEL_FATAL, "Renderer: Failed to create SDL window: %s\n", SDL_GetError());
		return -1;
	}

	int windowDisplayIndex = SDL_GetWindowDisplayIndex(window);

	if (windowDisplayIndex < 0) {
		debug_log(LOGLEVEL_FATAL, "Renderer: Failed to get window display index: %s\n", SDL_GetError());
		return -1;
	}

	SDL_DisplayMode currentMode;

	if (SDL_GetDesktopDisplayMode(windowDisplayIndex, &currentMode) != 0) {
		debug_log(LOGLEVEL_FATAL, "Renderer: Failed to get current desktop display mode: %s\n", SDL_GetError());
		return -1;
	}

	SDL_SetWindowSize(window, currentMode.w, currentMode.h);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer) {
		debug_log(LOGLEVEL_FATAL, "Renderer: Failed to create SDL renderer: %s\n", SDL_GetError());
		return -1;
	}

	// Center the camera to point at (or roughly at) 0, 0

	int rendererOutputWidth, rendererOutputHeight;

	if (SDL_GetRendererOutputSize(renderer, &rendererOutputWidth, &rendererOutputHeight) != 0) {
		debug_log(LOGLEVEL_FATAL, "Renderer: Failed to get renderer output size: %s\n", SDL_GetError());
		return -1;
	}

	cameraX = -(rendererOutputWidth/2);
	cameraY = -(rendererOutputHeight/2);

	// Register the camera keyboard handlers
	
	event_add_keyboard_handler(SDLK_UP, KMOD_NONE, renderer_camera_handler_up);
	event_add_keyboard_handler(SDLK_DOWN, KMOD_NONE, renderer_camera_handler_down);
	event_add_keyboard_handler(SDLK_RIGHT, KMOD_NONE, renderer_camera_handler_right);
	event_add_keyboard_handler(SDLK_LEFT, KMOD_NONE, renderer_camera_handler_left);

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

void renderer_render_instrument(struct instrument* instr) {
	struct renderer_instr_data* rendererData = (struct renderer_instr_data*)instr->rendererData;

	SDL_Rect rect;

	rect.x = instr->x - cameraX;
	rect.y = instr->y - cameraY;

	SDL_QueryTexture(rendererData->loadedGraphic, NULL, NULL, &rect.w, &rect.h);

	SDL_RenderCopy(renderer, rendererData->loadedGraphic, NULL, &rect);
}

void renderer_fini() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void renderer_iteration() {
	struct list* instrumentList = instrument_get_list();

	list_foreach(node, instrumentList) {
		renderer_render_instrument((struct instrument*)node->data);
	}

	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
}

void renderer_camera_get_position(int* x, int* y) {
	*x = cameraX;
	*y = cameraY;
}

void renderer_camera_set_position(int x, int y) {
	cameraX = x;
	cameraY = y;
}

void renderer_camera_handler_up() {
	cameraY -= 10;
}

void renderer_camera_handler_down() {
	cameraY += 10;
}

void renderer_camera_handler_right() {
	cameraX += 10;
}

void renderer_camera_handler_left() {
	cameraX -= 10;
}
