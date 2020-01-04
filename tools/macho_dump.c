//===---------------------------- macho_dump.c ----------------------------===//
//
//                                 macho_dump
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
    printf ("macho_dump %s - Built " __TIMESTAMP__ "\n", MACHO_SECT_VERSION);
    printf ("-----------------------------------------------------\n");
}

void version ()
{
    printf ("Libhelper macho_dump Version %s (%s)\n", MACHO_SECT_VERSION, LIBHELPER_VERSION_LONG);
    printf ("\tBuild Time:\t\t" __TIMESTAMP__ "\n");

    printf ("\tDefault Target:\t\t%s-%s\n", BUILD_TARGET, BUILD_ARCH);
    printf ("\tLibhelper Build:\t%s\n", LIBHELPER_VERSION_LONG);
}

void help ()
{
    banner ();

    printf ("Usage: macho_dump FILE [start_addr] [end_addr | -s size]\n\n");

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

	//Usage: macho_dump FILE [start_addr] [end_addr | -s size

    // If we are here, no options were given.
    char *filename = argv[1];
    uint32_t start = (uint32_t) argv[2];
    uint32_t a2 = (uint32_t) argv[3];
    uint32_t size = 0;

    macho_t *macho = macho_create ();
    unsigned char *dump = NULL;

    macho = macho_load (filename);
    uint32_t offset = macho->offset;
    printf ("filename: %s\noffset: 0x%x\n", macho->path, offset);
    
    if (!strcmp (argv[3], "-s")) {
        // use a size
        size = a2;
    } else {
        size = a2 - start;
    }
    size = 3048;
    printf ("size: %d\n", size);
    
    dump = malloc (size);
    
    memset (dump, '\0', size);
    memcpy (dump, macho->data + offset, size);

    if (dump == NULL) {
	errorf ("Something went wrong\n");
	return -1;
    }

    // hexdump
    uint8_t hex_off = 0;
    int lines = size / 8;
    int pos = 0;

    for (int i = 0; i < lines; i++) {
	printf ("%08x  ", hex_off);

	uint8_t ln[16];

	int j;
	for (j = 0; j < 16; j++) {
	    uint8_t byte = (uint8_t) dump[pos];
	    printf ("%02x ", byte);

	    if (j == 7) printf (" ");

	    pos++;
	    ln[j] = byte;
	}

	printf ("  |");

	for (int k = 0; k < 16; k++) {
	    if (ln[k] < 0x20 || ln[k] > 0x7e)
		printf ( "." );
	    else
		printf ( "%c", (char) ln[k]);
	}

	printf ("|\n");
	hex_off += 0x10;
    }

    printf ("\n");

    
    // write to file
    printf ("writing %d bytes to outfile.raw\n", size);
    FILE *fptr = fopen ("outfile.raw", "wb");
    fwrite (dump, sizeof (char), size, fptr);
    fclose (fptr);

    return 0;
}
