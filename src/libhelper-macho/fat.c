//===--------------------------- libhelper ----------------------------===//
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
//
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//	Copyright (C) 2020, Is This On?, @h3adsh0tzz
//
//  me@h3adsh0tzz.com.
//
//
//===------------------------------------------------------------------===//

#include "libhelper/libhelper.h"
#include "libhelper/libhelper-macho.h"
#include "libhelper/libhelper-fat.h"

//===-----------------------------------------------------------------------===//
/*-- FAT (Universal Binary)                              				      -*/
//===-----------------------------------------------------------------------===//

/**
 *  Function:   swap_header_bytes
 *  ------------------------------------
 * 
 *  Swaps the bytes of a fat_header_t. 
 * 
 *  returns:    A swapped fat_header_t struct.
 * 
 */
fat_header_t *swap_header_bytes (fat_header_t *header)
{
    header->magic = OSSwapInt32(header->magic);
    header->nfat_arch = OSSwapInt32(header->nfat_arch);
    return header;
}


/**
 *  Function:   swap_fat_arch_bytes
 *  ------------------------------------
 * 
 *  Swaps the bytes of a fat_arch. 
 * 
 *  returns:    A swapped fat_arch struct.
 * 
 */
struct fat_arch *swap_fat_arch_bytes (struct fat_arch *a)
{
    a->cputype = OSSwapInt32(a->cputype);
    a->cpusubtype = OSSwapInt32(a->cpusubtype);
    a->offset = OSSwapInt32(a->offset);
    a->size = OSSwapInt32(a->size);
    a->align = OSSwapInt32(a->align);
    return a;
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
	uint32_t size = file->size;
    unsigned char *data = (unsigned char *) file_load_bytes (file, size, 0);

    // Create the FAT header so we can read some data from
    // the file. The header starts at 0x0 in the file. It
    // is also in Little-Endian form, so we have to swap
    // the byte order.
    fat_header_t *fat_header = malloc (sizeof (fat_header_t));
    memset (fat_header, '\0', sizeof (fat_header_t));
    memcpy (fat_header, &data[0], sizeof (fat_header_t));

    fat_header = swap_header_bytes (fat_header);

    // Check the number of architectures
    if (!fat_header->nfat_arch) {
        errorf ("Empty Mach-O Universal Binary");
        exit (0);
    }

    if (fat_header->nfat_arch > 1) 
        printf ("[*] %s: Mach-O Universal Binary. Found %d architectures.\n", file->path, fat_header->nfat_arch);

    // Arch list
    HSList *archs = NULL;

    // Create an offset to move through the archs.
    uint32_t offset = sizeof(fat_header_t);
    for (uint32_t i = 0; i < fat_header->nfat_arch; i++) {

        // Current arch. Also needs to swap the bytes.
        struct fat_arch *arch = (struct fat_arch *) malloc (sizeof (struct fat_arch));
        memset (arch, '\0', sizeof (struct fat_arch));
        memcpy (arch, data + offset, sizeof (struct fat_arch));
        ///file_load_bytes (file, sizeof(struct fat_arch), offset);

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