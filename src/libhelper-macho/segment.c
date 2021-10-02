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

/* 64-bit Specific Segment Load Command parsing */

mach_segment_command_64_t *
mach_segment_command_64_load (unsigned char *data, uint32_t offset)
{
    mach_segment_command_64_t *sc = (mach_segment_command_64_t *) (data + offset);
    if (!sc) {
        errorf ("mach_segment_command_64_load: could not load segment command at offset: 0x%08x\n", offset);
        return NULL;
    }
    return sc;
}

mach_segment_command_32_t *
mach_segment_command_32_load (unsigned char *data, uint32_t offset)
{
    mach_segment_command_32_t *sc = (mach_segment_command_32_t *) (data + offset);
    if (!sc) {
        errorf ("mach_segment_command_32_load: could not load segment command at offset: 0x%08x\n", offset);
        return NULL;
    }
    return sc;
}

mach_segment_info_t *
mach_segment_info_load (unsigned char *data, uint32_t offset)
{
    mach_segment_info_t *inf = calloc (1, sizeof (mach_segment_info_t));

    /* check the architecture */
    mach_load_command_t *lc = (mach_load_command_t *) (data + offset);
    if (lc->cmd == LC_SEGMENT_64) {

        /* handle a 64-bit segment */
        mach_segment_command_64_t *seg = mach_segment_command_64_load (data, offset);

        /* check seg was loaded properly */
        if (!seg) goto segment_load_error;

        /* assign other info properties */
        inf->arch = LIBHELPER_ARCH_64;
        inf->offset = offset;
        inf->segcmd = seg;

        /* parse sections */
        uint32_t soff = offset + sizeof (mach_segment_command_64_t);
        for (int i = 0; i < (int) seg->nsects; i++) {
            mach_section_64_t *sect = mach_section_64_load (data, soff);

            inf->sections = h_slist_append (inf->sections, sect);
            soff += sizeof (mach_section_64_t);
        }

    } else if (lc->cmd == LC_SEGMENT) {

        /* handle a 32-bit segment */
        mach_segment_command_32_t *seg = mach_segment_command_32_load (data, offset);

        /* check seg was loaded properly */
        if (!seg) goto segment_load_error;

        /* assign other info properties */
        inf->arch = LIBHELPER_ARCH_32;
        inf->offset = offset;
        inf->segcmd = seg;

        /* parse sections */
        uint32_t soff = offset + sizeof (mach_segment_command_32_t);
        for (int i = 0; i < (int) seg->nsects; i++) {
            mach_section_32_t *sect = mach_section_32_load (data, offset);

            inf->sections = h_slist_append (inf->sections, sect);
            soff += sizeof (mach_section_32_t);
        }

    } else {
        goto segment_load_error;
    }

    /* return the segment info */
    return inf;

segment_load_error:
    errorf ("mach_segment_info_load: could not load segment at offset: 0x%08x\n", offset);
    return NULL;
}