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
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-core: file api */

#ifndef __LIBHELPER_CORE_FILE_H__
#define __LIBHELPER_CORE_FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/mman.h>

/******************************************************************************
* Libhelper File API.
* 
* Libhelper API for File interactions, reading and writing files, get, read and
* copy certain bytes, etc.
* 
*******************************************************************************/

/**
 *  \brief      Libhelper File API Structure.
 * 
 *  Libhelper File API structure, contains the files original load path, size and
 *  data pointer. The files are mmaped() to at this pointer.
 */
struct _libhelper_file {
    char            *path;      /* original file path */
    size_t           size;      /* file size */
    unsigned char   *data;      /* mmaped() file */
};
typedef struct _libhelper_file      file_t;

/**
 *  Result flags for file writes.
 */
#define LH_FILE_FAILURE            -1
#define LH_FILE_SUCCESS             1

/**
 *  \brief      File API Create.
 * 
 *  Create and allocate an API file_t structure to be used for reading or writing
 *  a file.
 * 
 *  \returns    Allocated file_t structure pointer.
 */
extern file_t *
file_create ();

/**
 *  \brief      File API Load.
 * 
 *  Load a file from a provided path into a file_t structure, and return the pointer
 *  to that file struct.
 * 
 *  \param  path     File path.
 *  
 *  \returns    Loaded file_t structure pointer.
 */
extern file_t *
file_load (const char *path);

/**
 *  \brief      File API Close
 * 
 *  Close and free a file_t structure.
 * 
 *  \param  file    File to close.
 */
extern void
file_close (file_t *file);

/**
 *  \brief      File API Get Data.
 * 
 *  Get a pointer to the data in a given file at a given offset. This data cannot
 *  be modified, as it's a pointer into the file_t struct.
 * 
 *  \param  file    File to use.
 *  \param  offset  Offset to get pointer to.
 * 
 *  \returns    Pointer to the data at (file->data + offset)
 */
extern const void *
file_get_data (file_t       *file,
               uint32_t      offset);

/**
 *  \brief      File API Read Data.
 * 
 *  Read data from a given file at a given offset into a provided buffer. This API
 *  call does NOT allocate memory for the buffer, and should be done by the caller.
 * 
 *  \param  file    File to use.
 *  \param  offset  Offset to copy from.
 *  \param  buf     Buffer to copy into.
 *  \param  size    Amount of bytes to copy from `offset`.
 */
extern void
file_read_data (file_t      *file,
                uint32_t     offset,
                void        *buf,
                size_t       size);

/**
 *  \brief      File API Duplicate Data.
 * 
 *  Duplicate data from a given pointer into a buffer and return it's pointer.
 * 
 *  \param  file    File to use.
 *  \param  offset  Offset to copy from.
 *  \param  size    Amount of bytes to copy from `offset`.
 * 
 *  \returns    Pointer to duplicated data.
 */
extern void *
file_dup_data (file_t       *file,
               uint32_t      offset,
               size_t        size);

/**
 *  \brief      File API Write.
 * 
 *  Write a given file structure to the filesystem using the path within the file type.
 * 
 *  \param  file    File to use.
 * 
 */
extern void
file_write_new (file_t *file);

/**
 *  \brief      File API Create with Data.
 * 
 *  Creates a new file with given name, data and size.
 * 
 *  \param  name    File name/path,
 *  \param  data    Data to write.
 *  \param  size    Amount of bytes to copy from `data`.
 * 
 *  \returns    New file structure.
 */
extern file_t *
file_create_with_data (char *name, unsigned char *data, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* __libhelper_core_file_h__ */