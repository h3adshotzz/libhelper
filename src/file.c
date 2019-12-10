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


file_t *file_load (const char *path)
{
	/* Create a new file struct */
	file_t *ret = malloc (sizeof(file_t));
	memset (ret, '\0', sizeof(file_t));

	/* Set the file path */
	if (!path) {
		errorf ("[*] Error: File path no valid.\n");
		exit (0);
	}
	ret->path = (char *) path;

	/* Load the file */
	ret->desc = fopen (ret->path, "rb");
	if (!ret->desc) {
		errorf ("[*] Error: File could not be loaded\n");
		exit(0);
	}

	/* Calculate the size of the file */
	fseek (ret->desc, 0, SEEK_END);
	ret->size = ftell (ret->desc);
	fseek (ret->desc, 0, SEEK_SET);

	return ret;
}

void file_close (file_t *file)
{
	fclose (file->desc);
	free(file);
}

char *file_load_bytes (file_t *f, size_t size, off_t offset)
{
	char *buf = malloc (size);

	fseek (f->desc, offset, SEEK_SET);
	fread (buf, size, 1, f->desc);

	return buf;
}

int file_read (const char *path, unsigned char **buf, unsigned int *len)
{
	FILE *f = fopen (path, "rb");
	if (!f) {
		return -1;
	}

	fseek (f, 0, SEEK_END);
	size_t size = ftell (f);
	fseek (f, 0, SEEK_SET);

	unsigned char *data = NULL; //= malloc (size);

	// There is an issue here on Linux with MAP_FILE, it cannot find it,
	// so if the OS is linux, use MAP_FAILED instead.
#ifdef __APPLE__
	data = mmap(data, size, PROT_READ | PROT_WRITE, MAP_FILE, f, 0);
#else
	data = mmap (data, size, PROT_READ | PROT_WRITE, MAP_FAILED, f, 0);
#endif

	fclose (f);

	*buf = data;
	*len = size;
	return size;
}