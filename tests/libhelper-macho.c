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


//
//	THIS IS A TEST PROGRAM FOR LIBHELPER
//
//      Purpose is to test libhelper-macho
//

#include <libhelper/libhelper.h>
#include <libhelper/libhelper-macho.h>

void __libhelper_macho_command_print_test (mach_load_command_info_t *inf, mach_load_command_t *lc)
{
    if (inf) {
        debugf ("--- Meta:\n");
        debugf ("  Type:\t0x%x\n", inf->type);
        debugf ("Offset:\t0x%llx\n", inf->offset);

        if (!lc)
            lc = inf->lc;
    }

    debugf ("     Command:\t%s\n", mach_load_command_get_string (lc));
    debugf ("Command Size:\t%d\n", lc->cmdsize);
}

int _libhelper_macho_tests (const char *path)
{
    //macho_t *macho = macho_load (path);

    file_t *f = file_load (path);
    macho_t *macho = macho_create_from_buffer (file_load_bytes (f, f->size, 0));


    if (!macho)
        errorf ("libhelper-macho.c: _libhelper_macho_tests(): macho == NULL\n")

    mach_header_t *header = macho->header;

    printf ("==================\nMach-O Header Dump\n==================\n\n");

    printf ("Magic: \t\t0x%x\n", header->magic);
    printf ("CPU Type: \t%s\n", mach_header_read_cpu_type(header->cputype));
    printf ("CPU Sub-Type: \t0x%x\n", header->cpusubtype);
    printf ("File Type: \t%s\n", mach_header_read_file_type (header->filetype));
    printf ("Load Commands: \t%d\n", header->ncmds);
    printf ("LC Size: \t%d\n", header->sizeofcmds);

    printf ("------------------\n\n");

    
    // segments
    for (int i = 0; i < (int) h_slist_length (macho->scmds); i++) {
        mach_segment_info_t *info = (mach_segment_info_t *) h_slist_nth_data (macho->scmds, i);

        mach_segment_command_64_t *seg64 = info->segcmd;
            printf ("LC %d: LC_SEGMENT_64\tOff: 0x%09llx-0x%09llx\t%s/%s   %s\n",
                        i, seg64->vmaddr - info->padding, (seg64->vmaddr + seg64->vmsize) - info->padding, 
                        mach_segment_vm_protection (seg64->initprot), mach_segment_vm_protection (seg64->maxprot),
                        seg64->segname);

            //  TODO: Add the vm protection just before the segment name, so it looks like:
            //          ... rw-/r--     __DATA
            //

            if (h_slist_length (info->sects)) {
                for (int k = 0; k < (int) h_slist_length (info->sects); k++) {
                    mach_section_64_t *sect64 = mach_section_create ();
                    sect64 = (mach_section_64_t *) h_slist_nth_data (info->sects, k);

                    // this should be pre-set names for known sections 
                    //char *__placeholder_text = "(placeholder)";
					char *__placeholder_text = "";

                    printf ("\tOff: 0x%09llx-0x%09llx\t%d bytes\t\t%s.%s\t%s\n",
                            sect64->addr, (sect64->addr + sect64->size), sect64->size,
                            sect64->segname, sect64->sectname,
                            __placeholder_text);
                }
            } else {
                printf ("\tNo Section 64 data\n");
            }
    }

}


int main (int argc, char *argv[])
{
    printf ("%s\n\n", libhelper_version_string());
    if (argc < 2)
        return 0;

    return _libhelper_macho_tests (argv[1]);
}