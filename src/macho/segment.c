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

mach_segment_command_64_t *mach_segment_command_create ()
{
    mach_segment_command_64_t *sc = malloc (sizeof(mach_segment_command_64_t));
    memset (sc, '\0', sizeof(mach_segment_command_64_t));
    return sc;
}

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


mach_segment_info_t *mach_segment_info_create ()
{
    mach_segment_info_t *si = malloc (sizeof (mach_segment_info_t));
    memset (si, '\0', sizeof(mach_segment_info_t));
    return si;
}

mach_segment_info_t *mach_segment_info_load (file_t *file, off_t offset)
{
    mach_segment_info_t *si = mach_segment_info_create ();
    si->segcmd = (mach_segment_command_64_t *) file_load_bytes (file, sizeof(mach_segment_command_64_t), offset);

    if (!si->segcmd) {
        g_print ("[*] Error: Could not load Segment at offset 0x%llx\n", offset);
    }

    offset = si->segcmd->vmaddr;
    for (int i = 0; i < (int) si->segcmd->nsects; i++) {
        mach_section_64_t *sect = mach_section_create ();
        sect = (mach_section_64_t *) file_load_bytes (file, sizeof(mach_section_64_t), offset);

        si->sections = g_slist_append (si->sections, sect);

        offset += sizeof(mach_section_64_t);
    }

    return si;
} 


mach_segment_info_t *mach_segment_command_search (macho_t *mach, char *segname)
{
    if (!segname) {
        g_print ("[*] Segment name not valid\n");
        exit (0);
    }

    int c = g_slist_length (mach->scmds);
    if (!c) {
        g_print ("[*] Error: No Segment Commands\n");
        exit (0);
    }

    for (int i = 0; i < c; i++) {
        mach_segment_info_t *si = (mach_segment_info_t *) g_slist_nth_data (mach->scmds, i);
        mach_segment_command_64_t *s = si->segcmd;
        if (!strcmp(s->segname, segname)) {
            return si;
        }
    }
    
    g_print ("[*] Could not find Segment %s\n", segname);
    return NULL;
}

GSList *mach_segment_get_list (macho_t *mach)
{
    GSList *r = NULL;
    for (int i = 0; i < (int) g_slist_length (mach->scmds); i++) {
        mach_segment_info_t *si = (mach_segment_info_t *) g_slist_nth_data (mach->scmds, i);
        mach_segment_command_64_t *s = (mach_segment_command_64_t *) si->segcmd;
        r = g_slist_append (r, s->segname);
    }
    return r;
}

void mach_segment_command_dump (mach_segment_info_t *si)
{
    mach_segment_command_64_t *sc = si->segcmd;

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