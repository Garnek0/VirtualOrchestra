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
#include <vo/renderer.h>
#include <vo/note.h>

int keyTextureIndexes[61];

int piano_init(struct instrument* instr) {

	for (int i = 0; i < 217 * 5; i+=217) {
		if ((keyTextureIndexes[i/217*12] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", i, 0, 0)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 1] = renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 14, 0, 1)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 2] =	renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", i + 31, 0, 0)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 3] =	renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 49, 0, 1)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 4] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", i + 62, 0, 0)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 5] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", i + 93, 0, 0)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 6] = renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 107, 0, 1)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 7] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", i + 124, 0, 0)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 8] = renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 140, 0, 1)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 9] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", i + 155, 0, 0)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 10] = renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 173, 0, 1)) < 0)
			goto fail;

		if ((keyTextureIndexes[i/217*12 + 11] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", i + 186, 0, 0)) < 0)
			goto fail;
	}

	// The last C
	if ((keyTextureIndexes[60] = renderer_load_instrument_texture(instr, "res/instrument/piano/whitekey.png", 5 * 217, 0, 0)) < 0)
		goto fail;

	debug_log(LOGLEVEL_DEBUG, "Piano: Initialized! (ID=%d)\n", instr->id);
	return 0;

fail:
	debug_log(LOGLEVEL_ERROR, "Piano: Failed to load key texture! (ID=%d)\n", instr->id);
	return -1;	
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

	octave -= 2;

	renderer_set_instrument_texture_opacity(instr, keyTextureIndexes[octave*12+note], 30);	

	return 0;
}

int piano_release_note(struct instrument* instr, int note, int octave) {
	if ((octave > 6) && (octave != 6) && (note != NOTE_C))
		return -1;

	if (octave < 1)
		return -1;

	octave -= 2;

	renderer_set_instrument_texture_opacity(instr, keyTextureIndexes[octave*12+note], 100);

	return 0;
}
