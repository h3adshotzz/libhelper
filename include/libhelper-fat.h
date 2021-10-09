//===----------------------------------------------------------------------===//
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
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//  Copyright (C) 2020, Is This On?, @h3adsh0tzz
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//
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
//  NOTE: The licenses from both Mach and Apple are included as the 
//  definitions from this file are included in the macOS SDK and XNU
//  source code. These are redefined here to allow for both Linux
//  support and the use of the library without including system headers.
//

#ifndef __LIBHELPER_FAT_H__
#define __LIBHELPER_FAT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <libhelper-hlibc.h>

/**
 *  If LIBHELPER_MACHO_USE_SYSTEM_HEADERS is defined, we can use the system
 *  loader.h and any other required headers. 
 * 
 *  This is not advised as changes made to system headers could affect libhelper
 *  if certain changes are made to structures and definitions. 
 */
#ifdef LIBHELPER_MACHO_USE_SYSTEM_HEADERS
#   include <mach/loader.h>
#else
#   include "macho/loader.h"
#endif

//
//  NOTE: Unlike with loader.h, fat.h has a small amount of definitions,
//  so they are all defined here, no need to import the system fat.h.
//

/**
 *  \brief      Check whether a given pointer, `p`, is a FAT magic.
 */
#define FAT(p)                          ((*(unsigned int *)(p) & ~1) == 0xbebafeca)

/**
 *  FAT/Universal Magic Values.
 * 
 *  Like Mach-O's, the magic values can be in either order. Define both
 *  here.
 * 
 */
#define FAT_MAGIC                       0xcafebabe
#define FAT_CIGAM                       0xbebafeca

/**
 *  \brief      FAT File Header (Universal Binary). The FAT File Header for Universal 
 *              Binaries. This appears at the top of a universal file, with a summary 
 *              of all the architectures contained within it.
 */
typedef struct fat_header               fat_header_t;
struct fat_header {
    uint32_t        magic;          /* FAT_MAGIC */
    uint32_t        nfat_arch;      /* number of fat_arch structs that follow */
};

/**
 *  \brief      FAT Architecture Header. The fat_arch struct defines an architecture 
 *              contained within a FAT archive, these follow directly after the fat 
 *              header in a file.
 */
typedef struct fat_arch                 fat_arch_t;
struct fat_arch {
    cpu_type_t      cputype;        /* cpu specifier for this architecture */
    cpu_subtype_t   cpusubtype;     /* cpu subtype specifier for this architecture */
    uint32_t        offset;         /* offset of where the Mach-O begins in the file */
    uint32_t        size;           /* size of the Mach-O */
    uint32_t        align;          /* byte align */
};

/**
 *  \brief      FAT Archive Info. This libhelper wrapper contains the FAT/Universal
 *              Binaries Header & architecture descriptors.
 */
typedef struct __libhelper_fat_info     fat_info_t;
struct __libhelper_fat_info {
    fat_header_t    *header;        /* FAT/Universal Binary Header */
    HSList          *archs;         /* list of contained architectures (fat_arch_t) */
};


fat_info_t *
macho_universal_load (const char *filename);

/* redefine funcs from libhelper-2.0.0 */


#ifdef __cplusplus
}
#endif

#endif /* __libhelper_fat_h__ */
