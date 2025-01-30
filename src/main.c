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

#include <vo/ver.h>
#include <vo/debug.h>
#include <vo/gfxui/renderer.h>
#include <vo/event.h>
#include <vo/note.h>

#include <vo/instruments/instrument.h>
#include <vo/instruments/piano.h>

#include <stdio.h>
#include <SDL2/SDL.h>

void test_chord_callback() {
	struct list* instrumentList = instrument_get_list();

	list_foreach(node, instrumentList) {
		struct instrument* instr = (struct instrument*)node->data;

		instr->play_note(instr, NOTE_C, 4);
		instr->play_note(instr, NOTE_Ds_Eb, 4);
		instr->play_note(instr, NOTE_G, 4);
	}
}

void test_chord_release_callback() {
	struct list* instrumentList = instrument_get_list();

	list_foreach(node, instrumentList) {
		struct instrument* instr = (struct instrument*)node->data;

		instr->release_note(instr, NOTE_C, 4);
		instr->release_note(instr, NOTE_Ds_Eb, 4);
		instr->release_note(instr, NOTE_G, 4);
	}
}

int main() {
	printf("Virtual Orchestra %d.%d.%d-%s by Garnek0 (Popa Vlad)\n", VO_VER_MAJOR, VO_VER_MINOR, VO_VER_PATCH, VO_VER_STAGE);
 
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		debug_log(LOGLEVEL_FATAL, "Main: SDL init failed: %s\n", SDL_GetError());
		return 1;
	}

	if (event_init() != 0) {
		debug_log(LOGLEVEL_FATAL, "Main: Events init failed!\n");
		return 1;
	}

	if (renderer_init() != 0) {
		debug_log(LOGLEVEL_FATAL, "Main: Renderer init failed!\n");
		return 1;
	}

	if (instrument_init() != 0) {
		debug_log(LOGLEVEL_FATAL, "Main: Instruments init failed!\n");
		return 1;
	}

	struct instrument_new_args args;
	args.x = args.y = 0;
	args.init = piano_init;
	args.fini = piano_fini;
	args.play_note = piano_play_note;
	args.release_note = piano_release_note;

	struct instrument* piano = instrument_new(args);

	(void)piano;

	event_register_keyboard_callback(SDLK_c, KMOD_NONE, test_chord_callback);
	event_register_keyboard_callback(SDLK_r, KMOD_NONE, test_chord_release_callback);

	unsigned int time1, time2;
	double deltatime;
	time2 = SDL_GetTicks();

	while(!event_has_signaled_quit()) {
		time1 = SDL_GetTicks();
		deltatime = time1 - time2;

		// Cap simulation at ~60 iterations/second
		if (deltatime > 1000/60.0) {
			time2 = time1;

			renderer_iteration();
			event_iteration();
		}
	}

	SDL_Quit();

	return 0;
}
