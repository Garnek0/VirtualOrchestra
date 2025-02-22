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
#include <vo/list.h>
#include <vo/note.h>

// Virtual Orchestra's audio engine uses fluidsynth 
// for SF loading and playing, but I am hoping to 
// write my own thing in the future.

static fluid_settings_t* settings;

int audio_init_instrument(struct instrument* instr, const char* soundfontPath, int bank, int preset, int polyphony) {
	instr->synth = new_fluid_synth(settings);

	if (!instr->synth) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to create new synth for instrument with ID %d!\n", instr->id);
		return -1;
	}

	int soundfontID;
	if ((soundfontID = fluid_synth_sfload(instr->synth, soundfontPath, 1)) == -1) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to load soundfont file \"%s\"!\n", soundfontPath);
		return -1;
	}

	fluid_synth_set_polyphony(instr->synth, polyphony);
	fluid_synth_set_gain(instr->synth, 5.0);

	instr->audioDriver = new_fluid_audio_driver(settings, instr->synth);
	if (!instr->audioDriver) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to create new audio driver for instrument with ID %d!\n", instr->id);
		return -1;
	}

	fluid_synth_program_select(instr->synth, 0, soundfontID, bank, preset);

	return 0;
}

void audio_note_on(struct instrument* instr, struct simple_note note) {
	fluid_synth_noteon(instr->synth, 0, NOTE_TO_MIDI_KEY(note.key, note.octave), note.velocity);
}

void audio_note_off(struct instrument* instr, struct simple_note note) {
	fluid_synth_noteoff(instr->synth, 0, NOTE_TO_MIDI_KEY(note.key, note.octave));
}

int audio_init() {
	// Initialize the fluidsynth settings
	settings = new_fluid_settings();

	if (!settings) {
		debug_log(LOGLEVEL_ERROR, "Audio Engine: FluidSynth: Failed to create new settings!\n");
		return -1;
	}

	// Default values are too low for what we're trying to do
	fluid_settings_setint(settings, "audio.period-size", 1024);
	fluid_settings_setint(settings, "audio.periods", 4);

	return 0;	
}
