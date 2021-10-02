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

/* 64-bit Specific Segment Section parsing */

mach_section_64_t *
mach_section_64_load (unsigned char *data, uint32_t offset)
{
    mach_section_64_t *s = (mach_section_64_t *) (data + offset);
    if (!s) {
        errorf ("mach_section_64_load: could not load segment command at offset: 0x%08x\n", offset);
        return NULL;
    }
    return s;
}

mach_section_32_t *
mach_section_32_load (unsigned char *data, uint32_t offset)
{
    mach_section_32_t *s = (mach_section_32_t *) (data + offset);
    if (!s) {
        errorf ("mach_section_64_load: could not load segment command at offset: 0x%08x\n", offset);
        return NULL;
    }
    return s;
}

mach_section_64_t *
mach_section_64_search_in_segment (mach_segment_info_t *info, char *sectname) 
{
    /* check the given section name is valid */
    if (!sectname || strlen(sectname) > 16) {
        errorf ("mach_section_64_search_in_segment: could not find section with invalid sectname.\n");
        return NULL;
    }

    /* check the length of the section list */
    int c = h_slist_length (info->sections);
    if (!c) {
        errorf ("mach_section_64_search_in_segment: section list invalid.\n");
        return NULL;
    }

    /* search for the section */
    for (int i = 0; i < c; i++) {
        mach_section_64_t *sect = (mach_section_64_t *) h_slist_nth_data (info->sections, i);
        if (!strcmp (sect->sectname, sectname)) return sect;
    }
    return NULL;
}

mach_section_32_t *
mach_section_32_search_in_segment (mach_segment_info_t *info, char *sectname) 
{
    /* check the given section name is valid */
    if (!sectname || strlen(sectname) > 16) {
        errorf ("mach_section_32_search_in_segment: could not find section with invalid sectname.\n");
        return NULL;
    }

    /* check the length of the section list */
    int c = h_slist_length (info->sections);
    if (!c) {
        errorf ("mach_section_32_search_in_segment: section list invalid.\n");
        return NULL;
    }

    /* search for the section */
    for (int i = 0; i < c; i++) {
        mach_section_32_t *sect = (mach_section_64_t *) h_slist_nth_data (info->sections, i);
        if (!strcmp (sect->sectname, sectname)) return sect;
    }
    return NULL;
}

mach_section_64_t *
mach_section_64_search (HSList *segments, char *segname, char *sectname)
{
    /* find the segment first */
    mach_segment_info_t *seg = mach_segment_info_search (segments, segname);
    if (!seg) {
        errorf ("mach_section_64_search: could not find segment: %s\n", segname);
        return NULL;
    }

    /* now look for the section */
    mach_section_64_t *sect = mach_section_64_search_in_segment (seg, sectname);

    /* check if the section is valid */
    return (sect) ? sect : NULL;
}

mach_section_32_t *
mach_section_32_search (HSList *segments, char *segname, char *sectname)
{
    /* find the segment first */
    mach_segment_info_t *seg = mach_segment_info_search (segments, segname);
    if (!seg) {
        errorf ("mach_section_64_search: could not find segment: %s\n", segname);
        return NULL;
    }

    /* now look for the section */
    mach_section_32_t *sect = mach_section_32_search_in_segment (seg, sectname);

    /* check if the section is valid */
    return (sect) ? sect : NULL;

}

void *
mach_section_find_at_index (HSList *segments, int index)
{
    void *sect = NULL;
    int count = 0;

    for (int i = 0; i < h_slist_length (segments); i++) {
        mach_segment_info_t *inf = (mach_segment_info_t *) h_slist_nth_data (segments, i);
        for (int k = 0; k < h_slist_length (inf->sections); k++) {
            count++;
            if (count == index) {
                sect = (void *) h_slist_nth_data (inf->sections, k);
                break;
            }
        }
    }
    return (sect) ? sect : NULL;
}