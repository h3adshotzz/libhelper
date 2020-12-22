//===--------------------------- libhelper ----------------------------===//
//
//                         The Libhelper Project
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//	Copyright (C) 2020, Is This On?, @h3adsh0tzz
//
//  me@h3adsh0tzz.com.
//
//
//===------------------------------------------------------------------===//

#include "libhelper/libhelper.h"
#include "libhelper/libhelper-macho.h"
#include "libhelper/libhelper-dyld.h"

/***********************************************************************
* DYLD Shared Cache Parser.
***********************************************************************/

/**
 * 
 */
dyld_cache_t *dyld_cache_load (const char *filename)
{
    file_t          *file = NULL;
    dyld_cache_t    *dyld = NULL;

    if (!filename) {
        errorf ("dyld_cache_load(): no filename specified.\n");
        return NULL;
    }

    debugf ("dyld_cache_load(): reading dyld_cache from filename: %s\n", filename);

    // load the file
    file = file_load (filename);
    if (file->size == 0) {
        errorf ("dyld_cache_load(): file could not be loaded.\n");
        free (dyld);
        return NULL;
    }

    // load the dyld cache from the file
    debugf ("dyld_cache_load(): creating dyld cache struct.\n");
    dyld = dyld_cache_create_from_file (file);

    if (dyld == NULL)
        return NULL;

    debugf ("dyld_cache_load(): dyld_shared_cache loaded successfully (probably).\n");
    return dyld;
}

dyld_cache_t *dyld_cache_create_from_file (file_t *file)
{
    dyld_cache_t *dyld = malloc (sizeof (dyld_cache_t));

    dyld->path = file->path;
    dyld->size = file->size;
    dyld->data = (uint8_t *) file_get_data (file, 0);

    dyld->header = malloc (sizeof (dyld_cache_header_t));
    memcpy (dyld->header, file->data, sizeof (dyld_cache_header_t));

    return dyld;
}

void *dyld_get_bytes (dyld_cache_t *dyld, uint32_t offset)
{
    return (void *) (dyld->data + offset);
}

void dyld_read_bytes (dyld_cache_t *dyld, uint32_t offset, void *buffer, size_t size)
{
    memcpy (buffer, dyld_get_bytes (dyld, offset), size);
}

void *dyld_load_bytes (dyld_cache_t *dyld, uint32_t size, uint32_t offset)
{
    void *ret = malloc (size);
    memcpy (ret, (void *) (dyld->data + offset), size);
    return ret;
}

/***********************************************************************
* DYLD Shared Cache Header functions.
***********************************************************************/

int dyld_shared_cache_verify_header (unsigned char *dyld_ptr)
{
    if (!strncmp ((const char *) dyld_ptr, "dyld_", 5))
        return 1;
    return 0;
}