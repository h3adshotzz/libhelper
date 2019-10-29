/**
 *     libhelper
 *     Copyright (C) 2019, @h3adsh0tzz
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include "libhelper.h"
#include "file.h"


#define MACH_MAGIC_32   0xfeedface
#define MACH_MAGIC_64   0xfeedfacf

typedef struct mach_header_t {
    uint32_t	magic;		/* mach magic number identifier */
	uint32_t	cputype;	/* cpu specifier */
	uint32_t	cpusubtype;	/* machine specifier */
	uint32_t	filetype;	/* type of file */
	uint32_t	ncmds;		/* number of load commands */
	uint32_t	sizeofcmds;	/* the size of all the load commands */
	uint32_t	flags;		/* flags */
} mach_header_t;

typedef struct macho_t {
    macho_header_t* header;         /* macho header */
	//macho_symtab_t** symtabs;
	//macho_command_t** commands;
	//macho_segment_t** segments;
    uint8_t* data;                  /* data */
	uint32_t size;                  /* file size */ 
	uint32_t offset;
	uint32_t command_count;
	uint32_t segment_count;
	uint32_t symtab_count;
} macho_t;


macho_t* macho_create ();