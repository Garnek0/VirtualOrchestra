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

#include <vo/audio.h>
#include <vo/debug.h>
#include <fluidsynth.h>

// Virtual Orchestra's audio engine uses fluidsynth 
// for SF loading and playing, but I am hoping to 
// write my own thing in the future.

int audio_init() {
	fluid_settings_t* settings = new_fluid_settings();
	
	if (!settings) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to create new settings!\n");
		return -1;
	}

	fluid_synth_t* synth = new_fluid_synth(settings);

	if (!synth) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to create new synth!\n");
		delete_fluid_settings(settings);
		return -1;
	}

	// Default values are too low for what we're trying to do
	fluid_settings_setint(settings, "audio.period-size", 1024);
	fluid_settings_setint(settings, "audio.periods", 2);
	fluid_synth_set_polyphony(synth, 256);

	int soundfontID;
	if ((soundfontID = fluid_synth_sfload(synth, "res/soundfont/msbasic.sf3", 1)) == -1) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to load default (msbasic.sf3) SoundFont file!\n");
		delete_fluid_synth(synth);
		delete_fluid_settings(settings);
		return -1;
	}

	if (fluid_synth_program_select(synth, 0, soundfontID, 0, 0) != FLUID_OK) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to get first program in the SoundFont file!\n");
		delete_fluid_synth(synth);
		delete_fluid_settings(settings);
		return -1;
	}

	fluid_audio_driver_t* driver = new_fluid_audio_driver(settings, synth);

	if (!driver) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to create audio driver for synth!\n");
		delete_fluid_synth(synth);
		delete_fluid_settings(settings);
		return -1;
	}

	fluid_synth_set_gain(synth, 5.0);

	fluid_synth_noteon(synth, 0, 60, 100);
	fluid_synth_noteon(synth, 0, 63, 100);
	fluid_synth_noteon(synth, 0, 66, 100);

	return 0;	
}
