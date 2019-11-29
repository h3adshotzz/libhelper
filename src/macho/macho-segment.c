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

#include "macho/macho-segment.h"

//////////////////////////////////////////////////////////////////////////
//               Base Mach-O Segment commands                           //
//////////////////////////////////////////////////////////////////////////


/**
 * 
 */
mach_segment_command_64_t *mach_segment_command_create ()
{
    mach_segment_command_64_t *sc = malloc (sizeof(mach_segment_command_64_t));
    memset (sc, '\0', sizeof(mach_segment_command_64_t));
    return sc;
}


/**
 * 
 */
mach_segment_command_64_t *mach_segment_command_load (file_t *file, off_t offset)
{
    mach_segment_command_64_t *sc = mach_segment_command_create ();
    sc = (mach_segment_command_64_t *) file_load_bytes (file, sizeof(mach_segment_command_64_t), offset);

    if (!sc) {
        g_print ("[*] Error: Problem loading Mach Segment Command at offset 0x%llx\n", offset);
        exit (0);
    } 

    return sc;
}


/**
 * 
 */
mach_segment_info_t *mach_segment_info_create ()
{
    mach_segment_info_t *si = malloc (sizeof (mach_segment_info_t));
    memset (si, '\0', sizeof(mach_segment_info_t));
    return si;
}


/**
 * 
 */
mach_segment_info_t *mach_segment_info_load (file_t *file, off_t offset)
{
    // Create a new segment info struct and load the segment command from file
    mach_segment_info_t *si = mach_segment_info_create ();
    mach_segment_command_64_t *segment = (mach_segment_command_64_t *) file_load_bytes (file, sizeof(mach_segment_command_64_t), offset);

    // Check that a segment was correctly loading from the offset at the file
    if (!segment) {
        g_print ("[*] Error: Could not load Segment (64) at offset 0x%llx\n", offset);
        exit (0);
    }

    // Fiddle with the vmaddr to ensure non-kernel images are loading correctly
    if (segment->vmaddr < 0xffffff0000000000) {
        uint64_t vmaddr = 0xffffff0000000010 + segment->vmaddr;
        segment->vmaddr = vmaddr;
    }

    // Calculate the offset and start loading section commands
    offset = segment->vmaddr;
    for (int i = 0; i < (int) segment->nsects; i++) {

        // Load a new section 64.
        mach_section_64_t *sect = mach_section_load (file, offset);

        // Append the section to the sections list of the segment info struct
        si->sections = g_slist_append (si->sections, sect);

        // Increment the offset
        offset += sizeof (mach_section_64_t);
    }

    // Set the segment command for the info struct
    si->segcmd = segment;

    // Return that segment info
    return si;
}


/**
 * 
 */
mach_segment_info_t *mach_segment_command_search (macho_t *mach, char *segname)
{
    // Check the segname given is valid
    if (!segname) {
        g_print ("[*] Segment name not valid\n");
        exit (0);
    }

    // Get the amount of segment commands and check its more than 0
    int c = g_slist_length (mach->scmds);
    if (!c) {
        g_print ("[*] Error: No Segment Commands\n");
        exit (0);
    }

    // Now go through each of them
    for (int i = 0; i < c; i++) {
        
        // Grab the segment info
        mach_segment_info_t *si = (mach_segment_info_t *) g_slist_nth_data (mach->scmds, i);
        mach_segment_command_64_t *s = si->segcmd;

        // Check if they match
        if (!strcmp(s->segname, segname)) {
            return si;
        }
    }

    // Output an error
    g_print ("[*] Could not find Segment %s\n", segname);
    return NULL;
}


/**
 * 
 */
GSList *mach_segment_get_list (macho_t *mach)
{
    // Create a new list, this'll be returned
    GSList *r = NULL;

    // Go through all of them, add them to the list
    for (int i = 0; i < (int) g_slist_length (mach->scmds); i++) {
        
        // Load the segment from the info struct, and add it to the list
        mach_segment_info_t *si = (mach_segment_info_t *) g_slist_nth_data (mach->scmds, i);
        mach_segment_command_64_t *s = (mach_segment_command_64_t *) si->segcmd;

        // Add to the list
        r = g_slist_append (r, s);
    }

    // Return the list
    return r;
}


/**
 * 
 */
mach_segment_info_t *mach_segment_find_with_name (macho_t *macho, char *segname)
{
    for (int i = 0; i < g_slist_length (macho->scmds); i++) {
        mach_segment_info_t *inf = (mach_segment_info_t *) g_slist_nth_data (macho->scmds, i);
        mach_segment_command_64_t *segment = inf->segcmd;

        if (!strcmp(segment->segname, segname)) {
            return inf;
        }
    }
    return NULL;
}


/**
 * 
 */
void mach_segment_command_dump (mach_segment_info_t *si)
{
    mach_segment_command_64_t *sc = si->segcmd;

    if (!sc) {
        g_print ("[*] Error: Segment command not loaded properly!\n");
        exit (0);
    }

    g_print ("Command:\t\t%s\n", mach_load_command_get_string ((mach_load_command_t *) sc));
    g_print ("Segment Name:\t\t%s\n", sc->segname);
    g_print ("VM Address:\t\t0x%llx\n", sc->vmaddr);
    g_print ("VM Size:\t\t%llu\n", sc->vmsize);
    g_print ("File Offset:\t\t0x%llx\n", sc->fileoff);
    g_print ("File Size:\t\t%llu\n", sc->filesize);
    g_print ("Max VM Protection:\t%d\n", sc->maxprot);
    g_print ("Initial VM Protection:\t%d\n", sc->initprot);
    g_print ("Number of Sections:\t%d\n", sc->nsects);
    g_print ("Flags:\t\t\t%d\n\n", sc->flags);
}


//////////////////////////////////////////////////////////////////////////
//               Base Mach-O Section commands                           //
//////////////////////////////////////////////////////////////////////////


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
        g_print ("[*] Error: Problem loading section at offset 0x%llx\n", offset);
        exit (0);
    }

    return s;
}


/**
 * 
 */
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


/**
 * 
 */
mach_section_64_t *mach_search_section (mach_segment_info_t *info, char *sectname)
{
    // Check the sectname given is valid
    if (!sectname || strlen(sectname) > 16) {
        g_print ("[*] Section name not valid\n");
        exit (0);
    }

    // Check the length of the sections
    int c = g_slist_length (info->sections);
    if (!c) {
        g_print ("[*] Error: No Sections\n");
        exit (0);
    }

    // Go through each of them, look for `sectname`
    for (int i = 0; i < c; i++) {
        mach_section_64_t *tmp = (mach_section_64_t *) g_slist_nth_data (info->sections, i);
        if (!strcmp(tmp->sectname, sectname)) return tmp;
    }

    return NULL;
}


/**
 * 
 */
mach_section_64_t *mach_find_section (macho_t *macho, int sect)
{
    GSList *segments = macho->scmds;
    int count = 0;
    for (int i = 0; i < g_slist_length (segments); i++) {
        mach_segment_info_t *seg = (mach_segment_info_t *) g_slist_nth_data (segments, i);
        for (int k = 0; k < seg->segcmd->nsects; k++) {
            count++;
            if (count == sect) {
                return (mach_section_64_t *) g_slist_nth_data (seg->sections, k);
            }
        }
    }
    return NULL;
}


/**
 * 
 */
void mach_section_print (mach_section_64_t *section)
{
    g_print ("Section:\t%s\n", section->sectname);
    g_print ("Segment:\t%s\n", section->segname);
    g_print ("Address:\t0x%llx\n", section->addr);
    g_print ("Size:\t\t%llu\n", section->size);
    g_print ("Offset:\t\t0x%x\n", section->offset);
    g_print ("Align:\t\t%u\n", section->align);
    g_print ("Reloff:\t\t0x%x\n", section->reloff);
    g_print ("Nreloc:\t\t%u\n", section->nreloc);
    g_print ("Flags:\t\t%u\n", section->flags);
    g_print ("Indr Sym Index:\t%d\n", section->reserved1);
    g_print ("Reserved 2:\t%d\n", section->reserved2);
    g_print ("Reserved 3:\t%d\n\n", section->reserved3);
}