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

/* 64-bit Specific Mach-O parsing */

macho_t *
macho_64_create_from_buffer (unsigned char *data)
{
    /* create and allocate a macho_t */
    macho_t *macho = calloc (1, sizeof (macho_t));

    /* set initial data */
    macho->data = (uint8_t *) data;
    macho->offset = 0;

    /* try to load the mach header */
    macho->header = mach_header_load (macho);
    if (macho->header == NULL) {
        errorf ("macho_64_create_from_buffer: Header is NULL.\n");
        free (macho);
        return NULL;
    }

    /* create lists for load commands */
    HSList *scmds = NULL, *lcmds = NULL, *dylibs = NULL;
    uint32_t offset = sizeof (mach_header_t);

    /* parse and load each load command */
    for (int i = 0; i < macho->header->ncmds; i++) {

        /* parse the basic load command first, then check it's type */
        mach_load_command_t *lc = (mach_load_command_t *) (macho->data + offset);
        
        /**
         *  NOTE:   Different types of Load Command are sorted into one of the three
         *          defined lists: scmds, lcmds or dylibs.
         */
        if (lc->cmd == LC_SEGMENT_64) {

            /* create segment info struct */
            mach_segment_info_t *seginf = mach_segment_info_load ((unsigned char *) macho->data, offset);
            if (seginf == NULL) {
                warningf ("macho_create_from_buffer(): failed to load LC_SEGMENT_64 at offset: 0x%08x\n", offset);
                continue;
            }

            /* add to segments list */
            scmds = h_slist_append (scmds, seginf);
        } else {

            /* any other commands are stored as load command info structs */
            mach_load_command_info_t *inf = calloc (1, sizeof (mach_load_command_info_t));
            inf->lc = lc;
            inf->offset = offset;

            lcmds = h_slist_append (lcmds, inf);        
        }

        debugf ("lc_type[%d]: %s\n", i, mach_load_command_get_name (lc));
        offset += lc->cmdsize;
    }

    /* set macho offset */
    macho->offset = offset;

    /* set command lists */
    macho->lcmds = lcmds;
    macho->scmds = scmds;
    macho->dylibs = dylibs;

    /* fix size */
    mach_segment_info_t *last_seg = (mach_segment_info_t *) h_slist_last (macho->scmds);
    if (last_seg->arch == LIBHELPER_ARCH_64) {
        mach_segment_command_64_t *segcmd = last_seg->segcmd;
        macho->size = segcmd->fileoff + segcmd->filesize;
    } else {
        mach_segment_command_32_t *segcmd = last_seg->segcmd;
        macho->size = segcmd->fileoff + segcmd->filesize;
    }
   
    return macho;
}