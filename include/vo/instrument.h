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

#pragma once

#include <vo/list.h>

struct instrument {
	int id; // Instrument ID

	float x, y; // Base coordinates
	
	char* graphic; // Instrument graphic
	
	void* rendererData; // Renderer private data
};

int instrument_init();

struct instrument* instrument_new(char* graphicPath);
void instrument_set_position(struct instrument* instr, float x, float y);
void instrument_destroy(struct instrument* instr);

struct list* instrument_get_list();
