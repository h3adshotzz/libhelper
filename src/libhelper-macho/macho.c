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
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//


#include "libhelper-logger.h"
#include "libhelper-macho.h"
#include "libhelper-file.h"
#include "libhelper-fat.h"

/**
 *  \brief
 */
static file_t *
macho_load_file_from_filename (const char *filename)
{
    /* Check the filename is set */
    if (!filename) {
        errorf ("macho_load(): No filename specified.\n");
        return NULL;
    }

    /* Load the file */
    file_t *file = file_load (filename);
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
    return file;
}

///////////////////////////////////////////////////////////////////////////////

mach_header_type_t
macho_check_arch (const char *filename)
{
    /* load the file and check the magic */
    file_t *file = macho_load_file_from_filename (filename);
    mach_header_type_t type = mach_header_verify (*(uint32_t *) file_get_data (file, 0));

    /* free the file */
    file_close (file);

    /* return mach_header_verify() */
    return type;
}

macho_t *
macho_64_load (const char *filename)
{
    file_t      *file = NULL;
    void        *macho = NULL;

    file = macho_load_file_from_filename (filename);

    /* We use macho_64_create_from_buffer() to load the macho from a pointer */
    macho = macho_64_create_from_buffer (file->data);

    /* Verify the Mach-O */
    if (macho == NULL) {
        errorf ("macho_load(): Error creating macho: macho == NULL.\n");
        free (file);
        return NULL;
    }

    /* the file structure is no longer needed, as the data is copied to the macho */
    free (file);

    debugf ("macho_load(): all is well.\n");
    return macho;
}

//macho_32_t *
//macho_32_load()

void *
macho_load_bytes (void *macho, size_t size, uint32_t offset)
{
    macho_t *tmp = (macho_t *) macho;
    void *ret = malloc (size);

    memcpy (ret, tmp->data + offset, size);
    return ret;
}

void
macho_read_bytes (void *macho, uint32_t offset, void *buffer, size_t size)
{
    macho_t *tmp = (macho_t *) macho;
    memcpy (buffer, (tmp->data + offset), size);
}

void *
macho_get_bytes (void *macho, uint32_t offset)
{
    macho_t *tmp = (macho_t *) macho;
    return (void *) (tmp->data + offset);
}