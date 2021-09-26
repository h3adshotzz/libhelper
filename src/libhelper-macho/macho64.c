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
        mach_load_command_info_t *lc_inf = mach_load_command_info_load ((const char *) macho->data, offset);
        uint32_t type = lc_inf->lc->cmd;

        /**
         *  Load commands a treated as three groups: segment commands, library commands and
         *  regular load commands.
         * 
         *   - Segment commands describe the code segments & sections of the binary.
         *   - Library commands describe dynamically-linked libraries.
         *   - Regular Load commands are all other load commands.
         */
        if (type == LC_SEGMENT_64) {

            /* Parse the LC as a segment command info */


        } else if (type == LC_ID_DYLIB || type == LC_LOAD_DYLIB ||
                   type == LC_LOAD_WEAK_DYLIB || type == LC_REEXPORT_DYLIB) {

            /* */
        } else { 
            
            /* Add any other LCs to the list */
            lc_inf->offset = offset;
            lcmds = h_slist_append (lcmds, lc_inf);
        }
        
        printf ("lc_type[%d]: %s\n", i, mach_load_command_get_name (lc_inf->lc));
        offset += lc_inf->lc->cmdsize;
    }

    /* Set the data start offset */
    macho->offset = offset;

    /* Set the LC lists */
    macho->lcmds = lcmds;
    macho->scmds = scmds;
    macho->dylibs = dylibs;

    /* Fix the size */
    mach_segment_info_t *last_seg = (mach_segment_info_t *) h_slist_last (macho->scmds);
    macho->size = last_seg->segcmd->fileoff + last_seg->segcmd->filesize;

    return macho;
}