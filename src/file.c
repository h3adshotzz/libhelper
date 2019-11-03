/**
 *     libhelper
 *     Copyright (C) 2019, @h3adsh0tzz
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include "file.h"

int file_read (const char *path, unsigned char **buf, unsigned int *len)
{
	FILE *f = fopen (path, "rb");
	if (!f) {
		return -1;
	}

	fseek (f, 0, SEEK_END);
	size_t size = ftell (f);
	fseek (f, 0, SEEK_SET);

	unsigned char *data = malloc (size);
	
	int bytes = fread (data, 1, size, f);
	if (bytes != size) {
		g_print ("[*] Error: File size and read bytes do not match!\n");
		fclose (f);
		return -1;
	}
	fclose (f);

	*buf = data;
	*len = bytes;
	return bytes;
}