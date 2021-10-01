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
//  Copyright (C) 2021, Is This On? Holdings
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
    printf ("magic: 0x%08x\n", mh->magic);
    printf ("cputype: 0x%08x\n", mh->cputype);
    printf ("cpusubtype: 0x%08x\n", mh->cpusubtype);
    printf ("filetype: 0x%08x\n", mh->filetype);
    printf ("ncmds: 0x%08x\n", mh->ncmds);
    printf ("sizeofcmds: 0x%08x\n", mh->sizeofcmds);
    printf ("flags: 0x%08x\n", mh->flags);
}

int main (int argc, char *argv[])
{
    macho_t *macho = (macho_t *) macho_load (argv[1]);
    if (!macho)
        errorf ("libhelper-macho-test: macho is NULL\n");


    for (int i = 0; i < h_slist_length (macho->scmds); i++) {
        mach_segment_info_64_t *s = (mach_segment_info_64_t *) h_slist_nth_data (macho->scmds, i);
        printf ("segment: %s\n", (char *) s->segcmd->segname);

        for (int a = 0; a < h_slist_length (s->sections); a++) {
            mach_section_64_t *sect = (mach_section_64_t *) h_slist_nth_data (s->sections, a);
            printf ("\tsectname: %s\n", (char *) sect->sectname);
        }

    }

    print_header (macho->header);

    return 0;
}