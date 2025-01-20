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
#include <vo/audio/soundfont.h>
#include <vo/debug.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// We load one single SF3 file containing all our instruments.
struct soundfont_riff_chunk* sfData;

int soundfont_load(const char* path) {
	FILE* sfFile = fopen(path, "rb");

	if (!sfFile) {
		debug_log(LOGLEVEL_ERROR, "SoundFont: Failed to load SoundFont file \"%s\": %s.\n", path, strerror(errno));
		return -1;
	}

	struct soundfont_riff_chunk* mainRiffChunk = malloc(sizeof(struct soundfont_riff_chunk));

	fread((void*)mainRiffChunk, sizeof(struct soundfont_riff_chunk), 1, sfFile);

	if (mainRiffChunk->chunkID[0] != 'R' ||
		mainRiffChunk->chunkID[1] != 'I' ||
		mainRiffChunk->chunkID[2] != 'F' ||
		mainRiffChunk->chunkID[3] != 'F') {
			debug_log(LOGLEVEL_ERROR, "SoundFont: Failed to load SoundFont file \"%s\": main RIFF chunk missing or corrupted.\n", path);
			fclose(sfFile);
			free(mainRiffChunk);
			return -1;
	}

	sfData = malloc(mainRiffChunk->chunkSize);

	fseek(sfFile, 0, SEEK_SET);
	fread((void*)sfData, mainRiffChunk->chunkSize, 1, sfFile);
	fclose(sfFile);

	free(mainRiffChunk);

	if ((char)sfData->chunkData[0] != 's' ||
		(char)sfData->chunkData[1] != 'f' ||
		(char)sfData->chunkData[2] != 'b' ||
		(char)sfData->chunkData[3] != 'k') {
			debug_log(LOGLEVEL_ERROR, "SoundFont: Failed to load SoundFont file \"%s\": SoundFont signature missing.\n", path);
			free(sfData);
			return -1;
	}

	debug_log(LOGLEVEL_INFO, "SoundFont: Using SoundFont file \"%s\" for instrument samples.\n", path);

	return 0;
}
