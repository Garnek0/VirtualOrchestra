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

#include <vo/instrument.h>
#include <vo/debug.h>
#include <vo/renderer.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct list* instrumentList;

static int __instrument_gen_id(){
    static int id = 0;
    return id++;
}

int instrument_init() {
	instrumentList = list_create();

	return 0;
}

struct instrument* instrument_new(char* graphicPath, int (*init)(struct instrument*)) {
	struct instrument* newInstr = (struct instrument*)malloc(sizeof(struct instrument));
	memset((void*)newInstr, 0, sizeof(struct instrument));

	// Give the new instrument an ID

	newInstr->id = __instrument_gen_id();	

	// Make sure the graphic file can be opened

	FILE* graphicFile = fopen(graphicPath, "r");

	if (!graphicFile) {
		debug_log(LOGLEVEL_ERROR, "Instrument: Graphic file \"%s\" for instrument with ID=%d could not be opened. Is the file missing?\n", graphicPath, newInstr->id);
		goto fail;
	}

	fclose(graphicFile);

	newInstr->graphic = graphicPath;

	newInstr->init = init;

	// Inform the renderer about this new instrument so that it can
	// generate its data structure and stuff.
	
	if (renderer_init_instrument(newInstr) != 0) {
		debug_log(LOGLEVEL_ERROR, "Instrument: Renderer initialization for instrument with ID=%d failed.\n", newInstr->id);
		goto fail;
	}

	if(newInstr->init(newInstr) != 0) {
		debug_log(LOGLEVEL_ERROR, "Instrument: Could not initialize instrument wiht ID=%d.\n", newInstr->id);
		goto fail;
	}

	list_insert(instrumentList, (void*)newInstr);

	return newInstr;

fail:
	free((void*)newInstr);
	return NULL;
}

void instrument_set_position(struct instrument* instr, float x, float y) {
	instr->x = x;
	instr->y = y;
}

void instrument_destroy(struct instrument* instr) {
	if (!instr) {
		debug_log(LOGLEVEL_WARN, "Instrument: Attempt to destroy NULL instrument.\n");
		return;
	}

	// Tell the renderer to deallocate its things for this instrument.
	
	renderer_free_instrument(instr);

	list_remove(instrumentList, (void*)instr);

	free((void*)instr);
}

struct list* instrument_get_list() {
	return instrumentList;
}
