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

struct soundfont_riff_chunk* soundfont_find_chunk(struct soundfont_riff_chunk* root, const char* fourcc, const char* firstFourBytes) {
	for (uint32_t i = 0; i < sizeof(struct soundfont_riff_chunk) + root->chunkSize;) {
		struct soundfont_riff_chunk* currentChunk = (struct soundfont_riff_chunk*)(&root->chunkData[i]);

		if (!strncmp(fourcc, currentChunk->chunkID, 4)) {
			if (firstFourBytes) {
				if (currentChunk->chunkSize < 4)
					continue;

				if (strncmp(firstFourBytes, (char*)currentChunk->chunkData, 4) != 0)
					continue;
			}

			struct soundfont_riff_chunk* newChunk = (struct soundfont_riff_chunk*)malloc(sizeof(struct soundfont_riff_chunk) + currentChunk->chunkSize);
			memcpy((void*)newChunk, (void*)currentChunk, sizeof(struct soundfont_riff_chunk) + currentChunk->chunkSize);

			return newChunk;
		} else {
			if (i == 0) {
				i = 4;
				continue;
			}
		}

		i += sizeof(struct soundfont_riff_chunk) + currentChunk->chunkSize + currentChunk->chunkSize % 2;
	}

	return NULL;
}

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

	sfData = malloc(sizeof(struct soundfont_riff_chunk) + mainRiffChunk->chunkSize);

	fseek(sfFile, 0, SEEK_SET);
	fread((void*)sfData, sizeof(struct soundfont_riff_chunk) + mainRiffChunk->chunkSize, 1, sfFile);
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

	struct soundfont_riff_chunk* infoChunk = soundfont_find_chunk(sfData, "LIST", "INFO");

	if (!infoChunk) {
		debug_log(LOGLEVEL_ERROR, "SoundFont: Failed to load SoundFont file \"%s\": SoundFont INFO chunk missing.\n", path);
		free(sfData);
		return -1;
	}

	struct soundfont_riff_chunk* INAMChunk = soundfont_find_chunk(infoChunk, "INAM", NULL);

	if (!INAMChunk) {
		debug_log(LOGLEVEL_ERROR, "Soundfont: Cannot find SoundFont name chunk (INAM).\n");
	} else {
		const char* name = (char*)INAMChunk->chunkData;

		debug_log(LOGLEVEL_INFO, "Soundfont: Name: %s\n", name);
		free(INAMChunk);
	}

	struct soundfont_riff_chunk* ifilChunk = soundfont_find_chunk(infoChunk, "ifil", NULL);

	if (!ifilChunk) {
		debug_log(LOGLEVEL_ERROR, "SoundFont: Failed to load SoundFont file \"%s\": SoundFont ifil (version) chunk missing.\n", path);
		free(ifilChunk);
		free(infoChunk);
		free(sfData);
		return -1;
	} else {
		struct soundfont_version_tag* verTag = (struct soundfont_version_tag*)ifilChunk->chunkData;
		
		debug_log(LOGLEVEL_INFO, "Soundfont: Version %d.%02d\n", verTag->major, verTag->minor);
		free(ifilChunk);
	}	

	struct soundfont_riff_chunk* isngChunk = soundfont_find_chunk(infoChunk, "isng", NULL);

	if (!isngChunk) {
		debug_log(LOGLEVEL_ERROR, "Soundfont: Cannot find SoundFont target sound engine chunk (isng).\n");
	} else {
		const char* targetSoundEngine = (char*)isngChunk->chunkData;
		
		debug_log(LOGLEVEL_INFO, "Soundfont: Target Sound Engine: %s\n", targetSoundEngine);
		free(isngChunk);
	}

	free(infoChunk);
	return 0;
}
