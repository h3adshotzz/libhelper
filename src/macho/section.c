/**
 * 
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

#include "macho.h"

mach_section_64_t *mach_section_create ()
{
    mach_section_64_t *s = malloc(sizeof(mach_section_64_t));
    memset (s, '\0', sizeof(mach_section_64_t));
    return s;
}

mach_section_64_t *mach_section_load (file_t *file, off_t offset)
{
    mach_section_64_t *s = NULL; //mach_section_create ();
    s = (mach_section_64_t *) file_load_bytes (file, sizeof(mach_section_64_t), offset);

    if (!s) {
        g_print ("[*] Error: Problem loading section at offset 0x%llx\n", offset);
        exit (0);
    }

    return s;
}

GSList *mach_sections_load_from_segment (macho_t *macho, mach_segment_command_64_t *seg)
{
    GSList *ret = NULL;
    uint64_t offset = seg->vmaddr;
    
    for (int i = 0; i < (int) seg->nsects; i++) {
        g_print ("Loading %lu bytes from 0x%llx\n", sizeof(mach_section_64_t), offset);
        mach_section_64_t *sect = (mach_section_64_t *) file_load_bytes (macho->file, sizeof(mach_section_64_t), offset);
        ret = g_slist_append (ret, sect);

        offset += sizeof(mach_section_64_t);
    }

    return ret;
}

void mach_section_dump (mach_section_64_t *section)
{
    g_print ("Section:\t%s\n", section->sectname);
    g_print ("Segment:\t%s\n", section->segname);
    g_print ("Address:\t0x%llx\n", section->addr);
    g_print ("Size:\t\t%llu\n", section->size);
    g_print ("Offset:\t\t0x%x\n", section->offset);
    g_print ("Align:\t\t%d\n", section->align);
    g_print ("Reloff:\t\t0x%x\n", section->reloff);
    g_print ("Nreloc:\t\t%d\n", section->nreloc);
    g_print ("Flags:\t\t%d\n\n", section->flags);
}