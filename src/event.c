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
#include <vo/gfxui/renderer.h>
#include <vo/list.h>

#include <SDL2/SDL.h>

static bool quitSignaled = false;

static struct list* keyboardCallbackList;
static struct list* mouseWheelCallbackList;
static struct list* mouseCallbackList;

int event_init() {
	keyboardCallbackList = list_create();
	mouseWheelCallbackList = list_create();
	mouseCallbackList = list_create();

	return 0;
}

void event_iteration() {
	struct keyboard_callback* keyboardCallback;
	struct mouse_wheel_callback* mouseWheelCallback;
	struct mouse_callback* mouseCallback;

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quitSignaled = true;
				break;
			// Check for mouse wheel events. If there is a mouse wheel
			// event in the event queue, call every registered mouse 
			// wheel event callback.
			case SDL_MOUSEWHEEL:
				if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
					event.wheel.x = -event.wheel.x;
					event.wheel.y = -event.wheel.y;
					event.wheel.preciseX = -event.wheel.preciseX;
					event.wheel.preciseY = -event.wheel.preciseY;
				}

				list_foreach(node, mouseWheelCallbackList) {
					mouseWheelCallback = (struct mouse_wheel_callback*)node->data;

					mouseWheelCallback->callback(event.wheel.x, event.wheel.y, event.wheel.preciseX, event.wheel.preciseY);
				}	
				break;
			default:
				break;
		}
	}

	// Check if any keyboard callbacks can be called and call them if thats
	// the case.

	const Uint8* kbState = SDL_GetKeyboardState(NULL);
	SDL_Keymod kmState = SDL_GetModState();

	list_foreach(node, keyboardCallbackList) {
		keyboardCallback = (struct keyboard_callback*)node->data;

		if (kbState[SDL_GetScancodeFromKey(keyboardCallback->key)] && ((kmState & keyboardCallback->mod) == keyboardCallback->mod))
			keyboardCallback->callback();
	}

	// Do the same thing for the mouse
	
	int mouseX, mouseY;
	Uint32 mState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

	list_foreach(node, mouseCallbackList) {
		mouseCallback = (struct mouse_callback*)node->data;

		if ((mState & mouseCallback->button) == mouseCallback->button)
			mouseCallback->callback(mouseX, mouseY);
	}
}

struct keyboard_callback* event_register_keyboard_callback(SDL_Keycode keycode, SDL_Keymod mod, void (*callback)(void)) {
	struct keyboard_callback* keyboardCallback = malloc(sizeof(struct keyboard_callback));
	keyboardCallback->key = keycode;
	keyboardCallback->mod = mod;
	keyboardCallback->callback = callback;

	list_insert(keyboardCallbackList, (void*)keyboardCallback);

	return keyboardCallback;
}

void event_remove_keyboard_callback(struct keyboard_callback* keyboardCallback) {
	list_remove(keyboardCallbackList, (void*)keyboardCallback);

	free((void*)keyboardCallback);
}

struct mouse_wheel_callback* event_register_mouse_wheel_callback(void (*callback)(int, int, float, float)) {
	struct mouse_wheel_callback* mouseWheelCallback = malloc(sizeof(struct mouse_wheel_callback));
	mouseWheelCallback->callback = callback;

	list_insert(mouseWheelCallbackList, (void*)mouseWheelCallback);

	return mouseWheelCallback;
}

void event_remove_mouse_wheel_callback(struct mouse_wheel_callback* mouseWheelCallback) {
	list_remove(mouseWheelCallbackList, (void*)mouseWheelCallback);

	free((void*)mouseWheelCallback);
}

struct mouse_callback* event_register_mouse_callback(Uint32 button, void (*callback)(int, int)) {
	struct mouse_callback* mouseCallback = malloc(sizeof(struct mouse_callback));
	mouseCallback->button = button;
	mouseCallback->callback = callback;

	list_insert(mouseCallbackList, (void*)mouseCallback);

	return mouseCallback;
}

void event_remove_mouse_callback(struct mouse_callback* mouseCallback) {
	list_remove(mouseCallbackList, (void*)mouseCallback);

	free((void*)mouseCallback);
}

bool event_has_signaled_quit() {
	return quitSignaled;
}

void event_get_mouse_position(int* x, int* y) {
	SDL_GetMouseState(x, y);
}
