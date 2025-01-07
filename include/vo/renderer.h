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

#include <vo/instrument.h>

void renderer_coord_screen_to_stage(int screenX, int screenY, float* stageX, float* stageY);
void renderer_coord_stage_to_screen(float stageX, float stageY, int* screenX, int* screenY);

int renderer_init();
void renderer_iteration();
int renderer_load_instrument_texture(struct instrument* instr, const char* path);
void renderer_set_instrument_texture_draw(struct instrument* instr, int textureIndex, bool doDraw);
void renderer_free_instrument_textures(struct instrument* instr);
void renderer_render_instrument(struct instrument* instr);

void renderer_get_screen_offset(float* x, float* y);
void renderer_set_screen_offset(float x, float y);
void renderer_keyboard_pan_up();
void renderer_keyboard_pan_down();
void renderer_keyboard_pan_right();
void renderer_keyboard_pan_left();
void renderer_mouse_wheel_zoom(int x, int y, float preciseX, float preciseY);
void renderer_mouse_pan(int relX, int relY);
