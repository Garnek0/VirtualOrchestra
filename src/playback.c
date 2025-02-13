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

#include <vo/playback.h>
#include <vo/event.h>
#include <vo/list.h>
#include <vo/instruments/instrument.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int previousTime;
int playbackTime;
bool playing;

void playback_toggle_callback() {
	if (playing)
		playing = false;
	else
		playing = true;
}

void playback_stop_callback() {
	playing = false;
	playbackTime = 0;

	list_foreach(i, instrument_get_list()) {
		struct instrument* instr = (struct instrument*)i->data;

		list_foreach(j, instr->noteList) {
			struct complex_note* note = (struct complex_note*)j->data;

			if (note->playing) {
				instr->release_note(instr, *note);
				note->playing = false;
			}
		}
	}
}

void playback_reset() {
	playback_stop_callback();
}

void playback_iteration() {
	int now = SDL_GetPerformanceCounter();
	double deltaTime = (double)(now - previousTime) / SDL_GetPerformanceFrequency();
	previousTime = now;

	if (playing) {
		playbackTime += (int)(deltaTime * 1000);

		list_foreach(i, instrument_get_list()) {
			struct instrument* instr = (struct instrument*)i->data;

			list_foreach(j, instr->noteList) {
				struct complex_note* note = (struct complex_note*)j->data;

				if ((note->endTime <= playbackTime) && note->playing) {
					note->playing = false;
					instr->release_note(instr, *note);
					continue;
				}

				if ((note->startTime <= playbackTime) && (note->endTime >= playbackTime) && !note->playing) {
					note->playing = true;
					instr->play_note(instr, *note);
				}
			}
		}

		return;
	}
}

int playback_init() {
	event_register_keyboard_callback(SDLK_SPACE, KMOD_NONE, playback_toggle_callback);
	event_register_keyboard_callback(SDLK_s, KMOD_NONE, playback_stop_callback);

	previousTime = SDL_GetPerformanceCounter();

	return 0;
}
