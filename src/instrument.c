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

struct instrument* instrument_new(struct instrument_new_args args) {
	struct instrument* newInstr = (struct instrument*)malloc(sizeof(struct instrument));
	memset((void*)newInstr, 0, sizeof(struct instrument));

	// Give the new instrument an ID

	newInstr->id = __instrument_gen_id();	

	// Fill in fields

	newInstr->x = args.x;
	newInstr->y = args.y;

	newInstr->init = args.init;
	newInstr->fini = args.fini;
	newInstr->play_note = args.play_note;
	newInstr->release_note = args.release_note;

	newInstr->maxTexturesBeforeRealloc = 0;

	if(newInstr->init(newInstr) != 0) {
		debug_log(LOGLEVEL_ERROR, "Instrument: Could not initialize instrument with ID=%d.\n", newInstr->id);
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

	// Tell the renderer to deallocate all textures and texture arrays for this instrument.
	
	renderer_free_instrument_textures(instr);

	if(instr->fini(instr) != 0)
		debug_log(LOGLEVEL_ERROR, "Instrument: Could not properly destroy instrument with ID=%d.\n", instr->id);

	list_remove(instrumentList, (void*)instr);

	free((void*)instr);
}

struct list* instrument_get_list() {
	return instrumentList;
}
