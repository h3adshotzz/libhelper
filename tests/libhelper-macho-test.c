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

/* libhelper-macho: tests of macho api */

#include <stdio.h>

#include <libhelper.h>
#include <libhelper-macho.h>
#include <libhelper-logger.h>
#include <libhelper-file.h>

///////////////////////////////////////////////////////////////////////////////

void print_header (mach_header_t *mh)
{
    printf ("==HEADER==\n");
    printf ("      magic: 0x%08x\n", mh->magic);
    printf ("    cputype: 0x%08x\n", mh->cputype);
    printf (" cpusubtype: 0x%08x\n", mh->cpusubtype);
    printf ("   filetype: 0x%04x\n", mh->filetype);
    printf ("      ncmds: %d\n", mh->ncmds);
    printf (" sizeofcmds: 0x%08x\n", mh->sizeofcmds);
    printf ("      flags: 0x%08x\n", mh->flags);
    printf ("==HEADER==\n");
}

int main (int argc, char *argv[])
{
    const char *filename = argv[1];
    printf ("\n**Running Libhelper-MachO Test Suite**\n");

    /* macho_check_arch */
    mach_header_type_t type = macho_check_arch (filename);
    if (type != MH_TYPE_MACHO64) {
        debugf ("macho is not 64-bit: type: %d, filename: %s\n", type, filename);
        test_failuref ("macho_check_arch\n");
        return 0;
    } else {
        test_successf ("macho_check_arch\n");
    }


    /* macho_64_load */
    macho_t *macho = macho_64_load (filename);
    if (!macho) {
        test_failuref ("macho_64_load\n");
    } else {
        test_successf ("macho_64_load\n");
    }


    /* macho_64_create_from_buffer */
    file_t *file = file_load (filename);
    macho_t *macho2 = macho_64_create_from_buffer (file->data);
    if (!macho2) {
        test_failuref ("macho_64_create_from_buffer\n");
    } else {
        test_successf ("macho_64_create_from_buffer\n");
    }


    /* macho_load_bytes */
    mach_header_t *mh_test_1 = macho_load_bytes (macho, sizeof (mach_header_t), 0);
    if (mach_header_verify (mh_test_1->magic) == MH_TYPE_UNKNOWN) {
        test_failuref ("macho_load_bytes\n");
    } else {
        test_successf ("macho_load_bytes\n");
    }


    /* macho_get_bytes */
    mach_header_t *mh_test_2 = macho_get_bytes (macho, 0);
    if (mach_header_verify (mh_test_2->magic) == MH_TYPE_UNKNOWN) {
        test_failuref ("macho_get_bytes\n");
    } else {
        test_successf ("macho_get_bytes\n");
    }


    /* macho_read_bytes */
    mach_header_t *mh_test_3 = malloc (sizeof (mach_header_t)); 
    macho_read_bytes (macho, 0, mh_test_3, sizeof (mach_header_t));
    if (mach_header_verify (mh_test_3->magic) == MH_TYPE_UNKNOWN) {
        test_failuref ("macho_read_bytes_test\n");
    } else {
        test_successf ("macho_read_bytes_test\n");
    }


    /* mach_header_load */
    mach_header_t *mh_test_4 = mach_header_load (macho);
    if (mach_header_verify (mh_test_3->magic) == MH_TYPE_UNKNOWN) {
        test_failuref("mach_header_load\n");
    } else {
        test_successf("mach_header_load\n");
    }


    /* mach_header_get_cpu_string */
    char *cpu_string = mach_header_get_cpu_string (mh_test_4->cputype, mh_test_4->cpusubtype);
    if (!cpu_string) {
        test_failuref ("mach_header_get_cpu_string\n");
    } else {
        test_successf ("mach_header_get_cpu_string\n");
    }


    /* mach_header_get_file_type_string */
    char *type_string = mach_header_get_file_type_string (mh_test_4->filetype);
    if (!type_string) {
        test_failuref ("mach_header_get_file_type_string\n");
    } else {
        test_successf ("mach_header_get_file_type_string\n");
    }


    /* mach_header_get_file_type_short */
    char *file_type_short = mach_header_get_file_type_short (mh_test_4->filetype);
    if (!file_type_short) {
        test_failuref ("mach_header_get_file_type_short\n");
    } else {
        test_successf ("mach_header_get_file_type_short\n");
    }


    /* mach_load_command_info_load && mach_load_command_get_name*/
    mach_load_command_info_t *cmd_inf = mach_load_command_info_load ((const char *) macho->data, sizeof(mach_header_t));
    char *cmd_inf_test = mach_load_command_get_name (cmd_inf->lc);
    if (!strcmp (cmd_inf_test, "LC_UNKNOWN")) {
        test_failuref ("mach_load_command_info_load\n");
        test_failuref ("mach_load_command_get_name\n");
    } else {
        test_successf ("mach_load_command_info_load\n");
        test_successf ("mach_load_command_get_name\n");
    }


    /* mach_load_command_find_command_by_type */
    mach_load_command_info_t *cmd_inf_2 = mach_load_command_find_command_by_type (macho, LC_SYMTAB);
    if (!cmd_inf_2->lc) {
        test_failuref ("mach_load_command_find_command_by_type\n");
    } else {
        test_successf ("mach_load_command_find_command_by_type\n");
    }


    /* mach_load_command_load_string */
    test_failuref("mach_load_command_load_string\n");


    /* mach_segment_info_search */
    mach_segment_info_t *seg_inf = mach_segment_info_search (macho->scmds, "__TEXT");
    if (!seg_inf) {
        test_failuref ("mach_segment_info_search\n");
    } else {
        test_successf ("mach_segment_info_search\n");
    }


    /* mach_segment_info_load */
    mach_segment_info_t *seg_inf_2 = mach_segment_info_load (macho->data, seg_inf->offset);
    if (!seg_inf_2) {
        test_failuref ("mach_segment_info_load\n");
    } else {
        test_successf ("mach_segment_info_load\n");
    }


    /* mach_segment_command_32_from_info */
    test_failuref ("mach_segment_command_32_from_info\n");


    /* mach_segment_command_64_from_info */
    mach_segment_command_64_t *seg_inf_64 = mach_segment_command_64_from_info (seg_inf_2);
    if (!seg_inf_64) {
        test_failuref ("mach_segment_command_64_from_info\n");
    } else {
        test_successf ("mach_segment_command_64_from_info\n");
    }


    /* mach_segment_command_64_load */
    mach_segment_command_64_t *seg_inf_64_2 = mach_segment_command_64_load (macho->data, seg_inf_2->offset);
    if (!seg_inf_64_2) {
        test_failuref ("mach_segment_command_64_load\n");
    } else {
        test_successf ("mach_segment_command_64_load\n");
    }


    /* mach_segment_command_32_load */
    test_failuref ("mach_segment_command_32_load\n");


    /* mach_section_64_load */
    mach_section_64_t *sect = mach_section_64_load (macho->data, seg_inf_2->offset + sizeof (mach_segment_command_64_t));
    if (!sect) {
        test_failuref ("mach_section_64_load\n");
    } else {
        test_successf ("mach_section_64_load\n");
    }


    /* mach_section_32_load */
    test_failuref ("mach_section_32_load\n");


    /* mach_section_64_search_in_segment */
    mach_section_64_t *sect_2 = mach_section_64_search_in_segment (seg_inf_2, "__text");
    if (!sect_2) {
        test_failuref ("mach_section_64_search_in_segment\n");
    } else {
        test_successf ("mach_section_64_search_in_segment\n");
    }

    
    /* mach_section_32_search_in_segment */
    test_failuref ("mach_section_32_search_in_segment\n");


    /* mach_section_64_search */
    mach_section_64_t *sect_3 = mach_section_64_search (macho->scmds, "__TEXT", "__text");
    if (!sect_3) {
        test_failuref ("mach_section_64_search\n");
    } else {
        test_successf ("mach_section_64_search\n");
    }


    /* mach_section_32_search */
    test_failuref ("mach_section_32_search\n");


    /* mach_section_find_at_index */
    mach_section_64_t *sect_4 = mach_section_find_at_index (macho->scmds, 3);
    if (!sect_4) {
        test_failuref ("mach_section_find_at_index\n");
    } else {
        test_successf ("mach_section_find_at_index\n");
    }


    /* mach_load_command_find_source_version_command */
    mach_source_version_command_t *svc = mach_load_command_find_source_version_command (macho);
    if (!svc) {
        test_failuref ("mach_load_command_find_source_version_command\n");
    } else {
        test_successf ("mach_load_command_find_source_version_command\n");
    }


    /* mach_load_command_get_source_version_string */
    char *svc_str = mach_load_command_get_source_version_string (svc);
    if (!svc_str) {
        test_failuref ("mach_load_command_get_source_version_string\n");
    } else {
        test_successf ("mach_load_command_get_source_version_string\n");
    }


    /* mach_load_command_dylib_format_version */
    test_failuref ("mach_load_command_dylib_format_version\n");


    ///////////////////////////////////////////////////////////////////////////

    printf ("\n**Running Example Tool**\n");

    print_header (macho->header);

    for (int i = 0; i < h_slist_length (macho->scmds); i++) {
        mach_segment_info_t *s = (mach_segment_info_t *) h_slist_nth_data (macho->scmds, i);
        mach_segment_command_64_t *segcmd = (mach_segment_command_64_t *) s->segcmd;

        printf ("segment: %s (initprot: %s, maxprot: %s)\n", (char *) segcmd->segname,
            mach_segment_read_vm_protection (segcmd->initprot),
            mach_segment_read_vm_protection (segcmd->maxprot));

        for (int a = 0; a < h_slist_length (s->sections); a++) {
            mach_section_64_t *sect = (mach_section_64_t *) h_slist_nth_data (s->sections, a);
            printf ("\tsectname: %s\n", (char *) sect->sectname);
        }
    }

    for (int i = 0; i < h_slist_length (macho->dylibs); i++) {
        mach_dylib_command_info_t *dylib = (mach_dylib_command_info_t *) h_slist_nth_data (macho->dylibs, i);
        printf ("dylib[%d]: %s, type: %d\n", i, dylib->name, dylib->type);
    }

    return 0;
}
