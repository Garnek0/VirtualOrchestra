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

#define NOTE_C 0
#define NOTE_Cs_Db 1
#define NOTE_D 2
#define NOTE_Ds_Eb 3
#define NOTE_E 4
#define NOTE_F 5
#define NOTE_Fs_Gb 6
#define NOTE_G 7
#define NOTE_Gs_Ab 8
#define NOTE_A 9
#define NOTE_As_Bb 10
#define NOTE_B 11

#define NOTE_IS_NATURAL(x) \
	((x != NOTE_Cs_Db) && (x != NOTE_Ds_Eb) && \
	(x != NOTE_Fs_Gb) && (x != NOTE_Gs_Ab) && (x != NOTE_As_Bb))
