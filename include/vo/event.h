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

struct keyboard_handler {
	SDL_Keycode key;
	SDL_Keymod mod;
	void (*handler)(void);
};

int event_init();
void event_iteration();

struct keyboard_handler* event_add_keyboard_handler(SDL_Keycode keycode, SDL_Keymod mod, void (*handler)(void));
void event_remove_keyboard_handler(struct keyboard_handler* kh);

bool event_has_signaled_quit();
