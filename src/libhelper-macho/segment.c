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

mach_segment_info_t *
mach_segment_info_search (HSList *segments, char *segname)
{
    /* check the segname is valid */
    if (!segname) {
        errorf ("mach_segment_info_search: segment name not valid\n");
        return NULL;
    }

    /* check the amount of segments is > 0 */
    int c = h_slist_length (segments);
    if (!c) {
        errorf ("mach_segment_info_search: invalid segment list\n");
        return NULL;
    }

    /* check each segment */
    for (int i = 0; i < c; i++) {

        /* the segname is located 8 bytes from the base of both 32-bit and 64-bit segment structs */
        mach_segment_info_t *seg = (mach_segment_info_t *) h_slist_nth_data (segments, i);
        char *check = (char *) seg->segcmd + 8;

        /* check if the names match */
        debugf ("segname: %s, check: %s\n", check, segname);
        if (!strcmp (check, segname))
            return seg;
    }

    /* error */
    errorf ("mach_segment_info_search: could not find segment with name: %s\n", segname);
    return NULL;
}

mach_segment_command_64_t *
mach_segment_command_64_from_info (mach_segment_info_t *info)
{
    if (info->arch == LIBHELPER_ARCH_64)
        return (info->segcmd) ? (mach_segment_command_64_t *) info->segcmd : NULL;
    return NULL;
}

mach_segment_command_32_t *
mach_segment_command_32_from_info (mach_segment_info_t *info)
{
    if (info->arch == LIBHELPER_ARCH_32)
        return (info->segcmd) ? (mach_segment_command_32_t *) info->segcmd : NULL;
    return NULL;
}

char *
mach_segment_read_vm_protection (vm_prot_t prot)
{
    HString *str = h_string_new ("");

    if ((prot & VM_PROT_READ) == VM_PROT_READ) str = h_string_append_c (str, 'r');
    else str = h_string_append_c (str, '-');

    if ((prot & VM_PROT_WRITE) == VM_PROT_WRITE) str = h_string_append_c (str, 'w');
    else str = h_string_append_c (str, '-');

    if ((prot & VM_PROT_EXEC) == VM_PROT_EXEC) str = h_string_append_c (str, 'x');
    else str = h_string_append_c (str, '-');

    str = h_string_append_c (str, '\0');
    return str->str;
}
