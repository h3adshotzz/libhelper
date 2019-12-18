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
#include "libhelper-macho/macho-header.h"


/**
 *  Function:   mach_header_create
 *  ------------------------------------
 * 
 *  Creates a new Mach-O Header structure and assigns sufficient memory. 
 *  Should be called to safely create a new Mach-O Header structure.
 * 
 *  Returns:    A mach_header_64_t structure with sufficient allocated memory.
 */
mach_header_64_t *mach_header_create ()
{
    mach_header_64_t *header = malloc(sizeof(mach_header_64_t));
    memset (header, '\0', sizeof (mach_header_64_t));
    return header;
}


/**
 *  Function:   mach_header_verify
 *  ----------------------------------
 * 
 *  Verifies the magic number of a given file, then returns a flag so the caller
 *  can proceed to either load a Mach-O or parse a FAT header.
 *  
 *  file:       The verified Mach-O file.
 * 
 *  Returns:    A header type flag.
 */
mach_header_type_t *mach_header_verify (file_t *file)
{
    // Assume that it is a fat header as its a smaller structure.
    fat_header_t *fat = (fat_header_t *) file_load_bytes (file, sizeof (uint32_t), 0);
    uint32_t magic = OSSwapInt32(fat->magic);
    if (magic == MACH_CIGAM_64 || magic == MACH_MAGIC_64) {
        return MH_TYPE_MACHO64;
    } else if (magic == MACH_CIGAM_32 || magic == MACH_MAGIC_32) {
        return MH_TYPE_MACHO32;
    } else if (magic == MACH_MAGIC_UNIVERSAL || magic == MACH_MAGIC_UNIVERSAL) {
        return MH_TYPE_FAT;
    } else {
        return MH_TYPE_UNKNOWN;
    }
}


/**
 *  Function:   mach_universal_load
 *  ----------------------------------
 * 
 *  Loads a raw Universal Mach-O Header from a given offset in a verified file, and
 *  returns the resulting structure.
 *  
 *  file:       The verified file.
 * 
 *  Returns:    A verified Universal/FAT Mach Header structure.
 */
fat_header_info_t *mach_universal_load (file_t *file)
{
    // Create the FAT header so we can read some data from
    // the file. The header starts at 0x0 in the file. It
    // is also in Little-Endian form, so we have to swap
    // the byte order.
    fat_header_t *fat_header = (fat_header_t *) file_load_bytes (file, sizeof (fat_header_t), 0);
    fat_header = swap_header_bytes (fat_header);

    // Check the number of architectures
    if (!fat_header->nfat_arch) {
        errorf ("Empty Mach-O Universal Binary");
        exit (0);
    }

    printf ("[*] %s: Mach-O Universal Binary. Found %d architectures.\n", file->path, fat_header->nfat_arch);

    // Arch list
    HSList *archs = NULL;

    // Create an offset to move through the archs.
    uint32_t offset = sizeof(fat_header_t);
    for (int i = 0; i < fat_header->nfat_arch; i++) {

        // Current arch. Also needs to swap the bytes.
        struct fat_arch *arch = (struct fat_arch *) file_load_bytes (file, sizeof(struct fat_arch), offset);
        arch = swap_fat_arch_bytes (arch);

        // Add to the list
        archs = h_slist_append (archs, arch);

        // Increment the offset
        offset += sizeof(struct fat_arch);
    }

    fat_header_info_t *ret = malloc (sizeof(fat_header_info_t));
    ret->header = fat_header;
    ret->archs = archs;

    return ret;
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
mach_header_64_t *mach_header_load (file_t *file)
{
    // Load bytes from the file to read the header
    mach_header_64_t *header = mach_header_create ();
    header = (mach_header_64_t *) file_load_bytes (file, MACH_HEADER_SIZE, 0);

    // Check that the header has a magic value
    if (!header->magic) {
        debugf ("[*] Error: Header was not loaded correctly. Aborting!\n");
        exit (0);
    }

    // Verify the magic value as Mach-O 64.
    mach_header_type_t h_type = mach_header_verify (file);
    if (h_type == MH_TYPE_MACHO64) {
        return header;
    } else if (h_type == MH_TYPE_MACHO32) {
        errorf ("Cannot handle Mach-O 32bit files just yet\n");
        exit(0);
    } else if (h_type == MH_TYPE_FAT) { 
        // call Universal Binary shit, then come back.
    } else {
        debugf ("[*] Could not determine Mach-O type with Magic 0x%x. Aborting!\n", header->magic);
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
 *  Function:   mach_header_read_cpu_sub_type
 *  -------------------------------------
 * 
 *  Returns a decoded string of header->cpusubtype.
 * 
 *  type:       The cpu_subtype_t from the Mach-O Header.
 * 
 *  Returns:    Decoded CPU sub type String.
 */
char *mach_header_read_cpu_sub_type (cpu_subtype_t type)
{
    char *cpu_subtype = "";
    switch (type) {
        case CPU_SUBTYPE_ARM64_ALL:
            cpu_subtype = "arm64";
            break;
        case CPU_SUBTYPE_ARM64_V8:
            cpu_subtype = "arm64_v8";
            break;
        case CPU_SUBTYPE_ARM64E:
            cpu_subtype = "arm64e";
            break;
        default:
            cpu_subtype = "unknown";
            break;
    }
    return cpu_subtype;
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
        case MACH_TYPE_KEXT_BUNDLE:
            ret = "Mach Kernel Extension Bundle (MH_KEXT_BUNDLE)";
            break;
        default:
            ret = "Unknown";
            break;
    }
    return ret;
}


/**
 *  Function:   mach_header_read_file_type_short
 *  -------------------------------------
 * 
 *  Returns a decoded string of header->filetype.
 * 
 *  type:       The header->filetype uint32_t from the Mach-O Header.
 * 
 *  Returns:    Decoded header->filetype.
 */
char *mach_header_read_file_type_short (uint32_t type)
{
    char *ret = "";
    switch (type) {
        case MACH_TYPE_OBJECT:
            ret = "Object";
            break;
        case MACH_TYPE_EXECUTE:
            ret = "Executable";
            break;
        case MACH_TYPE_DYLIB:
            ret = "Dynamic Library";
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


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


/**
 * 
 */
fat_header_t *swap_header_bytes (fat_header_t *header)
{
    header->magic = OSSwapInt32(header->magic);
    header->nfat_arch = OSSwapInt32(header->nfat_arch);
    return header;
}


struct fat_arch *swap_fat_arch_bytes (struct fat_arch *a)
{
    a->cputype = OSSwapInt32(a->cputype);
    a->cpusubtype = OSSwapInt32(a->cpusubtype);
    a->offset = OSSwapInt32(a->offset);
    a->size = OSSwapInt32(a->size);
    a->align = OSSwapInt32(a->align);
    return a;
}