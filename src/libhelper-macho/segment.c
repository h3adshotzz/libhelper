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

mach_segment_info_64_t *
mach_segment_info_64_load (unsigned char *data, uint32_t offset)
{
    mach_segment_info_64_t *seg_inf = calloc (1, sizeof (mach_segment_info_64_t));
    mach_segment_command_64_t *seg = mach_segment_command_64_load (data, offset);

    /* check the segment command was parsed correctly */
    if (!seg) {
        errorf ("mach_segment_info_load(): Could not load Segment Command at offset: 0x%08x\n", offset);
        return NULL;
    }

    uint32_t sectoff = offset + sizeof (mach_segment_command_64_t);
    for (uint32_t i = 0; i < seg->nsects; i++) {
        mach_section_64_t *sect = mach_section_64_load (data, sectoff);
        seg_inf->sections = h_slist_append (seg_inf->sections, sect);
        sectoff += sizeof (mach_section_64_t);
    }

    /* set the segment info */
    seg_inf->offset = offset;
    seg_inf->segcmd = seg;
    return seg_inf;
}