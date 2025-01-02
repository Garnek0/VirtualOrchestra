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
#include <vo/list.h>

#include <SDL2/SDL.h>

static bool quitSignaled = false;

static struct list* keyboardHandlerList;
static struct list* mouseWheelHandlerList;
static struct list* mouseHandlerList;

int event_init() {
	keyboardHandlerList = list_create();
	mouseWheelHandlerList = list_create();
	mouseHandlerList = list_create();

	return 0;
}

void event_iteration() {
	struct keyboard_handler* keyboardHandler;
	struct mouse_wheel_handler* mouseWheelHandler;
	struct mouse_handler* mouseHandler;

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quitSignaled = true;
				break;
			// Check for mouse wheel events
			case SDL_MOUSEWHEEL:
				if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
					event.wheel.x = -event.wheel.x;
					event.wheel.y = -event.wheel.y;
					event.wheel.preciseX = -event.wheel.preciseX;
					event.wheel.preciseY = -event.wheel.preciseY;
				}

				list_foreach(node, mouseWheelHandlerList) {
					mouseWheelHandler = (struct mouse_wheel_handler*)node->data;

					mouseWheelHandler->handler(event.wheel.x, event.wheel.y, event.wheel.preciseX, event.wheel.preciseY);
				}	
				break;
			default:
				break;
		}
	}

	// Check for keyboard events

	const Uint8* kbState = SDL_GetKeyboardState(NULL);
	SDL_Keymod kmState = SDL_GetModState();

	list_foreach(node, keyboardHandlerList) {
		keyboardHandler = (struct keyboard_handler*)node->data;

		if (kbState[SDL_GetScancodeFromKey(keyboardHandler->key)] && ((kmState & keyboardHandler->mod) == keyboardHandler->mod))
			keyboardHandler->handler();
	}

	// Check for mouse events
	
	int mouseX, mouseY;
	Uint32 mState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

	list_foreach(node, mouseHandlerList) {
		mouseHandler = (struct mouse_handler*)node->data;

		if ((mState & mouseHandler->button) == mouseHandler->button)
			mouseHandler->handler(mouseX, mouseY);
	}
}

struct keyboard_handler* event_add_keyboard_handler(SDL_Keycode keycode, SDL_Keymod mod, void (*handler)(void)) {
	struct keyboard_handler* kh = malloc(sizeof(struct keyboard_handler));
	kh->key = keycode;
	kh->mod = mod;
	kh->handler = handler;

	list_insert(keyboardHandlerList, (void*)kh);

	return kh;
}

void event_remove_keyboard_handler(struct keyboard_handler* kh) {
	list_remove(keyboardHandlerList, (void*)kh);

	free((void*)kh);
}

struct mouse_wheel_handler* event_add_mouse_wheel_handler(void (*handler)(int, int, float, float)) {
	struct mouse_wheel_handler* mwh = malloc(sizeof(struct mouse_wheel_handler));
	mwh->handler = handler;

	list_insert(mouseWheelHandlerList, (void*)mwh);

	return mwh;
}

void event_remove_mouse_wheel_handler(struct mouse_wheel_handler* mwh) {
	list_remove(mouseWheelHandlerList, (void*)mwh);

	free((void*)mwh);
}

struct mouse_handler* event_add_mouse_handler(Uint32 button, void (*handler)(int, int)) {
	struct mouse_handler* mh = malloc(sizeof(struct mouse_handler));
	mh->button = button;
	mh->handler = handler;

	list_insert(mouseHandlerList, (void*)mh);

	return mh;
}

void event_remove_mouse_handler(struct mouse_handler* mh) {
	list_remove(mouseHandlerList, (void*)mh);

	free((void*)mh);
}

bool event_has_signaled_quit() {
	return quitSignaled;
}

void event_get_mouse_position(int* x, int* y) {
	SDL_GetMouseState(x, y);
}
