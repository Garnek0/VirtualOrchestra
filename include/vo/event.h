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
#include <SDL_keycode.h>

struct keyboard_callback {
	SDL_Keycode key;
	SDL_Keymod mod;
	void (*callback)(void);
	bool alreadyCalled; // Needed to avoid extra calls if the key is not lifted quickly enough
};

struct mouse_wheel_callback {
	void (*callback)(int x, int y, float preciseX, float preciseY);
};

struct mouse_callback {
	Uint32 button;
	void (*callback)(int relX, int relY);
};

int event_init();
void event_iteration();

struct keyboard_callback* event_register_keyboard_callback(SDL_Keycode keycode, SDL_Keymod mod, void (*callback)(void));
void event_remove_keyboard_callback(struct keyboard_callback* keyboardCallback);

struct mouse_wheel_callback* event_register_mouse_wheel_callback(void (*callback)(int, int, float, float));
void event_remove_mouse_wheel_callback(struct mouse_wheel_callback* mouseWheelCallback);

struct mouse_callback* event_register_mouse_callback(Uint32 button, void (*callback)(int, int));
void event_remove_mouse_callback(struct mouse_callback* mouseCallback);

bool event_has_signaled_quit();
void event_get_mouse_position(int* x, int* y);
