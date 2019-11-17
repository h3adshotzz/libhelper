/**
 * 
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

#ifndef MACH_O_H
#define MACH_O_H

#include "libhelper.h"
#include "file.h"

#include <ctype.h>


/**	
 * 	This file contains definitions for different aspects of loading and parsing
 * 	Mach-O files. Some structures and macros in this header are originally defined 
 * 	in mach/loader.h, which is shipped with macOS. There are two reasons for this.
 * 
 * 		(1) 	I wanted to properly learn how Mach-O's are structured, and how they
 * 				work. I'll probably write a blog post on Mach-O files to go along
 * 				with this project. I felt that by writing everything myself, and not
 * 				relying on system headers, I'd gain a greater understanding of how
 * 				things work.
 * 
 * 		(2)		I'm aiming for this library to be cross-compatible with Linux and iOS
 * 				which do not have Mach headers by default. The aim is that this should
 * 				compile without any extra dependencies. 
 * 
 * 	So, I'm basically rewriting loader.h. I might change a few names of things, but
 * 	nothing significant and I'll note where I do.
 * 
 * 	Another note, to start with I'm only implementing 64bit, since there isn't much
 * 	32bit stuff floating around nowadays due to *OS removing all support. Also, the
 * 	only architectures that will be supported are x86_64 and ARM64.
 * 
 */



//////////////////////////////////////////////////////////////////////////
//                    Mach-O File and Headers                           //
//////////////////////////////////////////////////////////////////////////

/**
 * 	Capability bits used in the definition of cpu_type. These are used to
 * 	calculate the value of the 64bit CPU Type's by performing a logical OR
 * 	between the 32bit variant, and the architecture mask.
 * 
 * 		E.g. CPU_TYPE_ARM64 = (CPU_TYPE_ARM | CPU_ARCH_ABI64)
 * 
 */
#define CPU_ARCH_MASK           0xff000000      /* mask for architecture bits */
#define CPU_ARCH_ABI64          0x01000000      /* 64 bit ABI */
#define CPU_ARCH_ABI64_32       0x02000000      /* ABI for 64-bit hardware with 32-bit types; LP32 */


/**
 * 	Mach-O file type specifiers. Mach-O's can be executables, objects, dynamic
 * 	libraries, etc. 
 * 
 * 	The layout of the file is dependent on the type of Mach-O. For all types
 * 	excluding the MACH_TYPE_OBJECT, or MH_OBJECT as defined in loader.h, the
 * 	Segments are padded out and aligned on a segment alignment boundary.
 * 
 * 	The MACH_TYPE_EXECUTE, MACH_TYPE_FVMLIB, MACH_TYPE_DYLIB, MACH_TYPE_DYLINKER,
 * 	and MACH_TYPE_BUNDLE all have the headers included as part of their first
 * 	segment.
 * 
 * 	MACH_TYPE_OBJECT is intended as output of the assembler and input, or output,
 * 	of the linker. An example of this is when one compiles each source file induvidually
 * 	to get a number of .o files, then linking them all together.
 * 
 * 	Over time I will add more of these MACH_TYPE definitions as I add support for
 * 	them.
 *
 */
#define MACH_TYPE_UNKNOWN		0x0

#define MACH_TYPE_OBJECT		0x1
#define MACH_TYPE_EXECUTE		0x2

#define MACH_TYPE_DYLIB			0x6


/**
 * 	Mach-O 64bit magic 
 * 
 */
#define MACH_MAGIC_64			0xfeedfacf	/* 64bit magic number */
#define MACH_CIGAM_64			0xcffaedfe	/* NXSwapInt */


/**
 * 	Size of a Mach-O and Mach-O Header
 * 
 */
#define MACH_HEADER_SIZE		sizeof(mach_header_t)
#define MACH_O_SIZE				sizeof(macho_t)


/**
 * 	Definition of cpu_type_t, originally defined in machine.h. Contains types
 * 	for different CPU's.
 * 
 */
typedef enum cpu_type_t {
	CPU_TYPE_ANY = -1,

	CPU_TYPE_X86 = 6,
	CPU_TYPE_X86_64 = 0x01000007,

	CPU_TYPE_ARM = 12,
	CPU_TYPE_ARM64 = (CPU_TYPE_ARM | CPU_ARCH_ABI64),
	CPU_TYPE_ARM64_32 = (CPU_TYPE_ARM | CPU_ARCH_ABI64_32)
} cpu_type_t;


/**
 * 	Definition of cpu_subtype_t, originally defined in machine.h. Contains types
 * 	for different CPU Subtypes's.
 * 
 * 	TODO!
 */
typedef enum cpu_subtype_t {
	CPU_SUBTYPE_ANY = -1
} cpu_subtype_t;


/**
 * 	Mach-O file header for 64bit. This appears at the very top of Mach-O files
 * 	and sets out the format of the file, and how we should parse / load it.
 * 
 * 	We specify the 64 because 32bit support may be added at some point.
 * 	
 */
typedef struct mach_header_t {
	uint32_t		magic;			/* mach magic number identifier */
	cpu_type_t		cputype;		/* cpu specifier */
	cpu_subtype_t	cpusubtype;		/* machine specifier */
	uint32_t		filetype;		/* type of file */
	uint32_t		ncmds;			/* number of load commands */
	uint32_t		sizeofcmds;		/* the size of all the load commands */
	uint32_t		flags;			/* flags */
	uint32_t		reserved;		/* reserved */
} mach_header_t;


/**
 * 	Full Mach-O containing all properties of a file.
 * 		[document more]
 * 
 */
typedef struct macho_t {
	file_t 			*file;
	mach_header_t 	*header;
	GSList			*lcmds;
	GSList			*scmds;
} macho_t;


/**
 * 	Functions to safely create a macho_t structure and to load a Mach-O file
 * 	into a macho_t struct.
 * 
 */
macho_t *macho_create ();
macho_t *macho_load (file_t *file);


/**
 * 	Mach Header functions
 * 
 */
mach_header_t *mach_header_create ();
mach_header_t *mach_header_load ();
uint32_t mach_read_magic (unsigned char *buf, int offset);
char *mach_header_read_cpu_type_string (cpu_type_t type);
char *mach_header_read_type_string (uint32_t type);

void mach_header_dump_test (mach_header_t *header);


//////////////////////////////////////////////////////////////////////////
//                    Mach-O Segment Commands                           //
//////////////////////////////////////////////////////////////////////////


/**
 * 	Segment Load Commands, LC_SEGMENT and LC_SEGMENT_64, indicate a part of the Mach-O
 * 	to be mapped into a tasks address space. 
 * 
 */
typedef int		vm_prot_t;
typedef struct mach_segment_command_64_t {
	uint32_t	cmd;			/* LC_SEGMENT_64 */
	uint32_t	cmdsize;		/* includes sizeof section_64 structs */
	char		segname[16];	/* segment name */
	uint64_t	vmaddr;			/* memory address of this segment */
	uint64_t	vmsize;			/* memory size of this segment */
	uint64_t	fileoff;		/* file offset of this segment */
	uint64_t	filesize;		/* amount to map from the file */
	vm_prot_t	maxprot;		/* maximum VM protection */
	vm_prot_t	initprot;		/* initial VM protection */
	uint32_t	nsects;			/* number of sections in segment */
	uint32_t	flags;			/* flags */
} mach_segment_command_64_t;

typedef struct mach_segment_info_t {
	mach_segment_command_64_t 	*segcmd;		/* Segment command */
	GSList 						*sections;		/* List of sections */
} mach_segment_info_t;


/**
 * 
 */
mach_segment_command_64_t *mach_segment_command_create ();
mach_segment_command_64_t *mach_segment_command_load (file_t *file, off_t offset);
GSList *mach_segment_get_list (macho_t *mach);

mach_segment_info_t *mach_segment_info_create ();
mach_segment_info_t *mach_segment_info_load (file_t *file, off_t offset);
mach_segment_info_t *mach_segment_command_search (macho_t *mach, char *segname);
void mach_segment_command_dump (mach_segment_info_t *si);



//////////////////////////////////////////////////////////////////////////
//                            Mach-O Sections                           //
//////////////////////////////////////////////////////////////////////////


/**
 * 	A segment is made up of zero or more sections. CONT
 * 
 */
typedef struct mach_section_64_t {
	char		sectname[16];	/* name of this section */
	char		segname[16];	/* segment this section goes in */
	uint64_t	addr;			/* memory address of this section */
	uint64_t	size;			/* size in bytes of this section */
	uint32_t	offset;			/* file offset of this section */
	uint32_t	align;			/* section alignment (power of 2) */
	uint32_t	reloff;			/* file offset of relocation entries */
	uint32_t	nreloc;			/* number of relocation entries */
	uint32_t	flags;			/* flags (section type and attributes)*/
	uint32_t	reserved1;		/* reserved (for offset or index) */
	uint32_t	reserved2;		/* reserved (for count or sizeof) */
	uint32_t	reserved3;		/* reserved */
} mach_section_64_t;


/**
 * 
 */
mach_section_64_t *mach_section_create ();
mach_section_64_t *mach_section_load (file_t *file, off_t offset);
GSList *mach_sections_load_from_segment (macho_t *macho, mach_segment_command_64_t *seg);
void mach_section_dump (mach_section_64_t *section);


//////////////////////////////////////////////////////////////////////////
//                    	 Mach-O Load Commands                           //
//////////////////////////////////////////////////////////////////////////

/**
 * 	Include the Load Command definitions from macho-lc-types.h. They are defined there
 * 	to make this file more clean.
 * 
 */
#include "macho-lc-types.h"


/**
 * 	Flags for Load Command printing functions
 */	
#define	LC_RAW		0x0
#define LC_INFO		0x1


/**
 * 	Mach-O Load Command structure definition.
 * 
 * 	Load commands directly follow the Mach Header. The total size of the command area
 * 	is given by the `sizeofcmds` property in the header, and the number of commands as
 * 	`mcmds`. 
 * 
 * 	The first two properties of a Load Command are always the cmd, which represents a LC
 * 	type, and a Size. Based on the LC type the command can be parsed correctly. For example,
 * 	given the cmd is LC_SEGMENT_64, we know to copy sizeof(mach_segment_command_t) bytes
 * 	from the start offset of the LC into a mach_segment_command_t. 
 * 
 * 	The structure is not architecture-specific. It will work with both 32bit and 64bit
 * 	parsing.
 * 
 */
typedef struct mach_load_command_t {
	uint32_t cmd;					/* type of load command */
	uint32_t cmdsize;				/* total size of command in bytes */
} mach_load_command_t;


/**
 * 	Mach-O Load Command Info structure.
 * 
 * 	Used to carry the offset of the load command in the file with the load command structure.
 * 	I guess it is architecture specific, because the offset is a uint64_t.
 */
typedef struct mach_command_info_t {
	mach_load_command_t		*lc;		/* load command structure */
	uint32_t				type;		/* load command type */
	uint64_t				off;		/* offset within the file */
} mach_command_info_t;

#define MACH_LOAD_COMMAND_SIZE		sizeof(mach_load_command_t)
#define MACH_COMMAND_INFO_SIZE		sizeof(mach_command_info_t)


/*
 * The source_version_command is an optional load command containing
 * the version of the sources used to build the binary.
 */
typedef struct mach_source_version_command_t {
    uint32_t  cmd;						/* LC_SOURCE_VERSION */
    uint32_t  cmdsize;					/* 16 */
    uint64_t  version;					/* A.B.C.D.E packed as a24.b10.c10.d10.e10 */
} mach_source_version_command_t;


/**
 * 	Mach-O Load Commands - declared in command.c
 * 	
 */
mach_load_command_t *mach_load_command_create ();
mach_command_info_t *mach_command_info_create ();
mach_command_info_t *mach_command_info_load (file_t *file, off_t offset);

void mach_load_command_info_print (mach_command_info_t *cmd);
void mach_load_command_print (void *cmd, int flag);
char *mach_load_command_get_string (mach_load_command_t *lc);


/**
 * 	LC_SOURCE_VERSION functions
 * 
 */
mach_source_version_command_t *mach_lc_find_source_version_cmd (macho_t *macho);
char *mach_lc_source_version_string (mach_source_version_command_t *svc);

#endif /* mach_o_h */