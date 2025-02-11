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

#include <vo/midi.h>
#include <vo/debug.h>
#include <vo/note.h>
#include <vo/playback.h>
#include <smf.h>
#include <string.h>

int midi_load_file(struct instrument* instr, const char* path, int track) {
	smf_t* midiFile = smf_load(path);

	if (!midiFile) {
		debug_log(LOGLEVEL_ERROR, "Failed to load MIDI file \"%s\" for instrument with ID %d!\n", path, instr->id);
		return -1;
	}

	list_destroy(instr->noteList);

	instr->noteList = list_create();

	// Load the notes from the MIDI file.	
	
	smf_event_t* event, * notePairEvent;
	while ((event = smf_get_next_event(midiFile)) != NULL) {
		if (smf_event_is_metadata(event) || event->track_number != track)
			continue;	

		if ((event->midi_buffer[0] >> 4) == 0x9) {
			struct complex_note* note = malloc(sizeof(struct complex_note));
			memset((void*)note, 0, sizeof(struct complex_note));

			note->startTime = note->endTime = (int)(event->time_seconds*1000);
			note->midiKey = (int)event->midi_buffer[1];
			note->key = NOTE_MIDI_TO_KEY(note->midiKey);
			note->octave = NOTE_MIDI_TO_OCTAVE(note->midiKey);

			// Find the noteOff event that corresponds to this noteOn event
			
			while ((notePairEvent = smf_get_next_event(midiFile)) != NULL) {
				if (smf_event_is_metadata(notePairEvent) || notePairEvent->track->track_number != track)
					continue;

				if (((notePairEvent->midi_buffer[0] >> 4) == 0x8) && (notePairEvent->midi_buffer[1] == event->midi_buffer[1])) {
					note->endTime = (int)(notePairEvent->time_seconds*1000);
					break;
				}
			}

			list_insert(instr->noteList, (void*)note);

			(void)smf_seek_to_event(midiFile, event);
			smf_skip_next_event(midiFile);
		}
	}

	playback_reset();
	smf_delete(midiFile);

	return 0;
}
