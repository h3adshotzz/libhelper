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

#ifndef FILE_H_
#define FILE_H_

//#include <glib.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "libhelper/strutils.h"
#include "libhelper/hstring.h"

typedef struct file_t {
    FILE    *desc;      /* Loaded file */
    size_t   size;      /* Size of the file */
    char    *path;      /* Original path */
} file_t;

file_t *file_load (const char *path);
void file_close (file_t *file);
int file_read (const char *path, unsigned char **buf, unsigned int *len);

int file_write_new (char *filename, unsigned char *buf, size_t size);

char *file_load_bytes (file_t *f, size_t size, off_t offset);


#endif /* FILE_H_ */
