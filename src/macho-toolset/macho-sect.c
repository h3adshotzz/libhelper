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
//  Copyright (C) 2023, Is This On? Holdings Limited
//
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#include <stdlib.h>
#include <getopt.h>

#include <libhelper.h>
#include <libhelper-macho.h>
#include <libhelper-logger.h>

void version ()
{
    exit (0);
}

void help ()
{
    exit (0);
}

int main (int argc, char *argv[])
{
    char *filename = argv[1];
    char *segment_name = argv[2];
    char *section_name = argv[3];

    if (!segment_name) {
        errorf ("Segment name is invalid\n");
        return 0;
    }

    printf ("loading file: %s\n", filename);
    macho_t *macho = macho_64_load (filename);
    mach_segment_info_t *segment64 = mach_segment_info_search (macho->scmds, segment_name);

    char *outfile;
    unsigned char *out_data;
    uint32_t out_size = 0;

    if (section_name) {
        mach_section_64_t *section64 = (mach_section_64_t *) mach_section_64_search_in_segment (segment64, section_name);
        out_size = section64->size;
        out_data = macho_load_bytes (macho, out_size, section64->offset);

        uint32_t sout = strlen (segment_name) + strlen (section_name) + 6;
        outfile = malloc (sout);
        snprintf (outfile, sout, "%s.%s.data", segment_name, section_name);
    } else {

        mach_segment_command_64_t *segcmd = (mach_segment_command_64_t *) segment64->segcmd;
        out_size = segcmd->filesize;
        out_data = macho_load_bytes (macho, out_size, segcmd->fileoff);

        uint32_t sout = strlen (segment_name) + 6;
        outfile = malloc (sout);
        snprintf (outfile, sout, "%s.data", segment_name);
    }

    FILE *fp = fopen (outfile, "w+");
    fwrite (out_data, out_size, 1, fp);
    fclose (fp);

    printf ("wrote %s\n", outfile);

ERROR:
    return 0;
}