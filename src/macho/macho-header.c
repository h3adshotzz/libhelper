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


/**
 *  [Note]
 *      All documentation is contained within the header file, however comments
 *      will still be made to code within this file.
 */
#include "macho/macho-header.h"


/**
 *  Function:   mach_header_create
 *  ------------------------------------
 * 
 *  Creates a new Mach-O Header structure and assigns sufficient memory. 
 *  Should be called to safely create a new Mach-O Header structure.
 * 
 *  Returns:    A mach_header_t structure with sufficient allocated memory.
 */
mach_header_t *mach_header_create ()
{
    mach_header_t *header = malloc(sizeof(mach_header_t));
    memset (header, '\0', sizeof (mach_header_t));
    return header;
}


/**
 *  Function:   mach_header_load
 *  ----------------------------------
 * 
 *  Loads a raw Mach-O Header from a given offset in a verified Mach-O file, and
 *  returns the resulting structure.
 *  
 *  file:       The verified Mach-O file.
 * 
 *  Returns:    A verified Mach Header structure.
 */
mach_header_t *mach_header_load (file_t *file)
{
    // Load bytes from the file to read the header
    mach_header_t *header = mach_header_create ();
    header = (mach_header_t *) file_load_bytes (file, MACH_HEADER_SIZE, 0);

    // Check that the header has a magic value
    if (!header->magic) {
        debugf ("[*] Error: Header was not loaded correctly. Aborting!\n");
        exit (0);
    }

    // Check the header type.
    if (header->magic == MACH_CIGAM_64 || header->magic == MACH_MAGIC_64) {
        debugf ("[*] Detected 64-bit Mach-O Binary!\n");
        return header;
    } else if (header->magic == MACH_CIGAM_32 || header->magic == MACH_MAGIC_32) {
        debugf ("[*] Detected 32-bit Mach-O Binary!\n");
        debugf ("[*] Error: Cannot handle 32-bit Binaries yet. Aborting!\n");
        exit (0);
    } else if (header->magic == MACH_CIGAM_UNIVERSAL || header->magic == MACH_MAGIC_UNIVERSAL) {
        debugf ("[*] Detected Universal Mach-O Binary!\n");
        debugf ("[*] Error: Cannot handle Universal Binaries yet. Aborting!\n");
        exit (0);
    } else {
        debugf ("[*] Error: Could not determine Mach-O type with Magic 0x%x. Aborting!\n", header->magic);
        exit (0);
    }

    return NULL;
}


/**
 *  Function:   mach_header_read_cpu_type
 *  -------------------------------------
 * 
 *  Returns a decoded string of header->cputype.
 * 
 *  type:       The cpu_type_t from the Mach-O Header.
 * 
 *  Returns:    Decoded CPU type String.
 */
char *mach_header_read_cpu_type (cpu_type_t type)
{
    char *cpu_type = "";
    switch (type) {
        case CPU_TYPE_X86:
            cpu_type = "x86";
            break;
        case CPU_TYPE_X86_64:
            cpu_type = "x86_64";
            break;
        case CPU_TYPE_ARM:
            cpu_type = "arm";
            break;
        case CPU_TYPE_ARM64:
            cpu_type = "arm64";
            break;
        case CPU_TYPE_ARM64_32:
            cpu_type = "arm64_32";
            break;
        default:
            cpu_type = "unknown";
            break;
    }
    return cpu_type;
}


/**
 *  Function:   mach_header_read_file_type
 *  -------------------------------------
 * 
 *  Returns a decoded string of header->filetype.
 * 
 *  type:       The header->filetype uint32_t from the Mach-O Header.
 * 
 *  Returns:    Decoded header->filetype.
 */
char *mach_header_read_file_type (uint32_t type)
{
    char *ret = "";
    switch (type) {
        case MACH_TYPE_OBJECT:
            ret = "Mach Object (MH_OBJECT)";
            break;
        case MACH_TYPE_EXECUTE:
            ret = "Mach Executable (MH_EXECUTE)";
            break;
        case MACH_TYPE_DYLIB:
            ret = "Mach Dynamic Library (MH_DYLIB)";
            break;
        default:
            ret = "Unknown";
            break;
    }
    return ret;
}


/**
 *  Function:   mach_header_print_summary
 *  -------------------------------------
 * 
 *  Outputs a summary of a given mach_header_t.
 * 
 *  header:     The Mach-O Header.
 * 
 */
void mach_header_print_summary (mach_header_t *header)
{
    debugf ("==================\nMach-O Header Dump\n==================\n\n");

    debugf ("Magic: \t\t0x%x\n", header->magic);
    debugf ("CPU Type: \t%s\n", mach_header_read_cpu_type(header->cputype));
    debugf ("CPU Sub-Type: \t0x%x\n", header->cpusubtype);
    debugf ("File Type: \t%s\n", mach_header_read_file_type (header->filetype));
    debugf ("Load Commands: \t%d\n", header->ncmds);
    debugf ("LC Size: \t%d\n", header->sizeofcmds);

    debugf ("------------------\n\n");
}