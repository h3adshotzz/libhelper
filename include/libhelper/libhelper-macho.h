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
/*
 * Copyright (c) 2007-2016 Apple, Inc. All rights reserved.
 * Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */

//
//  NOTE: The licenses from both Mach and Apple are included as many of the
//      definitions within this header are taken from the `mach/` directory
//      of the macOS SDK.
//

#ifndef LIBHELPER_MACHO_H
#define LIBHELPER_MACHO_H

#ifdef cplusplus
extern "C" {
#endif
	
/**
 *                  === The Libhelper Project ===
 *                          Mach-O Parser
 *
 *	Libhelper Mach-O Parser is a standalone Mach-O Parser for all platfors
 *	and architectures supported by libhelper. By standalone, it means that
 *	it doesn't require any system Mach-O headers, so can run easily on linux.
 *
 *	The Mach-O parser in this version (libhelper-2000.16.4 and up) differs
 *	from the previous versions as it's aimed at being faster, more reliable
 *	and more efficient.
 *
 *  ----------------
 *  Original Author:
 *      Harry Moulton, @h3adsh0tzz  -   me@h3adsh0tzz.com.
 *
 */

#include <stdint.h>
	
#include "macho/header-types.h"
#include "libhelper.h"
	
typedef int                 cpu_type_t;
typedef int                 cpu_subtype_t;
typedef int                 cpu_threadtype_t;
	
/**
 *	Linux does not have OSSwapInt32(), instead is has bswap_32. If the
 *	build platform is Linux redefine bswap_32 as OSSwapInt32 and include
 *	byteswap.h/
 *
 */
#ifdef __APPLE__
#	define OSSwapInt32(x) 	 _OSSwapInt32(x)
#else
#	define OSSwapInt32(x)	bswap_32(x)
#endif

	
/**
 *  Capability bits used in the definition of cpu_type. These are used to
 *  calculate the value of the 64bit CPU Type's by performing a logical OR
 *  between the 32bit variant, and the architecture mask.
 *
 *      E.g. CPU_TYPE_ARM64 = (CPU_TYPE_ARM | CPU_ARCH_ABI64)
 *
 */
#define CPU_ARCH_MASK           0xff000000          /* mask for architecture bits */
#define CPU_ARCH_ABI64          0x01000000          /* 64 bit ABI */
#define CPU_ARCH_ABI64_32       0x02000000          /* ABI for 64-bit hardware with 32-bit types.

#define CPU_SUBTYPE_MASK        0xff000000          /* mask for feature flags */
#define CPU_SUBTYPE_LIB64       0x80000000          /* 64 bit libraries */
#define CPU_SUBTYPE_PTRAUTH_ABI 0x80000000          /* pointer authentication (PAC) with versioned ABI */


/**
 *  Machine types. There are many more of these defined in `machine.h`, however
 *  we do not need them all as libhelper supports x86_64 and arm64. There
 *  is also support for working with 32-bit ARM files, despite libhelper not
 *  being compatible build-wise.
 * 
 */
#define CPU_TYPE_ANY            ((cpu_type_t) -1)

#define CPU_TYPE_X86            ((cpu_type_t) 7)
#define CPU_TYPE_X86_64         (CPU_TYPE_X86 | CPU_ARCH_ABI64)

#define CPU_TYPE_ARM            ((cpu_type_t) 12)
#define CPU_TYPE_ARM64          (CPU_TYPE_ARM | CPU_ARCH_ABI64)
#define CPU_TYPE_ARM64_32       (CPU_TYPE_ARM | CPU_ARCH_ABI64_32)


/**
 *  Machine subtypes. The case is the same as with machine types, they're taken
 *  from the `machine.h` header. As we only defined x86 and ARM cpu types, not
 *  all CPU types will be required here.
 * 
 */
#define CPU_SUBTYPE_ANY         ((cpu_subtype_t) -1)


/**
 *  x86 subtypes
 */
#define CPU_SUBTYPE_X86_ALL     ((cpu_subtype_t) 3)
#define CPU_SUBTYPE_X86_64_ALL  ((cpu_subtype_t) 3)
#define CPU_SUBTYPE_X86_ARCH1   ((cpu_subtype_t) 4)
#define CPU_SUBTYPE_X86_64_H    ((cpu_subtype_t) 8)     /* Haswell feature subset */


/**
 *  ARM subtypes
 */
#define CPU_SUBTYPE_ARM_ALL             ((cpu_subtype_t) 0)
#define CPU_SUBTYPE_ARM_V4T             ((cpu_subtype_t) 5)
#define CPU_SUBTYPE_ARM_V6              ((cpu_subtype_t) 6)
#define CPU_SUBTYPE_ARM_V5TEJ           ((cpu_subtype_t) 7)
#define CPU_SUBTYPE_ARM_XSCALE          ((cpu_subtype_t) 8)
#define CPU_SUBTYPE_ARM_V7              ((cpu_subtype_t) 9)  /* ARMv7-A and ARMv7-R */
#define CPU_SUBTYPE_ARM_V7F             ((cpu_subtype_t) 10) /* Cortex A9 */
#define CPU_SUBTYPE_ARM_V7S             ((cpu_subtype_t) 11) /* Swift */
#define CPU_SUBTYPE_ARM_V7K             ((cpu_subtype_t) 12)
#define CPU_SUBTYPE_ARM_V8              ((cpu_subtype_t) 13)
#define CPU_SUBTYPE_ARM_V6M             ((cpu_subtype_t) 14) /* Not meant to be run under xnu */
#define CPU_SUBTYPE_ARM_V7M             ((cpu_subtype_t) 15) /* Not meant to be run under xnu */
#define CPU_SUBTYPE_ARM_V7EM            ((cpu_subtype_t) 16) /* Not meant to be run under xnu */
#define CPU_SUBTYPE_ARM_V8M             ((cpu_subtype_t) 17) /* Not meant to be run under xnu */

/**
 *  ARM64 subtypes
 */
#define CPU_SUBTYPE_ARM64_ALL           ((cpu_subtype_t) 0)
#define CPU_SUBTYPE_ARM64_V8            ((cpu_subtype_t) 1)
#define CPU_SUBTYPE_ARM64E              ((cpu_subtype_t) 2)


/**
 *  CPU subtype feature flags for ptrauth on arm64 platforms
 * 
 *  (I am unsure how these work, but they might be useful)
 * 
 */
#define CPU_SUBTYPE_ARM64_PTR_AUTH_MASK 0x0f000000
#define CPU_SUBTYPE_ARM64_PTR_AUTH_VERSION(x) (((x) & CPU_SUBTYPE_ARM64_PTR_AUTH_MASK) >> 24)
	

/***********************************************************************
* Mach-O Header.
*
*	Redefinitions of values specific to the Mach-O Header
*
************************************************************************/

/**
 *  Mach-O header type. These are defined in `macho/header-types.h`
 *  for libhelper and `loader.h` for the macOS SDK.
 * 
 */
typedef int         mach_header_type_t;

#define MH_TYPE_UNKNOWN         ((mach_header_type_t) -1)
#define MH_TYPE_MACHO64         ((mach_header_type_t) 1)
#define MH_TYPE_MACHO32         ((mach_header_type_t) 2)
#define MH_TYPE_FAT             ((mach_header_type_t) 3)

/**
 *  Mach-O Magic's
 * 
 *  There are three types of Mach-O magic's: 64 bit, 32 bit and Universal
 *  Binary. With macOS 11, it is possible the Universal Binary has changed
 *  in format for cross-platform Apple Silicon applications.
 *
 */
#define MACH_MAGIC_64           0xfeedfacf      /* 64bit magic number */
#define MACH_CIGAM_64           0xcffaedfe      /* NXSwapInt */

#define MACH_MAGIC_32           0xfeedface      /* 32bit magic number */
#define MACH_CIGAM_32           0xcefaedfe      /* NXSwapInt */

#define MACH_MAGIC_UNIVERSAL    0xcafebabe      /* Universal Binary magic number */
#define MACH_CIGAM_UNIVERSAL    0xbebafeca      /* NXSwapInt */


/**
 *  Mach-O type specifiers.
 * 
 *  These describe the type of Mach-O a particular file is, as the format
 *  of each type varies slightly, as an executable will not need the same
 *  structure as a dynamic library.
 * 
 *  NOTE: Not all of the types are defined here. As libhelper supports more,
 *  they will be added here.
 */
#define MACH_TYPE_UNKNOWN       0x0             /* unknown Mach type */

#define MACH_TYPE_OBJECT        0x1             /* object file */
#define MACH_TYPE_EXECUTE       0x2             /* executable file */
#define MACH_TYPE_FVMLIB        0x3             /* fixed vm shared library */
#define MACH_TYPE_CORE          0x4             /* core file */
#define MACH_TYPE_PRELOAD       0x5             /* preloaded executable file */
#define MACH_TYPE_DYLIB         0x6             /* dynamic library */
#define MACH_TYPE_DYLINKER      0x7             /* dynamic link editor */
#define MACH_TYPE_BUNDLE        0x8             /* dynamic bundle file */
#define MACH_TYPE_DYLIB_STUB    0x9             /* shared library stub for static linking */
#define MACH_TYPE_DSYM          0xa             /* debugging companion file */
#define MACH_TYPE_KEXT_BUNDLE   0xb             /* x86_64 KEXT */
#define MACH_TYPE_FILESET       0xc             /* file composed of other Mach-O's */


/**
 *  Mach-O Header flags
 * 
 *  NOTE: Not all are implemented.
 */ 
//...


/**
 *  Mach-O Header's
 * 
 *  The header is placed at the very top of a Mach-O file. There are two separate
 *  headers for 32bit and 64bit Mach-O's (Universal Binary/FAT archives are different).
 * 
 *  The differences between the 32 and 64 bit headers is the presence of the `reserved`
 *  property.
 * 
 *  NOTE: Compatibility issue with pre-libhelper-2000.16.4. The naming of the default
 *          mach_header_xx struct is different, but the alias's are the same.
 * 
 */
struct mach_header_64 {
    uint32_t        magic;          /* mach magic number */
    cpu_type_t      cputype;        /* cpu specifier */
    cpu_subtype_t   cpusubtype;     /* cpu subtype specifier */
    uint32_t        filetype;       /* mach filetype, e.g. MACH_TYPE_OBJECT */
    uint32_t        ncmds;          /* number of load commands */
    uint32_t        sizeofcmds;     /* size of load command region */
    uint32_t        flags;          /* flags */
    uint32_t        reserved;       /* *64 bit only* reserved */
};
// libhelper-macho alias
typedef struct mach_header_64       mach_header_t;

/**
 *  Mach-O Header (32 bit)
 * 
 *  To keep some sort of compatibility with code using the SDK `loader.h`, the original
 *  struct names are used (so the 64 bit header is specified, whereas 32 bit is simply
 *  mach_header). The libhelper alias reverts this, instead specifying 32 bit.
 * 
 */
struct mach_header {
    uint32_t        magic;          /* mach magic number */
    cpu_type_t      cputype;        /* cpu specifier */
    cpu_subtype_t   cpusubtype;     /* cpu subtype specifier */
    uint32_t        filetype;       /* mach filetype, e.g. MACH_TYPE_OBJECT */
    uint32_t        ncmds;          /* number of load commands */
    uint32_t        sizeofcmds;     /* size of load command region */
    uint32_t        flags;          /* flags */
    uint32_t        reserved;       /* *64 bit only* reserved */
};
// libhelper-macho alias
typedef struct mach_header          mach_header_32_t;


/**
 *  Mach-O file structure. Contains all parsed properties of a Mach-O file, and some
 *  other raw properties.
 * 
 *  There is no equiv for 32 bit yet, that is a TODO.
 */
struct __libhelper_macho {
    /* file data */
    char            *path;          /* filepath */

    /* raw file properties */
    uint8_t         *data;          /* pointer to mach-o in memory */
    uint32_t         size;          /* size of mach-o */
    uint32_t         offset;        /* base_addr + sizeof(mach_header) */

    /* mach-o parsed properties */
    mach_header_t   *header;        /* mach-o header */
    HSList          *lcmds;         /* list of all load commands (including LC_SEGMENT) */
    HSList          *scmds;         /* list of segment commands */
    HSList          *dylibs;        /* list of dynamic libraries */
    HSList          *symbols;       /* list of symbols */
    HSList          *strings;       /* list of strings */
};
typedef struct __libhelper_macho            macho_t;


/**
 *  Mach-O Header functions
 * 
 */
extern mach_header_t            *mach_header_create                 ();
extern mach_header_t            *mach_header_load                   (macho_t *macho);

extern mach_header_type_t        mach_header_verify                 (uint32_t magic);

extern char                     *mach_header_read_cpu_type          (cpu_type_t type);
extern char                     *mach_header_read_cpu_subtype       (cpu_subtype_t type);
extern char                     *mach_header_read_file_type         (uint32_t type);
extern char                     *mach_header_read_file_type_short   (uint32_t type);

/**
 *  Mach-O Parser functions
 */
extern macho_t                  *macho_create                       ();
extern macho_t                  *macho_create_from_buffer           (unsigned char *data);

extern macho_t                  *macho_load                         (const char *filename);
extern void                     *macho_load_bytes                   (macho_t *macho, size_t size, uint32_t offset);

// TODO: NOTE: MUST MOVE TO SEPARATE HEADER
#define FAT(p) ((*(unsigned int *)(p) & ~1) == 0xbebafeca)

/***********************************************************************
* Mach-O Load Commands.
*
*	Redefinitions of values specific to Mach-O Load Commands, and 
*   libhelpers parsing of those Load Commands.
*
************************************************************************/

/**
 *  Mach-O Generic Load Command.
 * 
 *  Load commands directly follow the Mach-O header and can vary in size and
 *  structure. However every command has the same first 16 bytes which define
 *  the command type and it's size.
 * 
 *  The `load_command` structure, and the libhelper alias, are not architecture
 *  specific (neither are most LCs).
 * 
 */
struct load_command {
    uint32_t        cmd;            /* load command type */
    uint32_t        cmdsize;        /* load command size */
};
// libhelper-macho alias
typedef struct load_command         mach_load_command_t;


/**
 *  Mach-O Load Command Info.
 * 
 *  Used as a wrapper around a `mach_load_command_t`, this provides a few more
 *  bits of information about a load command, such as it's index in the LC list,
 *  and the offset of the command within the Mach-O.
 * 
 */
#warning "The use of `type` within __libhelper_mach_command_info is deprecated and will soon be removed!"
struct __libhelper_mach_command_info {
    mach_load_command_t     *lc;    /* load command */

    uint32_t        offset;         /* offset in the Mach-O */
    uint32_t        index;          /* index in the LC list */
    uint32_t        type;           /* type of LC (DEPRECATED) */
};
// libhelper-macho alias
typedef struct __libhelper_mach_command_info    mach_load_command_info_t;

#define LC_RAW      0x0
#define LC_INFO     0x1

/**
 *  Mach-O Load Command functions
 */
extern mach_load_command_t      *mach_load_command_create           ();

extern mach_load_command_info_t *mach_load_command_info_create      ();
extern mach_load_command_info_t *mach_load_command_info_load        (unsigned char *data, uint32_t offset);

#warning "The use of mach_load_command_info_print() and mach_load_command_print() is deprecated and will soon be removed"
extern void                      mach_load_command_info_print       (mach_load_command_info_t *cmd);
extern void                      mach_load_command_print            (void *cmd, int flag);
extern char                     *mach_load_command_get_string       (mach_load_command_t *lc);




#ifdef cplusplus
}
#endif

#endif /* libhelper_macho_h */
