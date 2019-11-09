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


/**
 * 	Load command definitions. Mach-O's use these to map out the file structure, like
 * 	specifying where segment data, symbol tables, etc are located. These are simply 
 * 	the names, a structure definitions is later on.
 * 
 */
#define	LC_SEGMENT			0x1		/* segment of this file to be mapped */
#define	LC_SYMTAB			0x2		/* link-edit stab symbol table info */
#define	LC_SYMSEG			0x3		/* link-edit gdb symbol table info (obsolete) */
#define	LC_THREAD			0x4		/* thread */
#define	LC_UNIXTHREAD		0x5		/* unix thread (includes a stack) */
#define	LC_LOADFVMLIB		0x6		/* load a specified fixed VM shared library */
#define	LC_IDFVMLIB			0x7		/* fixed VM shared library identification */
#define	LC_IDENT			0x8		/* object identification info (obsolete) */
#define LC_FVMFILE			0x9		/* fixed VM file inclusion (internal use) */
#define LC_PREPAGE      	0xa     /* prepage command (internal use) */
#define	LC_DYSYMTAB			0xb		/* dynamic link-edit symbol table info */
#define	LC_LOAD_DYLIB		0xc		/* load a dynamically linked shared library */
#define	LC_ID_DYLIB			0xd		/* dynamically linked shared lib ident */
#define LC_LOAD_DYLINKER 	0xe		/* load a dynamic linker */
#define LC_ID_DYLINKER		0xf		/* dynamic linker identification */
#define	LC_PREBOUND_DYLIB 	0x10	/* modules prebound for a dynamically */
#define	LC_ROUTINES			0x11	/* image routines */
#define	LC_SUB_FRAMEWORK 	0x12	/* sub framework */
#define	LC_SUB_UMBRELLA 	0x13	/* sub umbrella */
#define	LC_SUB_CLIENT		0x14	/* sub client */
#define	LC_SUB_LIBRARY  	0x15	/* sub library */
#define	LC_TWOLEVEL_HINTS 	0x16	/* two-level namespace lookup hints */
#define	LC_PREBIND_CKSUM  	0x17	/* prebind checksum */

#define LC_SEGMENT_64		0x19	/* 64-bit segment of this file to be mapped */


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
	uint32_t cmd;		/* type of load command */
	uint32_t cmdsize;	/* total size of command in bytes */
} mach_load_command_t;


/**
 * 	Segment Load Commands, LC_SEGMENT and LC_SEGMENT_64, indicate a part of the Mach-O
 * 	to be mapped into a tasks address space. 
 * 
 */
typedef int		vm_prot_t;
typedef struct mach_segment_command_64_t {
	uint32_t	cmd;
	uint32_t	cmdsize;
	char		segname[16];
	uint64_t	vmaddr;
	uint64_t	vmsize;
	uint64_t	fileoff;
	uint64_t	filesize;
	vm_prot_t	maxprot;
	vm_prot_t	initprot;
	uint32_t	nsects;
	uint32_t	flags;
} mach_segment_command_64_t;


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
 * 	Mach Header functions
 * 
 */
mach_header_t *mach_header_create ();
mach_header_t *mach_header_load ();
uint32_t mach_read_magic (unsigned char *buf, int offset);
char *mach_header_read_cpu_type_string (cpu_type_t type);
char *mach_header_read_type_string (uint32_t type);

void mach_header_dump_test (mach_header_t *header);


/**
 * 
 */
mach_load_command_t *mach_load_command_create ();
mach_load_command_t *mach_load_command_load (file_t *file, off_t offset);
void mach_load_command_dump (mach_load_command_t *lc);

#endif /* MACH_O_H */