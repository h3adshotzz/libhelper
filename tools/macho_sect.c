//===---------------------------- macho_sect.c ----------------------------===//
//
//                                 macho_sect
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
//  me@h3adsh0tzz.com.
//
//
//===-----------------------------------------------------------------------===//

#include <libhelper/libhelper.h>
#include <libhelper-macho/macho.h>
#include <libhelper-macho/macho-header.h>
#include <libhelper-macho/macho-command.h>
#include <libhelper-macho/macho-segment.h>

#ifdef __APPLE__
#   define BUILD_TARGET         "darwin"
#   define BUILD_TARGET_CAP     "Darwin"
#else
#   define BUILD_TARGET         "linux"
#   define BUILD_TARGET_CAP     "Linux"
#endif

#ifdef __x86_64__
#   define BUILD_ARCH           "x86_64"
#elif __arm__
#   define BUILD_ARCH           "arm64"
#endif
#define	MACHO_SECT_VERSION		"1.0.0"


void banner ()
{
    printf ("-----------------------------------------------------\n");
    printf ("macho_sect %s - Built " __TIMESTAMP__ "\n", MACHO_SECT_VERSION);
    printf ("-----------------------------------------------------\n");
}

void version ()
{
    printf ("Libhelper macho_sect Version %s (%s)\n", MACHO_SECT_VERSION, LIBHELPER_VERSION_LONG);
    printf ("\tBuild Time:\t\t" __TIMESTAMP__ "\n");

    printf ("\tDefault Target:\t\t%s-%s\n", BUILD_TARGET, BUILD_ARCH);
    printf ("\tLibhelper Build:\t%s\n", LIBHELPER_VERSION_LONG);
}

void help ()
{
    banner ();

    printf ("Usage: macho_sect FILE [segment] [section]\n\n");

    printf ("Other Options:\n");
    printf ("  -h\tHelp Menu\n  -v\tVersion Info\n\n");
}

int main (int argc, char *argv[])
{
    if (argc < 2) {
	help ();
	return 0;
    }

    // Check for the -v or -v options
    for (int i = 1; i < argc; i++) {
	if (!strcmp (argv[i], "-h")) {
	    help ();
	    return 0;
	} else if (!strcmp (argv[i], "-v")) {
	    version ();
	    return 0;
     	}
    }

    if (argc < 4) {
	help ();
	return 0;
    }

    banner ();

    // If we are here, no options were given.
    char *filename = argv[1];
    char *segment = argv[2];
    char *section = argv[3];

    if (!filename || !segment || !section) {
	printf ("Unable to split %s.%s from %s\n", segment, section, filename);
	return 0;
    }

    printf ("seperating %s,%s from %s\n", segment, section, filename);

    // Create a file struct
    file_t *file = file_load (filename);

    // Check that the file is a macho-64
    mach_header_type_t type = mach_header_verify (file);
    if (type == MH_TYPE_MACHO64) {
	// just continue
    } else {
	printf ("Can only operate on 64-bit Mach-O's (0x%x).\n", (uint32_t) file_load_bytes (file, sizeof(uint32_t), 0));
    }

    return 0;
}
