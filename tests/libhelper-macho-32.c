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

/**
 *  Testing support for 32 bit Mach-O's in libhelper.
 * 
 */

#include <libhelper/libhelper.h>
#include <libhelper/libhelper-macho.h>

int _libhelper_macho_32_tests (const char *path)
{
    macho_32_t *macho = macho_load (path);

    if (!macho)
        errorf ("libhelper-macho-32.c: _libhelper_macho_32_tests() failed: macho == NULL\n");

    mach_header_32_t *hdr = macho->header;
    printf ("==================\nMach-O Header 32 Dump\n==================\n\n");
    printf ("Magic: \t\t0x%x\n", hdr->magic);
    printf ("CPU Type: \t%s\n", mach_header_get_cpu_name (hdr->cputype, hdr->cpusubtype));
    printf ("File Type: \t%s\n", mach_header_read_file_type (hdr->filetype));
    printf ("Load Commands: \t%d\n", hdr->ncmds);
    printf ("LC Size: \t%d\n", hdr->sizeofcmds);
    printf ("------------------\n\n\n");

    //////////////////////////////////////

    for (int i = 0; i < (int) h_slist_length (macho->scmds); i++) {
        mach_segment_info_32_t *info = (mach_segment_info_32_t *) h_slist_nth_data (macho->scmds, i);
        mach_segment_command_32_t *seg32 = info->segcmd;

        printf ("LC %d: LC_SEGMENT\tOff: 0x%09llx-0x%09llx\t%s/%s   %s\n",
                i, seg32->vmaddr - info->padding, (seg32->vmaddr + seg32->vmsize) - info->padding, 
                mach_segment_vm_protection (seg32->initprot), mach_segment_vm_protection (seg32->maxprot),
                seg32->segname);

        if (h_slist_length (info->sects)) {
            for (int k = 0; k < (int) h_slist_length (info->sects); k++) {
                mach_section_32_t *sect32 = (mach_section_32_t *) h_slist_nth_data (info->sects, k);
                char *__placeholder_text = "";

                printf ("\tOff: 0x%09llx-0x%09llx\t%d bytes\t\t%s.%s\t%s\n",
                            sect32->addr, (sect32->addr + sect32->size), (int) sect32->size,
                            sect32->segname, sect32->sectname,
                            __placeholder_text);
            }
        } else {
            printf ("\tNo section 32 data\n");
        }

    }

    return 1;
}

int main (int argc, char *argv[])
{
    printf ("%s\n\n", libhelper_version_string ());
    if (argc < 2)
        return 0;

    return _libhelper_macho_32_tests (argv[1]);
}