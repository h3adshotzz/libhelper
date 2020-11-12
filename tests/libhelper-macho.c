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

}


int main (int argc, char *argv[])
{
    printf ("%s\n\n", libhelper_version_string());
    if (argc < 2)
        return 0;

    return _libhelper_macho_tests (argv[1]);
}