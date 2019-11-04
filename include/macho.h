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

#include "libhelper.h"
#include "file.h"


/**
 * 	Part of the point of this is that I want to implement Mach-O
 * 	loading / parsing independently, i.e. without relying on AAPL
 * 	headers like loader.h. This would (I think?) allow this to run
 * 	without much modification on Linux.
 * 
 * 	So, yeah, I'm basically rewriting loader.h.
 * 
 * 
 * 	Another note, for now I'm only implementing 64-bit, since iOS and
 * 	macOS no longer support anything compiled for 32bit archs.
 */


/**
 * 	Redefinitions for machine.h. Again, this is to allow compatibility
 * 	with operating systems other than Linux. 
 * 
 * 	I'm only bothering with ARM and x86 cpu types. 
 * 
 */

/*
 * Capability bits used in the definition of cpu_type.
 */
#define CPU_ARCH_MASK           0xff000000      /* mask for architecture bits */
#define CPU_ARCH_ABI64          0x01000000      /* 64 bit ABI */
#define CPU_ARCH_ABI64_32       0x02000000      /* ABI for 64-bit hardware with 32-bit types; LP32 */


/**
 * 	cpu_type_t definition. Originally machine.h
 * 
 */
typedef enum cpu_type_t {
	CPU_TYPE_ANY = -1,

	CPU_TYPE_X86 = 6,
	CPU_TYPE_X86_64 = 0x01000007,

	CPU_TYPE_ARM = 12,
	CPU_TYPE_ARM64 = (CPU_TYPE_ARM | CPU_ARCH_ABI64),
	CPU_TYPE_ARM64_32 = (CPU_TYPE_ARM | CPU_ARCH_ABI64_32),

} cpu_type_t;


/**
 * 	Mach-O header.
 * 
 * 	parts of machine.h need to be implemented so cputype and cpusubtype
 * 	can be represented as cpu_type_t and cpu_subtype_t.
 */
typedef struct mach_header_t {
	uint32_t	magic;		/* mach magic number identifier */
	cpu_type_t	cputype;	/* cpu specifier */
	uint32_t	cpusubtype;	/* machine specifier */
	uint32_t	filetype;	/* type of file */
	uint32_t	ncmds;		/* number of load commands */
	uint32_t	sizeofcmds;	/* the size of all the load commands */
	uint32_t	flags;		/* flags */
	uint32_t	reserved;	/* reserved */
} mach_header_t;


/**
 * 	Mach-O file type specifiers
 * 
 */
#define MACH_TYPE_OBJECT 	0x1
#define MACH_TYPE_EXECUTE	0x2

#define MACH_TYPE_DYLIB		0x6
/* Add more */


/**
 * 	Mach-O 64bit magic
 * 
 */
#define MACH_MAGIC_64		0xfeedfacf	/* 64bit magic number */
#define MACH_CIGAM_64		0xcffaedfe	/* NXSwapInt */

/**
 * 
 */
typedef struct mach_load_command_t {
	uint32_t cmd;		/* type of load command */
	uint32_t cmdsize;	/* total size of command in bytes */
} mach_load_command_t;

/**
 * 
 */
typedef struct mach_segment_command_t {
	uint32_t	cmd;
	uint32_t	cmdsize;
	char		segname[16];
	uint32_t	vmaddr;
	uint32_t	vmsize;
	uint32_t	fileoff;
	uint32_t	filesize;
	uint32_t	maxprot;
	uint32_t	initprot;
	uint32_t	nsects;
	uint32_t	flags;
} mach_segment_command_t;


/**
 * 
 */
typedef struct macho_t {
	mach_header_t *header;
	
} macho_t;



/**
 * 	Mach-O parser function definitiosn
 * 	
 */
uint32_t macho_read_magic (unsigned char *buf, int offset);
mach_header_t *mach_header_load (unsigned char *buf);
char *mach_header_read_cpu_type (cpu_type_t type);
char *mach_header_read_file_type (uint32_t type);
void mach_header_dump_test (mach_header_t *header);