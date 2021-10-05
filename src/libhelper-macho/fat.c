//===----------------------------------------------------------------------===//
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
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//  Copyright (C) 2020, Is This On?, @h3adsh0tzz
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#include "libhelper-logger.h"
#include "libhelper-macho.h"
#include "libhelper-fat.h"

#include "macho.h"


fat_header_t *
swap_fat_header_bytes (fat_header_t *hdr)
{
    hdr->magic = OSSwapInt32 (hdr->magic);
    hdr->nfat_arch = OSSwapInt32 (hdr->nfat_arch);
    return hdr;
}

struct fat_arch *
swap_fat_arch_bytes (struct fat_arch *fa)
{
    fa->cputype = OSSwapInt32(fa->cputype);
    fa->cpusubtype = OSSwapInt32(fa->cpusubtype);
    fa->offset = OSSwapInt32(fa->offset);
    fa->size = OSSwapInt32(fa->size);
    fa->align = OSSwapInt32(fa->align);
    return fa;
}

mach_header_t *
swap_mach_header_bytes (mach_header_t *hdr)
{
    hdr->magic = OSSwapInt32(hdr->magic);
    hdr->cputype = OSSwapInt32(hdr->cputype);
    hdr->cpusubtype = OSSwapInt32(hdr->cpusubtype);
    hdr->filetype = OSSwapInt32(hdr->filetype);
    hdr->ncmds = OSSwapInt32(hdr->ncmds);
    hdr->sizeofcmds = OSSwapInt32(hdr->sizeofcmds);
    hdr->flags = OSSwapInt32(hdr->flags);
    hdr->reserved = OSSwapInt32(hdr->reserved);
    return hdr;
}

fat_info_t *
macho_universal_load (const char *filename)
{
    /**
     *  Check we are actually loading a FAT/Universal binary file.
     */
    if (macho_check_arch (filename) != MH_TYPE_FAT) {
        errorf ("mach_universal_load: Could not load a non-FAT file as a Universal Binary.\n");
        return NULL;
    }
    file_t *f = macho_load_file_from_filename (filename);
    
    /**
     *  Create the FAT header, which starts at 0x0. The header is
     *  in little-endian format, so we have to use the swap bytes
     *  functions.
     */
    fat_header_t *hdr = (fat_header_t *) file_dup_data (f, 0, sizeof (fat_header_t));
    hdr = swap_fat_header_bytes (hdr);
    
    /* check the number of architectures */
    if (!hdr->nfat_arch) {
        errorf ("mach_universal_load: FAT file does not contain any Mach-O's\n");
        return NULL;
    }
    // should be a debugf
    printf ("mach_universal_load: Found %d architectures in file: %s\n", hdr->nfat_arch, filename);
    
    /* list of archs */
    HSList *archs = NULL;
    uint32_t off = 0;
    
    for (int i = 0; i < hdr->nfat_arch; i++) {
        struct fat_arch *arch = (struct fat_arch *) (f->data + (sizeof (fat_header_t) + off));
        arch = swap_fat_arch_bytes (arch);
        
        printf ("arch->cputype: %s\n", mach_header_get_cpu_string (arch->cputype, arch->cpusubtype));
        
        off += sizeof (struct fat_arch);
    }
    
    return NULL;
}











