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
 * 	Mach-O header.
 * 
 * 	parts of machine.h need to be implemented so cputype and cpusubtype
 * 	can be represented as cpu_type_t and cpu_subtype_t.
 */
struct mach_header_64_t {
	uint32_t	magic;		/* mach magic number identifier */
	uint32_t	cputype;	/* cpu specifier */
	uint32_t	cpusubtype;	/* machine specifier */
	uint32_t	filetype;	/* type of file */
	uint32_t	ncmds;		/* number of load commands */
	uint32_t	sizeofcmds;	/* the size of all the load commands */
	uint32_t	flags;		/* flags */
	uint32_t	reserved;	/* reserved */
};


/**
 * 	Mach-O 64bit magic
 * 
 */
#define MACH_MAGIC_64		0xfeedfacf	/* 64bit magic number */
#define MACH_CIGAM_64		0xcffaedfe	/* NXSwapInt */



/**
 * 	Mach-O parser function definitiosn
 * 	
 */
uint32_t macho_read_magic (unsigned char *buf, int offset);