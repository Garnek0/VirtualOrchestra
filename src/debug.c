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

#include <vo/debug.h>

#include <stdarg.h>
#include <stdio.h>

int debug_log(int loglevel, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	int chars;

	switch(loglevel) {
		case LOGLEVEL_DEBUG:
			fprintf(stderr, "\e[0;35m[ DEBUG ] ");
			break;
		case LOGLEVEL_INFO:
			fprintf(stderr, "\e[0;34m[ INFO  ] ");
			break;
		case LOGLEVEL_WARN:
			fprintf(stderr, "\e[0;33m[ WARN  ] ");
			break;
		case LOGLEVEL_ERROR:
			fprintf(stderr, "\e[0;31m[ ERROR ] ");
			break;
		case LOGLEVEL_FATAL:
			fprintf(stderr, "\e[0;31m[ FATAL ] ");
			break;
		default:
			break;
	}

	chars = vfprintf(stderr, fmt, args);
	fprintf(stderr, "\e[0m");

	va_end(args);

	return chars;
}
