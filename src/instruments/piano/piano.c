/*	
 *	SPDX-License-Identifier: GPL-3.0-only
 *
 *	Virtual Orchestra - Musical Instrument Simulation
 *	Copyright (C) 2024 Garnek0 (Popa Vlad) and Contributors
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vo/instruments/piano.h>
#include <vo/debug.h>
#include <vo/gfxui/renderer.h>
#include <vo/note.h>
#include <vo/audio.h>

int keyTextureIndexes[61];
int pressedKeyTextureIndexes[61];

int piano_init(struct instrument* instr) {

#define LOAD_WHITE_KEY_TEXTURE(i, x, xoff) \
	if (((keyTextureIndexes[(i)/217*12 + (x)] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", (xoff), 0, 0)) < 0) || \
		((pressedKeyTextureIndexes[(i)/217*12 + (x)] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey-pressed.png", (xoff), 0, 1)) < 0)) \
			goto fail;

#define LOAD_BLACK_KEY_TEXTURE(i, x, xoff) \
	if (((keyTextureIndexes[(i)/217*12 + (x)] = renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", (xoff), 0, 2)) < 0) || \
		((pressedKeyTextureIndexes[(i)/217*12 + (x)] = renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey-pressed.png", (xoff), 0, 3)) < 0)) \
			goto fail;
	
	// Load piano key textures
	for (int i = 0; i < 217 * 5; i+=217) {
		LOAD_WHITE_KEY_TEXTURE(i, 0, i + 0);
		LOAD_BLACK_KEY_TEXTURE(i, 1, i + 14);
		LOAD_WHITE_KEY_TEXTURE(i, 2, i + 1 * 31);
		LOAD_BLACK_KEY_TEXTURE(i, 3, i + 49);
		LOAD_WHITE_KEY_TEXTURE(i, 4, i + 2 * 31);
		LOAD_WHITE_KEY_TEXTURE(i, 5, i + 3 * 31);
		LOAD_BLACK_KEY_TEXTURE(i, 6, i + 107);
		LOAD_WHITE_KEY_TEXTURE(i, 7, i + 4 * 31);
		LOAD_BLACK_KEY_TEXTURE(i, 8, i + 140);
		LOAD_WHITE_KEY_TEXTURE(i, 9, i + 5 * 31);
		LOAD_BLACK_KEY_TEXTURE(i, 10, i + 173);
		LOAD_WHITE_KEY_TEXTURE(i, 11, i + 6 * 31);	
	}

#undef LOAD_WHITE_KEY_TEXTURE
#undef LOAD_BLACK_KEY_TEXTURE

	// The last C
	if (((keyTextureIndexes[60] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", 5*217, 0, 0)) < 0) || \
		((pressedKeyTextureIndexes[60] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey-pressed.png", 5*217, 0, 0)) < 0)) \
			goto fail;

	// Hide all pressed textures
	for (int i = 0; i < 61; i++)
		renderer_set_instrument_texture_opacity(instr, pressedKeyTextureIndexes[i], 0);

	debug_log(LOGLEVEL_DEBUG, "Piano: New piano initialized! (ID=%d)\n", instr->id);
	return 0;

fail:
	debug_log(LOGLEVEL_ERROR, "Piano: Failed to load key texture! (ID=%d)\n", instr->id);
	return -1;	
}

int piano_fini(struct instrument* instr) {
	// Destroyed!?... Poor piano...
	debug_log(LOGLEVEL_DEBUG, "Piano: Destroyed! (ID=%d)\n", instr->id);
	return 0;
}

int piano_play_note(struct instrument* instr, int note, int octave) {
	if ((octave > 6) && (octave != 6) && (note != NOTE_C))
		return -1;

	if (octave < 1)
		return -1;

	octave -= 2;

	renderer_set_instrument_texture_opacity(instr, pressedKeyTextureIndexes[octave*12+note], 60);

	audio_note_on(instr, note, octave+2, 127 - (instr->dynamic - 1)*(127/8));

	return 0;
}

int piano_release_note(struct instrument* instr, int note, int octave) {
	if ((octave > 6) && (octave != 6) && (note != NOTE_C))
		return -1;

	if (octave < 1)
		return -1;

	octave -= 2;

	renderer_set_instrument_texture_opacity(instr, pressedKeyTextureIndexes[octave*12+note], 0);

	audio_note_off(instr, note, octave+2);

	return 0;
}
