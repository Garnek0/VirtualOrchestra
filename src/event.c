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

int event_init() {
	keyboardHandlerList = list_create();

	return 0;
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

	// Check for keyboard events

	const Uint8* kbState = SDL_GetKeyboardState(NULL);
	SDL_Keymod kmState = SDL_GetModState();

	struct keyboard_handler* kh;

	list_foreach(node, keyboardHandlerList) {
		kh = (struct keyboard_handler*)node->data;

		if (kbState[SDL_GetScancodeFromKey(kh->key)] && ((kmState & kh->mod) == kh->mod))
			kh->handler();
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

bool event_has_signaled_quit() {
	return quitSignaled;
}
