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

static SDL_Window* window;
static SDL_Renderer* renderer;

static float zoomScale = 1;

// Stage 0, 0 - Screen 0, 0 offset. The stage is the
// "world" where all the instruments are rendered,
// whereas the screen is... well... the screen.
// We use these values to determine what part of
// the stage we need to render.
static float screenOffsetY;
static float screenOffsetX;

// Convert screen coordinates to stage coordinates. 
void renderer_coord_screen_to_stage(int screenX, int screenY, float* stageX, float* stageY) {
	*stageX = (float)(screenX) / zoomScale + screenOffsetX;
	*stageY = (float)(screenY) / zoomScale + screenOffsetY;
}

// ... The other way around
void renderer_coord_stage_to_screen(float stageX, float stageY, int* screenX, int* screenY) {
	*screenX = (int)((stageX - screenOffsetX) * zoomScale);
	*screenY = (int)((stageY - screenOffsetY) * zoomScale);
}

int renderer_init() {
	char* windowTitle = malloc(50);

#ifndef VO_VER_SNAPSHOT
	sprintf(windowTitle, "Virtual Orchestra %d.%d.%d-%s", VO_VER_MAJOR, VO_VER_MINOR, VO_VER_PATCH, VO_VER_STAGE);
#else
	sprintf(windowTitle, "Virtual Orchestra %s (snapshot)", VO_VER_STAGE);
#endif

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

	// Get the default screen offset values

	int rendererOutputWidth, rendererOutputHeight;

	if (SDL_GetRendererOutputSize(renderer, &rendererOutputWidth, &rendererOutputHeight) != 0) {
		debug_log(LOGLEVEL_FATAL, "Renderer: Failed to get renderer output size: %s\n", SDL_GetError());
		return -1;
	}

	renderer_set_screen_offset(-(rendererOutputWidth/2.0) / zoomScale, -(rendererOutputHeight/2.0) / zoomScale);

	// Register callbacks
	
	event_register_keyboard_callback(SDLK_UP, KMOD_NONE, renderer_keyboard_pan_up);
	event_register_keyboard_callback(SDLK_DOWN, KMOD_NONE, renderer_keyboard_pan_down);
	event_register_keyboard_callback(SDLK_RIGHT, KMOD_NONE, renderer_keyboard_pan_right);
	event_register_keyboard_callback(SDLK_LEFT, KMOD_NONE, renderer_keyboard_pan_left);
	event_register_mouse_wheel_callback(renderer_mouse_wheel_zoom);
	event_register_mouse_callback(SDL_BUTTON_MMASK, renderer_mouse_pan);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		debug_log(LOGLEVEL_FATAL, "Renderer: SDL_image init failed: %s\n", IMG_GetError());
		return -1;
	}

	return 0;
}

void renderer_free_instrument_textures(struct instrument* instr) {
	for (int i = 0; i < instr->textureCount; i++)
		SDL_DestroyTexture(instr->textures[i].loadedTexture);

	free((void*)instr->textures);
}

int renderer_load_instrument_texture(struct instrument* instr, const char* path, int offsetX, int offsetY) {
	SDL_Surface* textureSurface = IMG_Load(path);
	if (!textureSurface) {
		debug_log(LOGLEVEL_ERROR, "Renderer: Texture file \"%s\" for instrument with ID=%d could not be loaded: %s\n", path, instr->id, IMG_GetError());
		return -1;
	}

	SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(renderer, textureSurface);

	if (!loadedTexture) {
		debug_log(LOGLEVEL_ERROR, "Renderer: Could not create texture from surface for instrument with ID=%d: %s\n", instr->id, SDL_GetError());
		SDL_FreeSurface(textureSurface);
		return -1;
	}

	SDL_FreeSurface(textureSurface);

	if (instr->maxTexturesBeforeRealloc == 0) {
		instr->maxTexturesBeforeRealloc = 16;
		instr->textureCount = 0;

		instr->textures = (struct texture*)calloc(16, sizeof(struct texture));
		memset((void*)instr->textures, 0, sizeof(struct texture)*16);
	} else if (instr->maxTexturesBeforeRealloc == instr->textureCount) {

		// Reallocate and double the size of the textures and textureDraw arrays. 

		instr->maxTexturesBeforeRealloc *= 2;

		struct texture* newTexturesArray = (struct texture*)calloc(instr->maxTexturesBeforeRealloc, sizeof(struct texture));

		memset((void*)newTexturesArray, 0, sizeof(struct texture)*instr->maxTexturesBeforeRealloc);

		memcpy((void*)newTexturesArray, (void*)instr->textures, sizeof(struct texture)*(instr->maxTexturesBeforeRealloc/2));

		free((void*)instr->textures);

		instr->textures = newTexturesArray;
	}

	instr->textures[instr->textureCount].loadedTexture = loadedTexture;
	instr->textures[instr->textureCount].textureDraw = true;
	instr->textures[instr->textureCount].textureOffsetX = offsetX;
	instr->textures[instr->textureCount].textureOffsetY = offsetY;

	instr->textureCount++;

	return instr->textureCount-1;
}

// This function should be used for setting the texture draw toggles (as opposed to
// indexing the textureDraw array in the instrument's code). This is mostly for
// safety reasons but also to avoid redundant code.
void renderer_set_instrument_texture_draw(struct instrument* instr, int textureIndex, bool doDraw) {	
	if ((textureIndex >= instr->textureCount) || textureIndex < 0) {
		debug_log(LOGLEVEL_WARN, "Renderer: Attempt to modify out-of-bounds texture draw toggle for instrument with ID=%d.\n", instr->id);
		return;
	}

	instr->textures[textureIndex].textureDraw = doDraw;
}

void renderer_set_instrument_texture_offset(struct instrument* instr, int textureIndex, int offsetX, int offsetY) {
	if ((textureIndex >= instr->textureCount) || textureIndex < 0) {
		debug_log(LOGLEVEL_WARN, "Renderer: Attempt to modify out-of-bounds texture offset for instrument with ID=%d.\n", instr->id);
		return;
	}
	
	instr->textures[textureIndex].textureOffsetX = offsetX;
	instr->textures[textureIndex].textureOffsetY = offsetY;
}

void renderer_render_instrument(struct instrument* instr) {
	SDL_Rect rect;

	for (int i = 0; i < instr->textureCount; i++) {
		if (!instr->textures[i].textureDraw)
			continue;

		renderer_coord_stage_to_screen(instr->x + instr->textures[i].textureOffsetX, instr->y + instr->textures[i].textureOffsetY, &rect.x, &rect.y);

		SDL_QueryTexture(instr->textures[i].loadedTexture, NULL, NULL, &rect.w, &rect.h);

		rect.w *= zoomScale;
		rect.h *= zoomScale;

		SDL_RenderCopy(renderer, instr->textures[i].loadedTexture, NULL, &rect);
	}
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

void renderer_get_screen_offset(float* x, float* y) {
	*x = screenOffsetX;
	*y = screenOffsetY;
}

void renderer_set_screen_offset(float x, float y) {
	screenOffsetX = x;
	screenOffsetY = y;
}

void renderer_keyboard_pan_up() {
	screenOffsetY -= 10 / zoomScale;
}

void renderer_keyboard_pan_down() {
	screenOffsetY += 10 / zoomScale;
}

void renderer_keyboard_pan_right() {
	screenOffsetX += 10 / zoomScale;
}

void renderer_keyboard_pan_left() {
	screenOffsetX -= 10 / zoomScale;
}

void renderer_mouse_wheel_zoom(int x, int y, float preciseX, float preciseY) {
	int mouseX, mouseY;
	float mouseStageX1, mouseStageY1, mouseStageX2, mouseStageY2;

	event_get_mouse_position(&mouseX, &mouseY);

	renderer_coord_screen_to_stage(mouseX, mouseY, &mouseStageX1, &mouseStageY1);

	if (preciseY > 0) {
		zoomScale *= 0.9;
	} else if (preciseY < 0) {
		zoomScale *= 1.1;
	}

	if (zoomScale > 5) 
		zoomScale = 5;
	else if (zoomScale < 0.8)
		zoomScale = 0.8;

	renderer_coord_screen_to_stage(mouseX, mouseY, &mouseStageX2, &mouseStageY2);

	screenOffsetX += (mouseStageX1 - mouseStageX2);
	screenOffsetY += (mouseStageY1 - mouseStageY2);
}

void renderer_mouse_pan(int relX, int relY) {
	screenOffsetY -= relY / zoomScale;
	screenOffsetX -= relX / zoomScale;
}
