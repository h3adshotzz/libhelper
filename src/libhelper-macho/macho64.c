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

/* 64-bit Specific Mach-O parsing */

macho_t *
macho_64_create_from_buffer (unsigned char *data)
{
    /* Create and allocate a macho_t */
    macho_t *macho = calloc (1, sizeof(macho_t));

    /* Set initial data */
    macho->data = (uint8_t *) data;
    macho->offset = 0;

    /* Try to load the mach header */
    macho->header = mach_header_load (macho);
    if (macho->header == NULL) {
        errorf ("macho_64_create_from_buffer: Header is NULL.\n");
        free (macho);
        return NULL;
    }

    uint32_t offset = sizeof (mach_header_t);
    HSList *scmds = NULL, *lcmds = NULL, *dylibs = NULL;

    /* Search through and parse every load command */
    for (int i = 0; i < (int) macho->header->ncmds; i++) {
        
    }
}