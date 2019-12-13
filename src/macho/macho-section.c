/**
 *     libhelper
 *     Copyright (C) 2019, @h3adsh0tzz
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include "libhelper-macho/macho-section.h"


/**
 * 
 */
mach_section_64_t *mach_section_create ()
{
    mach_section_64_t *s = malloc(sizeof(mach_section_64_t));
    memset (s, '\0', sizeof(mach_section_64_t));
    return s;
}


/**
 * 
 */
mach_section_64_t *mach_section_load (file_t *file, off_t offset)
{
    mach_section_64_t *s = NULL; //mach_section_create ();
    s = (mach_section_64_t *) file_load_bytes (file, sizeof(mach_section_64_t), offset);

    if (!s) {
        debugf ("[*] Error: Problem loading section at offset 0x%llx\n", offset);
        exit (0);
    }

    return s;
}


/**
 * 
 */
HSList *mach_sections_load_from_segment (macho_t *macho, mach_segment_command_64_t *seg)
{
    HSList *ret = NULL;
    uint64_t offset = seg->vmaddr;
    
    for (int i = 0; i < (int) seg->nsects; i++) {
        debugf ("Loading %lu bytes from 0x%llx\n", sizeof(mach_section_64_t), offset);
        mach_section_64_t *sect = (mach_section_64_t *) file_load_bytes (macho->file, sizeof(mach_section_64_t), offset);
        ret = h_slist_append (ret, sect);

        offset += sizeof(mach_section_64_t);
    }

    return ret;
}


/**
 * 
 */
void mach_section_print (mach_section_64_t *section)
{
    debugf ("Section:\t%s\n", section->sectname);
    debugf ("Segment:\t%s\n", section->segname);
    debugf ("Address:\t0x%llx\n", section->addr);
    debugf ("Size:\t\t%llu\n", section->size);
    debugf ("Offset:\t\t0x%x\n", section->offset);
    debugf ("Align:\t\t%u\n", section->align);
    debugf ("Reloff:\t\t0x%x\n", section->reloff);
    debugf ("Nreloc:\t\t%u\n", section->nreloc);
    debugf ("Flags:\t\t%u\n\n", section->flags);
}