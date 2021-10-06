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
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-macho: tests of FAT loading api */

#include <libhelper.h>
#include <libhelper-file.h>
#include <libhelper-logger.h>
#include <libhelper-macho.h>
#include <libhelper-fat.h>

///////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
    const char *filename = argv[1];
    printf ("\n**Running Libhelper-MachO FAT API Test Suite**\n");

    /* macho_check_arch */
    mach_header_type_t type = macho_check_arch (filename);
    if (type != MH_TYPE_FAT) {
        debugf ("file is not a FAT file\n");
	return 0;
    } else {
        test_successf ("macho_check_arch\n");
    }

    /* macho_universal_load */
    fat_info_t *fat = macho_universal_load (filename);
    printf ("File %s contains %d Architecture[s]: \n", filename, fat->header->nfat_arch);
    for (int i = 0; i < fat->header->nfat_arch; i++) {
        fat_arch_t *arch = (fat_arch_t *) h_slist_nth_data (fat->archs, i);
        printf ("arch[%d]: %s (%s)\n", i, 
                mach_header_get_cpu_string (arch->cputype, arch->cpusubtype),
                mach_header_get_cpu_descriptor (arch->cputype, arch->cpusubtype));
    }

    return 0;
}

