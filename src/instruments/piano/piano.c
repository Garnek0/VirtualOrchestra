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

		if (renderer_load_instrument_texture(instr, "res/instrument/piano/blackkey.png", i + 139, 0) < 0) {
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
