//===--------------------------- macho_segment ------------------------===//
//
//                          Libhelper Mach-O Parser
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
//
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//  me@h3adsh0tzz.com.
//
//
//===------------------------------------------------------------------===//

#include "libhelper-macho-ll/macho-segment.h"
#include "libhelper-macho-ll/macho.h"


//===-----------------------------------------------------------------------===//
/*-- Mach-O Segments                     									 --*/
//===-----------------------------------------------------------------------===//


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
mach_segment_command_64_t *mach_segment_command_load (unsigned char *data, uint32_t offset)
{
    mach_segment_command_64_t *sc = mach_segment_command_create ();
    memset (sc, '\0', sizeof (mach_segment_command_64_t));
    memcpy (sc, data + offset, sizeof (mach_segment_command_64_t));

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
    mach_segment_info_t *ret = malloc (sizeof (mach_segment_info_t));
    memset (ret, '\0', sizeof (mach_segment_info_t));
    return ret;
}


/**
 * 
 */
mach_segment_info_t *mach_segment_info_load (unsigned char *data, uint32_t offset, uint32_t test)
{
    // Create a new segment info struct and load the segment command
    mach_segment_info_t *seg_inf = mach_segment_info_create ();
    mach_segment_command_64_t *segment = mach_segment_command_load (data, offset);

    // Check that the segment cmmmand is valid
    if (!segment) {
        errorf ("Could not load Segment Command\n");
        return NULL;
    }

    //  the section commands are placed directly after the segment command.
    uint32_t sectoff = offset + sizeof (mach_segment_command_64_t);
    for (int i = 0; i < (int) segment->nsects; i++) {

        // Load a section 64
        mach_section_64_t *sect = mach_section_load (data, sectoff);
        
        debugf ("%s.%s\t0x%09llx (%d bytes)\n", sect->segname, sect->sectname, sect->addr, sect->size);

        seg_inf->sections = h_slist_append (seg_inf->sections, sect);
        sectoff += sizeof (mach_section_64_t);
    }

    seg_inf->segcmd = segment;
    return seg_inf;
}


/**
 * 
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
mach_segment_info_t *mach_segment_command_search (HSList *segments, char *segname)
{
    // Check the segname given is valid
    if (!segname) {
        debugf ("[*] Segment name not valid\n");
        exit (0);
    }

    // Get the amount of segment commands and check its more than 0
    int c = h_slist_length (segments);
    if (!c) {
        debugf ("[*] Error: No Segment Commands\n");
        exit (0);
    }

    // Now go through each of them
    for (int i = 0; i < c; i++) {
        
        // Grab the segment info
        mach_segment_info_t *si = (mach_segment_info_t *) h_slist_nth_data (segments, i);
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


//===-----------------------------------------------------------------------===//
/*-- Mach-O Sections                     									 --*/
//===-----------------------------------------------------------------------===//


/**
 * 
 */
mach_section_64_t *mach_section_create ()
{
    mach_section_64_t *ret = malloc (sizeof(mach_section_64_t));
    memset (ret, '\0', sizeof(mach_section_64_t));
    return ret;
}


/**
 * 
 */
mach_section_64_t *mach_section_load (unsigned char *data, uint32_t offset)
{
    mach_section_64_t *sect = mach_section_create ();
    memcpy (sect, data + offset, sizeof (mach_section_64_t));

    if (sect == NULL) {
        errorf ("There was a problme loading the section at offset 0x%x\n");
    }
    return sect;
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
mach_section_64_t *mach_find_section (HSList *segments, int sect)
{
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

    mach_segment_info_t *seginfo = mach_segment_command_search (macho->scmds, segment);
    mach_section_64_t *__sect = mach_search_section (seginfo, section);

    ret->segment = __sect->segname;
    ret->section = __sect->sectname;
    ret->size = __sect->size;

    ret->data = malloc (__sect->size);
    memset (ret->data, '\0', __sect->size);
    memcpy (ret->data, macho->data + __sect->offset, __sect->size);

    return ret;
}