//===--------------------------- macho_split.c -----------------------------===//
//
//                                macho_split
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
#include <libhelper-macho-ll/macho.h>
#include <libhelper-macho-ll/macho-command.h>
#include <libhelper-macho-ll/macho-segment.h>

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
#define	MACHO_SPLIT_VERSION		"1.0.0"

#define FAT(p) ((*(unsigned int *)(p) & ~1) == 0xbebafeca)

struct archs {
	size_t 	  size;
	char	 *buf;
	char	 *name;
};

void banner ()
{
	printf ("-----------------------------------------------------\n");
    printf ("macho_split %s - Built " __TIMESTAMP__ "\n", MACHO_SPLIT_VERSION);
    printf ("-----------------------------------------------------\n");
}

void version ()
{
	printf ("Libhelper macho_split Version %s (%s)\n", MACHO_SPLIT_VERSION, LIBHELPER_VERSION_LONG);
    printf ("\tBuild Time:\t\t" __TIMESTAMP__ "\n");

    printf ("\tDefault Target:\t\t%s-%s\n", BUILD_TARGET, BUILD_ARCH);
    printf ("\tLibhelper Build:\t%s\n", LIBHELPER_VERSION_LONG);
}

void help ()
{
	banner ();

	printf ("Usage: macho_split FILE\n\n");

	printf ("Other Options:\n");
	printf ("  -h\tHelp Menu\n  -v\tVersion Info\n  -l\tList Archs contained within file\n\n");
}

int main (int argc, char *argv[])
{

	if (argc < 2) {
		help ();
		return 0;
	}

	// Check for the -h or -v opt
	for (int i = 1; i < argc; i++) {
		if (!strcmp (argv[i], "-h")) {
			help ();
			return 0;
		} else if (!strcmp (argv[i], "-v")) {
			version ();
			return 0;
		}
	}

	banner ();

	// If we are here, no options were given.
	char *filename = argv[1];
	if (!filename) {
		printf ("Unable to read filename, abort\n");
		return 0;
	}

	// Create a file struct
	file_t *file = file_load (filename);
	unsigned char *fdata = (unsigned char *) file_load_bytes (file, file->size, 0);
	uint32_t size = file->size;


	// Check that the file is a Universal Binary
	mach_header_type_t type = mach_header_verify (fdata);
	if (FAT(fdata)) {
		// just continue
	} else {
		printf ("File is not a Universal Binary. macho_split is designed for Universal Binaries\n");
		return 0;
	}


	// Try to detect the architectures contained within the file
	fat_header_info_t *fat_info = mach_universal_load (file);

	// Create a list of archs to write to a file
	HSList *arch_list = NULL;

	// Go through each arch, print details in `file` format, then split and write each one
	for (int i = 0; i < h_slist_length (fat_info->archs); i++) {

		// Check the arch
		struct fat_arch *arch = (struct fat_arch *) h_slist_nth_data (fat_info->archs, i);
		char *arch_name = NULL;

		// Check for arm64e
		if (arch->cputype == CPU_TYPE_ARM64) {
			if (arch->cpusubtype == CPU_SUBTYPE_ARM64E) arch_name = "arm64e";
			else arch_name = "arm64";
		} else {
			arch_name = mach_header_read_cpu_type (arch->cputype);
		}

		// Print the arch info.
		mach_header_t *hdr = malloc (sizeof (mach_header_t));
		memset (hdr, '\0', sizeof (mach_header_t));
		memcpy (hdr, fdata + arch->offset, sizeof (mach_header_t));

		if (hdr->magic == MACH_MAGIC_64 || hdr->magic == MACH_CIGAM_64) {
       		printf ("\t%s (for architecture %s):\tMach-O 64-bit %s %s\n", file->path, arch_name, mach_header_read_file_type_short (hdr->filetype), arch_name);
       	} else if (hdr->magic == MACH_MAGIC_32 || hdr->magic == MACH_CIGAM_32) {
           	printf ("\t%s (for architecture %s):\tMach-O 32-bit %s %s\n", file->path, arch_name, mach_header_read_file_type_short (hdr->filetype), arch_name);
       	} else {
           	printf ("\tunknown: 0x%x\n", hdr->magic);
       	}

		// Add to the archs list
		struct archs *tmp = malloc (sizeof(struct archs));
		tmp->size = arch->size;
		tmp->name = arch_name;

		tmp->buf = malloc (arch->size);
		memset (tmp->buf, '\0', arch->size);
		memcpy (tmp->buf, fdata + arch->offset, arch->size);

		arch_list = h_slist_append (arch_list, tmp);
		arch_name = NULL;
	}


	// Now write each extracted arch to a file
	for (int i = 0; i < (int) h_slist_length (arch_list); i++) {
		struct archs *tmp = (struct archs *) h_slist_nth_data (arch_list, i);

		// Create filename
        char outname[25];
        snprintf (outname, 25, "macho_split%02d.%s", i, tmp->name);
		printf ("[*] %s", outname);

		FILE *fptr = fopen (outname, "wb");
		fwrite (tmp->buf, tmp->size, 1, fptr);
		fclose (fptr);

		printf (" \t...done\n");
	}


	return 0;
}
