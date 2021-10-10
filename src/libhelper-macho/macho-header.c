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

#include "libhelper-macho.h"
#include "libhelper-hlibc.h"
#include "libhelper-logger.h"

mach_header_t *
mach_header_load (macho_t *macho)
{
    mach_header_t *hdr;

    /* Check that the macho_t given was initialised properly */
    if (macho) {
        unsigned char *data = macho->data;
        hdr = calloc(1, sizeof (mach_header_t));

        /* Copy bytes from data to hdr */
        memcpy (hdr, &data[0], sizeof (mach_header_t));

        /* Verify the magic value was loaded */
        if (!hdr->magic) {
            errorf ("mach_header_load(): No magic value in given macho, something went wrong.\n");
            return NULL;
        }

        /* Check the magic value */
        mach_header_type_t mh_type = mach_header_verify (hdr->magic);

        if (mh_type == MH_TYPE_MACHO64) {
            debugf ("mach_header_load(): detected mach-o 64-bit.\n");
            return hdr;
        } else if (mh_type == MH_TYPE_MACHO32) {
            debugf ("mach_header_load(): detected mach-o 32-bit.\n");
            return hdr;
        } else if (mh_type == MH_TYPE_FAT) {
            debugf ("mach_header_load(): detected mach-o fat/universal.\n");
            return hdr;
        } else {
            errorf ("mach_header_load(): Could not detect file magic: 0x%08x.\n", hdr->magic);
            return hdr;
        }
    }
    return hdr;
}

mach_header_type_t
mach_header_verify (uint32_t magic)
{
    debugf("magic: 0x%08x\n", magic);
    switch (magic) {
        case MACH_MAGIC_32:
        case MACH_CIGAM_32:
            return MH_TYPE_MACHO32;
        case MACH_MAGIC_64:
        case MACH_CIGAM_64:
            return MH_TYPE_MACHO64;
        case MACH_MAGIC_UNIVERSAL:
        case MACH_CIGAM_UNIVERSAL:
            return MH_TYPE_FAT;
        default:
            return MH_TYPE_UNKNOWN;
    }
}

char *
mach_header_get_cpu_string (cpu_type_t      cpu_type,
                            cpu_subtype_t   cpu_subtype)
{
    switch (cpu_type) {
        /* ARM CPUs */
        case CPU_TYPE_ARM:      /* AArch32 */
            return "arm32";

        /* ARM64 CPUs */
        case CPU_TYPE_ARM64:

            if ((cpu_subtype & CPU_SUBTYPE_ARM64_V8) == CPU_SUBTYPE_ARM64_V8)
                return "arm64";

            if ((cpu_subtype & CPU_SUBTYPE_ARM64E) == CPU_SUBTYPE_ARM64E)
                return "arm64e";

            if ((cpu_subtype & CPU_SUBTYPE_ARM64_ALL) == CPU_SUBTYPE_ARM64_ALL)
                return "arm64";

            return "arm64_unknown";

        /* ARM64_32 CPUs */
        case CPU_TYPE_ARM64_32:
            return "arm64_32";

        /* Intel x86_64 */
        case CPU_TYPE_X86_64:
            return "x86_64";

        /* Any other unknown CPU type */
        default:
            return "unknown_cpu";
    }
}

char *
mach_header_get_cpu_descriptor (cpu_type_t      cpu_type,
                                cpu_subtype_t   cpu_subtype)
{
    HString *str = NULL;
    switch (cpu_type) {

        /* ARM64 */
        case CPU_TYPE_ARM64:

            /* All ARM64 CPUs are ARMv8 */
            str = h_string_new ("ARMv8");

            /* Check if this is arm64e or not */
            if (cpu_subtype & CPU_SUBTYPE_ARM64E)
                str = h_string_append_len (str, ".5-A", 4);
            else
                str = h_string_append_len (str, "-A", 2);

            /* Check for CPU extensions */
            if (cpu_subtype & CPU_SUBTYPE_ARM64E_MTE_MASK)
                str = h_string_append_len (str, ", MTE", 5);

            if (cpu_subtype & CPU_SUBTYPE_ARM64_PTR_AUTH_MASK)
                str = h_string_append_len (str, ", PAC", 5);

            /* CPU extensions display as "ARMv8-A, <ext>, <ext>" */
            return str->str;


        /* Anything other than ARM64 */
        case CPU_TYPE_ARM64_32:
        case CPU_TYPE_X86_64:
        default:
            return mach_header_get_cpu_string (cpu_type, cpu_subtype);
    }
}

char *
mach_header_get_file_type_string (uint32_t type)
{
    switch (type) {
        case MACH_TYPE_OBJECT:
            return "Mach Object (MH_OBJECT)";
        case MACH_TYPE_EXECUTE:
            return "Mach Executable (MH_EXECUTE)";
        case MACH_TYPE_DYLIB:
            return "Mach Dynamic Library (MH_DYLIB)";
        case MACH_TYPE_KEXT_BUNDLE:
            return "Mach Kernel Extension Bundle (MH_KEXT_BUNDLE)";
        case MACH_TYPE_FILESET:
            return "Mach File Set (MH_FILESET)";
        case MACH_TYPE_DYLINKER:
            return "Mach Dyanmic Linker (MH_DYLINKER)";
        default:
            return "Mach Unknown";
    }
}

char *
mach_header_get_file_type_short (uint32_t type)
{
    char *ret = "";
    switch (type) {
        case MACH_TYPE_OBJECT:
            return "Object";
        case MACH_TYPE_EXECUTE:
            return "Executable";
        case MACH_TYPE_DYLIB:
            return "Dynamic Library";
        case MACH_TYPE_FILESET:
            return "File set";
        case MACH_TYPE_DYLINKER:
            return "Dynamic Linker";
        default:
            return "Unknown";
    }
}
