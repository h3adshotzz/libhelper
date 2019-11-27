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
/*
 * Copyright (c) 1999-2010 Apple Inc.  All Rights Reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef LIBHELPER_MACHO_HEADER_H
#define LIBHELPER_MACHO_HEADER_H

/**
 *                  === The Libhelper Project ===
 *                          Mach-O Parser
 * 
 *  Documentation relating to the macho-header.h header file:           |
 *                                                                      |
 *      This header contains definitions and functions for handling 64-bit
 *      Mach-O files. Some code contained within this header is originally
 *      Apple's, so the APSL Open Source License is included above.
 * 
 *      == Structures
 *          
 *                                                                   |
 *                                                                      |
 * 
 *  ----------------
 *  Original Author:
 *      Harry Moulton, @h3adsh0tzz  -   me@h3adsh0tzz.com.
 * 
 */

/**
 *  [Development Notes & TODOs]
 * 
 *      (LOW PRIORITY)
 *      - Add types and handle cpu_subtype
 *      - Handle the header->flags property
 *      
 *  When the entire header functionality is complete, write the above
 *  documentation, which will also go on:
 * 
 *      https://h3adsh0tzz.com/docs/libhelper-macho/
 * 
 */

#include <stdint.h>
#include "consts/macho-header-const.h"
#include "file.h"

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
 *  Mach-O Magic's.
 * 
 *  There are three Magic Numbers we need to be concerned with. That is 
 *  the 32bit, 64bit and Universal Binary Magic Numbers.
 * 
 *  32-bit:     0xfeedface
 *  64-bit:     0xfeedfacf
 *  Uni Bin:    0xcafebabe
 * 
 *  [Note]
 *      Currently, libhelper-macho does not support 32-bit Mach-O's, nor
 *      the Universal Binary format, however, I do include detection so
 *      one can at least check if a binary is UB or 32-bit. More on this 
 *      is discussed in my blog post:
 * 
 *          https://h3adsh0tzz.com/blog/<postid>
 * 
 */
#define MACH_MAGIC_64			0xfeedfacf	    /* 64bit magic number */
#define MACH_CIGAM_64			0xcffaedfe	    /* NXSwapInt */

#define	MACH_MAGIC_32	        0xfeedface	    /* 32bit magic number */
#define MACH_CIGAM_32	        0xcefaedfe	    /* NXSwapInt */

#define MACH_MAGIC_UNIVERSAL    0xcafebabe      /* Universal Binary magic number */
#define MACH_CIGAM_UNIVERSAL    0xbebafeca      /* NXSwapInt */


/**
 *  Defines the size of the mach_header_t structure. Doing this prevents
 *  dozens of calls for `sizeof()`.
 *  
 */
#define MACH_HEADER_SIZE        sizeof(mach_header_t)


/**
 *  Redefinition of `cpu_type_t` and `cpu_subtype_t` for mach_header_t.
 * 
 *  These are originally defined in macho/machine.h, but I'd like to write
 *  this library in a way that's cross-compatible with systems that do not
 *  have these headers natively.
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

// TODO
typedef enum cpu_subtype_t {
	CPU_SUBTYPE_ANY = -1
} cpu_subtype_t;


/**
 *  Mach-O Header.
 * 
 *  Mach-O file header for 64bit. This appears at the very top of Mach-O files
 * 	and sets out the format of the file, and how we should parse / load it.
 * 
 * 	We specify the 64 because 32-bit support may be added at some point.
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


//////////////////////////////////////////////////////////////////////////
//                       Function Definitions                           //
//////////////////////////////////////////////////////////////////////////


/**
 *  Function:   mach_header_create
 *  ------------------------------------
 * 
 *  Creates a new Mach-O Header structure and assigns sufficient memory. 
 *  Should be called to safely create a new Mach-O Header structure.
 * 
 *  Returns:    A mach_header_t structure with sufficient allocated memory.
 */
mach_header_t       *mach_header_create ();


/**
 *  Function:   mach_header_load
 *  ----------------------------------
 * 
 *  Loads a raw Mach-O Header from a given offset in a verified Mach-O file, and
 *  returns the resulting structure.
 *  
 *  file:       The verified Mach-O file.
 * 
 *  Returns:    A verified Mach Header structure.
 */
mach_header_t       *mach_header_load (file_t *file);


/**
 *  Function:   mach_header_read_cpu_type
 *  -------------------------------------
 * 
 *  Returns a decoded string of header->cputype.
 * 
 *  type:       The cpu_type_t from the Mach-O Header.
 * 
 *  Returns:    Decoded CPU type String.
 */
char *mach_header_read_cpu_type (cpu_type_t type);


/**
 *  Function:   mach_header_read_file_type
 *  -------------------------------------
 * 
 *  Returns a decoded string of header->filetype.
 * 
 *  type:       The header->filetype uint32_t from the Mach-O Header.
 * 
 *  Returns:    Decoded header->filetype.
 */
char *mach_header_read_file_type (uint32_t type);


/**
 *  Function:   mach_header_print_summary
 *  -------------------------------------
 * 
 *  Outputs a summary of a given mach_header_t.
 * 
 *  header:     The Mach-O Header.
 * 
 */
void mach_header_print_summary (mach_header_t *header);


#endif /* libhelper_macho_header_h */