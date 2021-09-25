//===----------------------------------------------------------------------===//
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
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//  Copyright (C) 2020, Is This On?, @h3adsh0tzz
//  Copyright (C) 2021, Is This On? Holdings
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//


#include "libhelper-logger.h"
#include "libhelper-macho.h"
#include "libhelper-file.h"

char *
macho_load (const char *filename)
{
    file_t      *file = NULL;
    void        *macho = NULL;

    /* Check the filename is set */
    if (!filename) {
        errorf ("macho_load(): No filename specified.\n");
        return NULL;
    }

    /* Load the file */
    file = file_load (filename);
    if (!file) {
        errorf ("macho_load(): Filename invalid. Could not load the file.\n");
        free (file);
        return NULL;
    }

    /* Verify the file loaded correctly */
    if (file->size <= 0) {
        errorf ("macho_load(): File could not be loaded properly. Filesize invalid.\n");
        free (file);
        return NULL;
    }

    /* We use macho_create_from_buffer() to reuse more code */
    debugf ("macho_load(): creating mach-o struct.\n");
    macho = macho_create_from_buffer ((unsigned char *) file_get_data (file, 0));

    /* Verify the Mach-O */
    if (macho == NULL) {
        errorf ("macho_load(): Error creating macho: macho == NULL.\n");
        free (file);
        return NULL;
    }

    debugf ("macho_load(): all is well.\n");
    return macho;
}

macho_t *
macho_create_from_buffer (unsigned char *data)
{
    /* Check that the data pointer is valid */
    if (!data) {
        errorf ("macho_create_from_buffer(): Invalid data pointer.\n");
        return NULL;
    }

    /* Check if the data is a FAT file */
    if (FAT(data)) {
        errorf ("macho_create_from_buffer(): Cannot handle FAT archive with this function.\n");
        return NULL;
    }

    /* Try to load the mach header here. */
    mach_header_t *hdr = (mach_header_t *) data;
    mach_header_type_t type = mach_header_verify (hdr->magic);

    /* Handle the file based on the architecture */
    if (type == MH_TYPE_MACHO64) {
        /* return macho_64_create_from_buffer (data) */
    } else if (type == MH_TYPE_MACHO32) {
        /* return macho_32_create_from_buffer (data) */
    } else {
        errorf ("macho_create_from_buffer(): Cannot handle file with magic: 0x%08x\n", hdr->magic);
        return NULL;
    }
}

void *
macho_get_bytes (void *macho, uint32_t offset)
{
    macho_t *tmp = (macho_t *) macho;
    return (void *) (tmp->data + offset);
}