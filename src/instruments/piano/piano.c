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

#include <vo/instruments/piano.h>
#include <vo/debug.h>
#include <vo/renderer.h>
#include <vo/note.h>

#define PIANO_FIRST_WHITE_KEY_TEXTURE 0
#define PIANO_FIRST_BLACK_KEY_TEXTURE 36

int piano_init(struct instrument* instr) {

	for (int i = 0; i < 31 * 36; i+=31) {
		if (renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", i, 0) < 0) {
			debug_log(LOGLEVEL_ERROR, "Piano: Failed to load white key texture! (ID=%d)\n", instr->id);
			return -1;
		}
	}

	for (int i = 0; i < 217 * 5; i+=217) {
		if (renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 14, 0) < 0) {
			debug_log(LOGLEVEL_ERROR, "Piano: Failed to load black key texture! (ID=%d)\n", instr->id);
			return -1;
		}

		if (renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 49, 0) < 0) {
			debug_log(LOGLEVEL_ERROR, "Piano: Failed to load black key texture! (ID=%d)\n", instr->id);
			return -1;
		}

		if (renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 107, 0) < 0) {
			debug_log(LOGLEVEL_ERROR, "Piano: Failed to load black key texture! (ID=%d)\n", instr->id);
			return -1;
		}

		if (renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 140, 0) < 0) {
			debug_log(LOGLEVEL_ERROR, "Piano: Failed to load black key texture! (ID=%d)\n", instr->id);
			return -1;
		}

		if (renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 173, 0) < 0) {
			debug_log(LOGLEVEL_ERROR, "Piano: Failed to load black key texture! (ID=%d)\n", instr->id);
			return -1;
		}
	}

	debug_log(LOGLEVEL_DEBUG, "Piano: Initialized! (ID=%d)\n", instr->id);
	return 0;
}

int piano_fini(struct instrument* instr) {
	debug_log(LOGLEVEL_DEBUG, "Piano: Destroyed! (ID=%d)\n", instr->id);
	return 0;
}

int piano_play_note(struct instrument* instr, int note, int octave) {
	if ((octave > 6) && (octave != 6) && (note != NOTE_C))
		return -1;

	if (octave < 1)
		return -1;

	int textureIndex = 0;

	if (NOTE_IS_NATURAL(note)) {
		switch (note) {
			case NOTE_C:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE;
				break;
			case NOTE_D:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 1;
				break;
			case NOTE_E:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 2;
				break;
			case NOTE_F:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 3;
				break;
			case NOTE_G:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 4;
				break;
			case NOTE_A:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 5;
				break;
			case NOTE_B:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 6;
				break;
			default:
				return -1;	
		}

		textureIndex += 7*(octave-1);
	} else {
		switch (note) {
			case NOTE_Cs_Db:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE;
				break;
			case NOTE_Ds_Eb:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE + 1;
				break;
			case NOTE_Fs_Gb:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE + 2;
				break;
			case NOTE_Gs_Ab:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE + 3;
				break;
			case NOTE_As_Bb:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE + 4;
				break;
			default:
				return -1;
		}

		textureIndex += 5*(octave-1);
	}

	renderer_set_instrument_texture_opacity(instr, textureIndex, 30);

	return 0;
}

int piano_release_note(struct instrument* instr, int note, int octave) {
	if ((octave > 6) && (octave != 6) && (note != NOTE_C))
		return -1;

	if (octave < 1)
		return -1;

	int textureIndex = 0;

	if (NOTE_IS_NATURAL(note)) {
		switch (note) {
			case NOTE_C:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE;
				break;
			case NOTE_D:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 1;
				break;
			case NOTE_E:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 2;
				break;
			case NOTE_F:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 3;
				break;
			case NOTE_G:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 4;
				break;
			case NOTE_A:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 5;
				break;
			case NOTE_B:
				textureIndex = PIANO_FIRST_WHITE_KEY_TEXTURE + 6;
				break;
			default:
				return -1;	
		}

		textureIndex += 7*(octave-1);
	} else {
		switch (note) {
			case NOTE_Cs_Db:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE;
				break;
			case NOTE_Ds_Eb:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE + 1;
				break;
			case NOTE_Fs_Gb:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE + 2;
				break;
			case NOTE_Gs_Ab:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE + 3;
				break;
			case NOTE_As_Bb:
				textureIndex = PIANO_FIRST_BLACK_KEY_TEXTURE + 4;
				break;
			default:
				return -1;
		}

		textureIndex += 5*(octave-1);
	}

	renderer_set_instrument_texture_opacity(instr, textureIndex, 100);

	return 0;
}
