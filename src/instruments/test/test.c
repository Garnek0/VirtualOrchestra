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

#include <vo/instruments/test.h>
#include <vo/debug.h>
#include <vo/renderer.h>

int test_init(struct instrument* instr) {
	if (renderer_load_instrument_texture(instr, "res/instrument/test/test.png", 0, 0) < 0) {
		debug_log(LOGLEVEL_ERROR, "Test Instrument: Failed to load texture! (ID=%d)\n", instr->id);
		return -1;
	}

	debug_log(LOGLEVEL_DEBUG, "Test Instrument: Initialized! (ID=%d)\n", instr->id);
	return 0;
}

int test_fini(struct instrument* instr) {
	debug_log(LOGLEVEL_DEBUG, "Test Instrument: Destroyed! (ID=%d)\n", instr->id);
	return 0;
}
