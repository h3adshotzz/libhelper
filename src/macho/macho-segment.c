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
        debugf ("[*] Error: Problem loading Mach Segment Command at offset 0x%llx\n", offset);
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
        debugf ("[*] Error: Could not load Segment (64) at offset 0x%llx\n", offset);
        exit (0);
    }

    // Fiddle with the vmaddr to ensure non-kernel images are loading correctly
    if (segment->vmaddr < 0xffffff0000000000) {
        uint64_t vmaddr = 0xffffff0000000010 + segment->vmaddr;
        segment->vmaddr = vmaddr;
        si->padding = 0xffffff0000000000;
    } else {
        si->padding = 0x0;
    }

    // Calculate the offset and start loading section commands
    offset = segment->vmaddr;
    for (int i = 0; i < (int) segment->nsects; i++) {

        // Load a new section 64.
        mach_section_64_t *sect = mach_section_load (file, offset);

        // Append the section to the sections list of the segment info struct
        si->sections = h_slist_append (si->sections, sect);

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
        debugf ("[*] Segment name not valid\n");
        exit (0);
    }

    // Get the amount of segment commands and check its more than 0
    int c = h_slist_length (mach->scmds);
    if (!c) {
        debugf ("[*] Error: No Segment Commands\n");
        exit (0);
    }

    // Now go through each of them
    for (int i = 0; i < c; i++) {
        
        // Grab the segment info
        mach_segment_info_t *si = (mach_segment_info_t *) h_slist_nth_data (mach->scmds, i);
        mach_segment_command_64_t *s = si->segcmd;

        // Check if they match
        if (!strcmp(s->segname, segname)) {
            return si;
        }
    }

    // Output an error
    debugf ("[*] Could not find Segment %s\n", segname);
    return NULL;
}


/**
 * 
 */
HSList *mach_segment_get_list (macho_t *mach)
{
    // Create a new list, this'll be returned
    HSList *r = NULL;

    // Go through all of them, add them to the list
    for (int i = 0; i < (int) h_slist_length (mach->scmds); i++) {
        
        // Load the segment from the info struct, and add it to the list
        mach_segment_info_t *si = (mach_segment_info_t *) h_slist_nth_data (mach->scmds, i);
        mach_segment_command_64_t *s = (mach_segment_command_64_t *) si->segcmd;

        // Add to the list
        r = h_slist_append (r, s);
    }

    // Return the list
    return r;
}


/**
 * 
 */
mach_segment_info_t *mach_segment_find_with_name (macho_t *macho, char *segname)
{
    for (int i = 0; i < h_slist_length (macho->scmds); i++) {
        mach_segment_info_t *inf = (mach_segment_info_t *) h_slist_nth_data (macho->scmds, i);
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
        debugf ("[*] Error: Segment command not loaded properly!\n");
        exit (0);
    }

    debugf ("Command:\t\t%s\n", mach_load_command_get_string ((mach_load_command_t *) sc));
    debugf ("Segment Name:\t\t%s\n", sc->segname);
    debugf ("VM Address:\t\t0x%llx\n", sc->vmaddr);
    debugf ("VM Size:\t\t%llu\n", sc->vmsize);
    debugf ("File Offset:\t\t0x%llx\n", sc->fileoff);
    debugf ("File Size:\t\t%llu\n", sc->filesize);
    debugf ("Max VM Protection:\t%d\n", sc->maxprot);
    debugf ("Initial VM Protection:\t%d\n", sc->initprot);
    debugf ("Number of Sections:\t%d\n", sc->nsects);
    debugf ("Flags:\t\t\t%d\n\n", sc->flags);
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
mach_section_64_t *mach_search_section (mach_segment_info_t *info, char *sectname)
{
    // Check the sectname given is valid
    if (!sectname || strlen(sectname) > 16) {
        debugf ("[*] Section name not valid\n");
        exit (0);
    }

    // Check the length of the sections
    int c = h_slist_length (info->sections);
    if (!c) {
        debugf ("[*] Error: No Sections\n");
        exit (0);
    }

    // Go through each of them, look for `sectname`
    for (int i = 0; i < c; i++) {
        mach_section_64_t *tmp = (mach_section_64_t *) h_slist_nth_data (info->sections, i);
        if (!strcmp(tmp->sectname, sectname)) return tmp;
    }

    return NULL;
}


/**
 * 
 */
mach_section_64_t *mach_find_section (macho_t *macho, int sect)
{
    HSList *segments = macho->scmds;
    int count = 0;
    for (int i = 0; i < h_slist_length (segments); i++) {
        mach_segment_info_t *seg = (mach_segment_info_t *) h_slist_nth_data (segments, i);
        for (int k = 0; k < seg->segcmd->nsects; k++) {
            count++;
            if (count == sect) {
                return (mach_section_64_t *) h_slist_nth_data (seg->sections, k);
            }
        }
    }
    return NULL;
}


/**
 * 
 */
mach_section_info_t *mach_load_section_data (macho_t *macho, char *segment, char *section)
{
    mach_section_info_t *ret = malloc (sizeof(mach_section_info_t));

    mach_segment_info_t *seginfo = mach_segment_command_search (macho, segment);
    mach_section_64_t *__sect = mach_search_section (seginfo, section);

    ret->data = file_load_bytes (macho->file, __sect->size, __sect->offset);
    ret->size = __sect->size;
    ret->sect = __sect;

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
    debugf ("Flags:\t\t%u\n", section->flags);
    debugf ("Indr Sym Index:\t%d\n", section->reserved1);
    debugf ("Reserved 2:\t%d\n", section->reserved2);
    debugf ("Reserved 3:\t%d\n\n", section->reserved3);
}