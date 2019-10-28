/**
 *     ===== ORIGINAL LICENSE ====
 *     libcrippy-1.0 - file.h
 *     Copyright (C) 2013 Crippy-Dev Team
 *     Copyright (C) 2010-2013 Joshua Hill
 *     ===========================
 * 
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

#define BUFSIZE 4096

file_t* file_create() {
	file_t* file = (file_t*) malloc(sizeof(file_t));
	if (file) {
		memset(file, '\0', sizeof(file_t));
	}
	return file;
}

file_t* file_open(const char* path) {
	uint64_t got = 0;
	uint8_t buffer[4096];

	file_t* file = file_create();
	if (file) {
		file->desc = fopen(path, "rb");
		if (file->desc == NULL) {
			g_printerr("Unable to open file %s\n", path);
			return NULL;
		}

		file->path = strdup(path);
		if (file->path == NULL) {
			g_printerr("Unable to allocate memory for file path\n");
			file_free(file);
			return NULL;
		}

		fseek(file->desc, 0, SEEK_END);
		file->size = ftell(file->desc);
		fseek(file->desc, 0, SEEK_SET);

		file->offset = 0;
		file->data = (unsigned char*) malloc(file->size);
		if (file->data == NULL) {
			g_printerr("Unable to allocate memory for file\n");
			file_free(file);
			return NULL;
		}

		uint64_t offset = 0;
		while (offset < file->size) {
			memset(buffer, '\0', BUFSIZE);
			got = fread(buffer, 1, BUFSIZE, file->desc);
			if (got > 0) {
				offset += got;
				memcpy(&(file->data[offset]), buffer, got);
			} else {
				break;
			}
		}
		g_printerr("Read in %llu of %llu bytes from %s\n", file->offset, file->size, file->path);
		// We have the data stored in memory now, so we don't need to keep this open anymore
		//fseek(file->desc, 0, SEEK_SET);
		file_close(file);
		file->offset = 0;
	}
	return file;
}

void file_close(file_t* file) {
	if (file) {
		if (file->desc) {
			fclose(file->desc);
			file->desc = NULL;
		}
	}
}

void file_free(file_t* file) {
	if (file) {
		if (file->desc) {
			file_close(file);
			file->desc = NULL;
		}
		if (file->path) {
			free(file->path);
			file->path = NULL;
		}
		if(file->data) {
			free(file->data);
			file->data = NULL;
		}
		file->size = 0;
		file->offset = 0;
		free(file);
	}
}

int file_read(file_t* file, unsigned char** buf, unsigned int* length) {
	FILE* fd = file->desc;
	if(fd == NULL) {
		return -1;
	}

	fseek(fd, 0, SEEK_END);
	long size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	unsigned char* data = malloc(size);

	int bytes = fread(data, 1, size, fd);
	if(bytes != size) {
		fclose(fd);
		return -1;
	}
	fclose(fd);

	*buf = data;
	*length = bytes;
	return bytes;
}

int file_write(const char* file, unsigned char* buf, unsigned int length) {
	FILE* fd = NULL;
	fd = fopen(file, "wb");
	if(fd == NULL) {
		return -1;
	}

	int bytes = fwrite(buf, 1, length, fd);
	if(bytes != length) {
		fclose(fd);
		return -1;
	}
	fclose(fd);
	return bytes;
}

int file_copy(const char* from, const char* to)
{
	FILE* ffr = NULL;
	FILE* fto = NULL;
	char buf[8192];
	size_t size;

	ffr = fopen(from, "rb");
	if(ffr == NULL) {
		g_printerr("could not open source file '%s' for reading\n", from);
		return -1;
	}
	fto = fopen(to, "wb");
	if(fto == NULL) {
		g_printerr("could not open target file '%s' for writing\n", to);
		fclose(ffr);
		return -1;
	}

	while (!feof(ffr)) {
		size = fread(buf, 1, sizeof(buf), ffr);
		if (size > 0) {
			size_t bytes = fwrite(buf, 1, size, fto);
			if (bytes != size) {
				fclose(fto);
				fclose(ffr);
				return -1;
			}
		}
	}
	fclose(fto);
	fclose(ffr);

	return 0;
}