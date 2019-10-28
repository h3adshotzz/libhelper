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

#ifndef FILE_H_
#define FILE_H_

#include <glib.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct file_t {
	FILE* desc;
	char* path;
	uint64_t size;
	uint64_t offset;
	unsigned char* data;
} file_t;

file_t* file_create();
void file_close(file_t* file);
void file_free(file_t* file);
file_t* file_open(const char* path);

int file_read(file_t* file, unsigned char** buf, unsigned int* length);
int file_write(const char* file, unsigned char* buf, unsigned int length);
int file_copy(const char* from, const char* to);

#endif /* FILE_H_ */
