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

#include <vo/event.h>
#include <vo/renderer.h>

#include <SDL2/SDL.h>

static bool quitSignaled = false;
static const Uint8* kbState;

// TODO: Handler system. This event thing should provide some sort of routine
// for registering handlers for different types of events
// (e.g. Call function1() when the W key is pressed, or call
// function2() when the scroll wheel is moved, or maybe even
// call function3() AND function4() when H is pressed).

// NOTE: Temporary! This should be part of renderer.c!
static void __event_check_camera_movement() {
	// Check camera movement events via the keyboard

	int cameraX, cameraY;
	renderer_camera_get_position(&cameraX, &cameraY);

	if (kbState[SDL_SCANCODE_UP] && !kbState[SDL_SCANCODE_DOWN])
		cameraY -= 10;
	if (kbState[SDL_SCANCODE_DOWN] && !kbState[SDL_SCANCODE_UP])
		cameraY += 10;
	if (kbState[SDL_SCANCODE_RIGHT] && !kbState[SDL_SCANCODE_LEFT])
		cameraX += 10;
	if (kbState[SDL_SCANCODE_LEFT] && !kbState[SDL_SCANCODE_RIGHT])
		cameraX -= 10;

	renderer_camera_set_position(cameraX, cameraY);
} 

void event_iteration() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quitSignaled = true;
				break;
			default:
				break;
		}
	}

	kbState = SDL_GetKeyboardState(NULL);

	__event_check_camera_movement();
}

bool event_has_signaled_quit() {
	return quitSignaled;
}
