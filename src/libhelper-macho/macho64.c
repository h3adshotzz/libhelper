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
    HSList *scmds = NULL, *lcmds = NULL, *dylibs = NULL, *fileset = NULL;
    uint32_t offset = sizeof (mach_header_t);

    /* parse and load each load command */
    for (int i = 0; i < macho->header->ncmds; i++) {

        /* parse the basic load command first, then check it's type */
        mach_load_command_t *lc = (mach_load_command_t *) (macho->data + offset);
        mach_load_command_info_t *inf = calloc (1, sizeof (mach_load_command_info_t));
        inf->lc = lc;
        inf->offset = offset;

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

        } else if (lc->cmd == LC_ID_DYLIB || lc->cmd == LC_LOAD_DYLIB ||
                   lc->cmd == LC_LOAD_WEAK_DYLIB || lc->cmd == LC_REEXPORT_DYLIB) {

            /**
             *  NOTE:   Because a Mach-O can have multiple dynamically linked libraries
             *          which means there are multiple LC_DYLIB-like load commands, it is
             *          easier to store them all in a separate list.
             */

            /* create a dylib info struct */
            mach_dylib_command_info_t *dylib_inf = calloc (1, sizeof (mach_dylib_command_info_t));
            uint32_t cmdsize = lc->cmdsize;

            /* create the raw dylib command */
            mach_dylib_command_t *raw = calloc (1, sizeof (mach_dylib_command_t));
            memcpy (raw, macho->data + offset, sizeof (mach_dylib_command_t));

            /* load the name of the dylib. This is located after the load commnad and is
             *  included in the cmdsize
             */
            char *name = mach_load_command_load_string (macho, cmdsize, sizeof (mach_dylib_command_t), offset, raw->dylib.name.offset);

            /* set the name, raw cmd struct and type */
            dylib_inf->name = name;
            dylib_inf->dylib = raw;
            dylib_inf->type = lc->cmd;

            /* add them both to the lists */
            dylibs = h_slist_append (dylibs, dylib_inf);
            lcmds = h_slist_append (lcmds, inf);

        } else if (lc->cmd == LC_FILESET_ENTRY) {

            /**
             *
             */
            mach_fileset_entry_command_t *entry_cmd = (mach_fileset_entry_command_t *) (macho->data + offset);
            mach_fileset_entry_info_t *entry_info = calloc (1, sizeof (mach_fileset_entry_info_t));
            macho_t *entry_macho = macho_64_create_from_buffer (macho->data + entry_cmd->fileoff);

            if (entry_macho) {
                entry_info->cmd = entry_cmd;
                entry_info->macho = entry_macho;
                entry_info->offset = offset;
                entry_info->entry_id = mach_load_command_load_string (macho, entry_cmd->cmdsize, sizeof (mach_fileset_entry_command_t), offset, entry_cmd->entry_id.offset);

                fileset = h_slist_append (fileset, entry_info);
            }
            lcmds = h_slist_append (lcmds, inf);

        } else {
            lcmds = h_slist_append (lcmds, inf);
        }

        offset += lc->cmdsize;
    }

    /* set macho offset */
    macho->offset = offset;

    /* set command lists */
    macho->lcmds = lcmds;
    macho->scmds = scmds;
    macho->dylibs = dylibs;
    macho->fileset = fileset;

    /* fix size */
    mach_segment_info_t *last_seg = (mach_segment_info_t *) h_slist_last (macho->scmds);
    if (last_seg->arch == LIBHELPER_ARCH_64) {
        mach_segment_command_64_t *segcmd = last_seg->segcmd;
        if (segcmd->filesize == 0) {
            last_seg = (mach_segment_info_t *) h_slist_nth_data (macho->scmds, h_slist_length (macho->scmds) - 1);
            segcmd = last_seg->segcmd;
        }

        macho->size = segcmd->fileoff + segcmd->filesize;
    } else {
        mach_segment_command_32_t *segcmd = last_seg->segcmd;
        macho->size = segcmd->fileoff + segcmd->filesize;
    }

    return macho;
}
