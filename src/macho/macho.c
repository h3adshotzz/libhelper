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

#include "libhelper-macho/macho.h"
#include "libhelper-macho/macho-command.h"
#include "libhelper-macho/macho-segment.h"

/**
 *  Function:   macho_create
 *  ------------------------------------
 * 
 *  Creates a new Mach-O  structure and assigns sufficient memory. Should be 
 *  called to safely create a new Mach-o structure.
 * 
 *  returns:    A macho_t structure with sufficient allocated memory.
 * 
 */
macho_t *macho_create ()
{
    macho_t *macho = malloc(sizeof(macho_t));
    memset (macho, '\0', sizeof(macho_t));
    return macho;
}


/**
 *  Function:   macho_load
 *  ------------------------------------
 * 
 *  Creates a new Mach-O  structure and assigns sufficient memory. Should be 
 *  called to safely create a new Mach-o structure.
 * 
 *  returns:    A macho_t structure with sufficient allocated memory.
 * 
 */
macho_t *macho_load (file_t *file)
{
    // Create and allocate memory for a new macho_t struct.
    macho_t *mach = macho_create ();

    // Set the file for which we will pull our data from.
    mach->file = file;

    // Set the header, pulled from mach->file.
    mach->header = mach_header_load (mach->file);
    
    /**
     *  The first major chunk of data we will pull from mach->file is the
     *  Load Commands. They will be be split into two GSLists, cmdlist
     *  for the wide range of Load Commands, and seglist, which contains
     *  LC_SEGMENT_XX commands, of which there are many in a single file.
     * 
     *  My method is simply using a for loop to go through each base Load
     *  Command, which contains a command type and size field only, and
     *  check the lc->cmd property. 
     *  
     *  If the lc->cmd property equals LC_SEGMENT_64, we will load a segment
     *  command from the offset of that command, otherwise it is added to
     *  the cmdlist.
     * 
     *  You may notice I use mach_command_info_t instead of mach_load_command_t.
     *  I do this because the info struct contains the offset of where the
     *  Load Command is in the file.
     * 
     *  Mach-O's can contain dynamically linked libraries, which are referenced
     *  by the LC_LOAD_DYLIB, ..., Load Commands. As there are typically more
     *  than one, they are also stored within a list.
     * 
     */
    HSList *segcmds = NULL;
    HSList *lcmds = NULL;
    HSList *dylibs = NULL;

    off_t offset = sizeof(mach_header_64_t);

    for (int i = 0; i < (int) mach->header->ncmds; i++) {

        // Create the Command Info struct
        mach_command_info_t *lc = mach_command_info_load (mach->file, offset);

        // Check if the Load Command is a LC_SEGMENT_64
        if (lc->type == LC_SEGMENT_64) {

            // Create a mach_segment_info_t struct for the Segment.
            mach_segment_info_t *seginfo = mach_segment_info_load (mach->file, offset);

            // Append the seginfo to the mach->segcmds GSList.
            segcmds = h_slist_append (segcmds, seginfo);

        } else if (lc->type == LC_ID_DYLIB || lc->type == LC_LOAD_DYLIB ||
                lc->type == LC_LOAD_WEAK_DYLIB || lc->type == LC_REEXPORT_DYLIB) {

            // Because a Mach-O can have multiple Dynamically linked libraries,
            // that means there are multiple LC_DYLIB-like commands, so it's
            // easier that we have a seperate list for them.

            // Create the mach_dylib_command_info_t struct for the dylib command
            mach_dylib_command_info_t *dylibinfo = malloc (sizeof(mach_dylib_command_info_t));

            // Load the raw dylib command, we need this to load the dylib name
            mach_dylib_command_t *raw = (mach_dylib_command_t *) file_load_bytes (mach->file, lc->lc->cmdsize, lc->off);

            // Load the name of the dylib. This is located after the Load Command
            // and is included in the cmdsize property of the Loac Command.
            char *name = file_load_bytes (mach->file, (raw->cmdsize - sizeof(mach_dylib_command_t)), lc->off + raw->dylib.offset);

            // Set the name, raw cmd struct, and type of the dylib
            dylibinfo->name = name;
            dylibinfo->dylib = raw;
            dylibinfo->type = lc->type;

            // Add it to the list
            dylibs = h_slist_append (dylibs, dylibinfo);
            lcmds = h_slist_append (lcmds, lc);
        }
        
        else {

            // Set the offset so we can find the command again in the file later
            lc->off = offset;

            // Append the Load Command to the mach->lcmds GSList.
            lcmds = h_slist_append (lcmds, lc);
        }

        // Increment the offset by the size of the Load Command
        offset += lc->lc->cmdsize;
    }

    mach->lcmds = lcmds;
    mach->scmds = segcmds;
    mach->dylibs = dylibs;

    return mach;
}