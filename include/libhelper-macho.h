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
//  Copyright (C) 2021, Is This On? Holdings
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

#ifndef __LIBHELPER_MACHO_H__
#define __LIBHELPER_MACHO_H__

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

/**
 *  Mach-O Magic's
 * 
 *  There are three formats we need magics for: 64-, 32-bit and Universal files.
 *  The magics for 64- and 32-bit are already defined in the loader.h, so just
 *  redefine them.
 * 
 *  The magic for the Universal/FAT files is 0xcafebabe, so we define that here.
 */
#define MACH_MAGIC_UNIVERSAL            0xcafebabe
#define MACH_MAGIC_64                   MH_MAGIC_64
#define MACH_MAGIC_32                   MH_MAGIC

#define MACH_CIGAM_UNIVERSAL            0xbebafeca
#define MACH_CIGAM_64                   MH_CIGAM_64
#define MACH_CIGAM_32                   MH_CIGAM

/**
 *  Mach-O Header Types.
 * 
 *  These are similar to the Mach magic's defined above, however they do not
 *  take into account the byte order, and can be passed more easily as a data
 *  type.
 */
typedef int                         mach_header_type_t;

#define MH_TYPE_UNKNOWN             ((mach_header_type_t) -1)
#define MH_TYPE_MACHO64             ((mach_header_type_t) 1)
#define MH_TYPE_MACHO32             ((mach_header_type_t) 2)
#define MH_TYPE_FAT                 ((mach_header_type_t) 3)

/**
 *  Libhelper definitions for mach_header_64 and mach_header.
 */
typedef struct mach_header			mach_header_32_t;
typedef struct mach_header_64		mach_header_t;

/**
 *  \brief      Mach-O File Structure.
 * 
 *  Mach-O File Structure contains all parsed properties of a Mach-O file and
 *  the raw file data. The Mach-O parser does not use the File API.
 */ 
struct __libhelper_macho {
    /* Raw file properties */
    uint32_t             size;          /* size of mach-o */
    uint32_t             offset;        /* start of data */
    uint8_t             *data;          /* pointer to mach-o in memory */

    /* File properties */
    const char          *path;          /* original filepath */

    /* Parsed Mach-O properties */
    mach_header_t       *header;        /* 64-bit mach-o header */
    HSList              *lcmds;         /* list of all load commands (including LC_SEGMENT) */
    HSList              *scmds;         /* list of segment commands */
    HSList              *dylibs;        /* list of dynamic libraries */
    HSList              *symbols;       /* list of symbols */
    HSList              *strings;       /* list of strings */
};
typedef struct __libhelper_macho        macho_t;

/* TODO: Implement 32-bit mach-o */  

/******************************************************************************
* Libhelper Mach-O Header Parser.
*
* Functions for parsing/handling the Header from a Mach-O.
*******************************************************************************/

/**
 *  \brief      Create a new Mach-O header structure.
 * 
 *  \returns    An allocated \ref macho_header_t structure
 */
extern mach_header_t *
mach_header_create ();

/**
 *  \brief      Load a Mach header from a parsed Mach-O structure.
 * 
 *  \param macho    Parsed Mach-O to fetch header from.
 * 
 *  \returns    Header from given Mach-O.
 */
extern mach_header_t *
mach_header_load (macho_t                  *macho);

/* mach_header_32_load () */

/**
 *  \brief      Verify a Mach header by it's magic bytes and return a mach header
 *              type enum.
 * 
 *  \param magic    Magic bytes from a file/buffer.
 * 
 *  \returns    A mach_header_type_t describing the type of header (unknown, fat, mh64
 *              or mh32).
 */
extern mach_header_type_t
mach_header_verify (uint32_t                magic);

/**
 *  \brief      Generate a string to describe a given cpu type & subtype.
 * 
 *  \param cpu_type     Mach-O CPU type identifier.
 *  \param cpu_subtype  Mach-O CPU subtype identifier.
 * 
 *  \returns    A "name" for a CPU type/subtype combination.
 */
extern char *
mach_header_get_cpu_string (cpu_type_t      cpu_type,
                            cpu_subtype_t   cpu_subtype);

/**
 *  \brief      Generate a string to describe the type of Mach-O given.
 * 
 *  \param type     Mach-O type flag.
 * 
 *  \returns    A "name" for the Mach-O type.
 */
extern char *
mach_header_get_file_type_string (uint32_t  type);

/**
 *  \brief      Generate a short string to describe the type of Mach-O given.
 * 
 *  \param type     Mach-O type flag.
 * 
 *  \returns    A short "name" for the Mach-O type.
 */
extern char *
mach_header_get_file_type_short (uint32_t   type);


/******************************************************************************
* Libhelper Mach-O General Parser.
*
* Functions for parsing/handling a Mach-O File.
*******************************************************************************/

/**
 *  \brief      Load a Mach-O from a specified 
 */
extern char *
macho_load (const char                     *filename);

/**
 * 
 */
extern char *
macho_create_from_buffer (unsigned char    *data);

/* macho_64_create_from_buffer & macho_32_create_from_buffer */

/**
 * 
 */
extern void *
macho_load_bytes (void                     *macho,
                  size_t                    size,
                  uint32_t                  offset);

/**
 * 
 */
extern void
macho_read_bytes (void                     *macho,
                  uint32_t                  offset,
                  void                     *buffer,
                  size_t                    size);

/**
 * 
 */
extern void *
macho_get_bytes (void                      *macho,
                 uint32_t                   offset);


#ifdef __cplusplus
}
#endif

#endif /* __libhelper_macho_h__ */