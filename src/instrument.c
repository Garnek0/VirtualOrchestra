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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vo/instrument.h>
#include <vo/debug.h>
#include <vo/renderer.h>

struct __instrument_node {
	struct instrument* instr;
	
	struct __instrument_node* next;
};
struct __instrument_node* instrumentListHead;
struct __instrument_node* instrumentListTail;

static void __instrument_list_add(struct instrument* instr) {
	struct __instrument_node* newNode = (struct __instrument_node*)malloc(sizeof(struct __instrument_node));
	memset((void*)newNode, 0, sizeof(struct __instrument_node));

	newNode->instr = instr;

	if (!instrumentListHead)
		instrumentListHead = newNode;
	else
		instrumentListTail->next = newNode;

	instrumentListTail = newNode;
}

static void __instrument_list_remove(struct instrument* instr) {
	struct __instrument_node* prev = NULL;

	for (struct __instrument_node* i = instrumentListHead; i; i = i->next) {
		if (i->instr == instr) {
			if (i == instrumentListHead) {
				instrumentListHead = i->next;
			} else {
				prev->next = i->next;
				if (i == instrumentListTail)
					instrumentListTail = prev;
			}
			
		}
		prev = i;
	}
}

static int __instrument_gen_id(){
    static int id = 0;
    return id++;
}

struct instrument* instrument_new(char* graphicPath) {
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

	// Inform the renderer about this new instrument so that it can
	// generate its data structure and stuff.
	
	if (renderer_init_instrument(newInstr) != 0) {
		debug_log(LOGLEVEL_ERROR, "Instrument: Renderer initialization for instrument with ID=%d failed.\n", newInstr->id);
		goto fail;
	}	

	__instrument_list_add(newInstr);

	return newInstr;

fail:
	free((void*)newInstr);
	return NULL;
}

void instrument_destroy(struct instrument* instr) {
	if (!instr) {
		debug_log(LOGLEVEL_WARN, "Instrument: Attempt to destroy NULL instrument.\n");
		return;
	}

	// Tell the renderer to deallocate its things for this instrument.
	
	renderer_fini_instrument(instr);

	__instrument_list_remove(instr);

	free((void*)instr);
}

// NOTE: If you're wondering why this isnt part of renderer.c, its
// because currently the instrument list is only accessible to
// this translation unit. Yeah, bad design, i know. This will
// probably be fixed in the future, though.
void instrument_render_all() {
	for (struct __instrument_node* i = instrumentListHead; i; i = i->next)
		renderer_render_instrument(i->instr, 0, 0);
}
