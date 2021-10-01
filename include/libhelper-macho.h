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
#include <libhelper-fat.h>

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
#define MACH_MAGIC_UNIVERSAL            FAT_MAGIC
#define MACH_MAGIC_64                   MH_MAGIC_64
#define MACH_MAGIC_32                   MH_MAGIC

#define MACH_CIGAM_UNIVERSAL            FAT_CIGAM
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
 *  \returns    An allocated `macho_header_t` structure
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
 *  \brief      Load a Mach-O from a specified file name/path. This function is
 *              a wrapper for macho_create_from_buffer, the file is loaded and
 *              mapped into memory, and that pointer is passed to the create from
 *              buffer function to parse the file.
 * 
 *  \param filename     Filename / path to load from.
 * 
 *  \returns    A parsed macho_t structure from the loaded file.
 */
extern void *
macho_load (const char                     *filename);

/**
 *  \brief      Parse a Mach-O from a given data buffer into a macho_t structure.
 *              This function is architecture-agnostic, meaning that it can load
 *              either 32-bit or 64-bit Mach-O's. 
 * 
 *  \param data         The data pointer to read & parse from.
 * 
 *  \returns    A parsed macho_t structure from the given buffer.
 */
extern void *
macho_create_from_buffer (unsigned char    *data);

/* macho_64_create_from_buffer & macho_32_create_from_buffer */

/**
 *  \brief      Parse a 64-bit Mach-O. Take a given buffer and detect the Mach-O
 *              type, verify and parse into a macho_t structure. It is not required
 *              to pass a size as the size of the macho can be worked out from 
 *              information in the header.
 * 
 *  \param data         Pointer to the macho in memory.
 * 
 *  \returns    A parsed 64-bit macho_t.
 */
extern macho_t *
macho_64_create_from_buffer (unsigned char *data);

/**
 *  \brief      Allocate a new buffer and copy data from a given offset in the macho
 *              to that new buffer. This should be used when needing to modify data,
 *              or creating another structure.
 * 
 *  \param macho        The macho structure to copy from.
 *  \param size         Amount of bytes to copy.
 *  \param offset       Offset to copy from.
 * 
 *  \returns    An allocated buffer of size `size`, from macho base + offset.
 */
extern void *
macho_load_bytes (void                     *macho,
                  size_t                    size,
                  uint32_t                  offset);

/**
 *  \brief      Copy data from a given offset in the macho into a given buffer. This
 *              is similar to macho_load_bytes, but copies the data to an already 
 *              (presumably) allocated buffer, rather than creating and allocating one.
 * 
 *  \param macho        The macho structure to copy from.
 *  \param offset       Amount of bytes to copy.
 *  \param buffer       Buffer/pointer to copy data to.
 *  \param size         Amount of bytes to copy.
 * 
 */
extern void
macho_read_bytes (void                     *macho,
                  uint32_t                  offset,
                  void                     *buffer,
                  size_t                    size);

/**
 *  \brief      Get the pointer to an offset within a given macho. This should be
 *              used when just needed to get a pointer to an area within the allcoated
 *              macho.
 * 
 *  \param offset       Offset from the base of the macho to get a pointer
 *                      to.
 * 
 *  \returns    Pointer to base + offset.
 */
extern void *
macho_get_bytes (void                      *macho,
                 uint32_t                   offset);


/******************************************************************************
* Libhelper Mach-O Load Command Parser.
*
* Functions for parsing/handling the Load Command's from a Mach-O.
*******************************************************************************/

/**
 *  \brief      Redefine the load_command struct as a type.
 */
typedef struct load_command         mach_load_command_t;

/**
 *  \brief      Libhelper Mach-O Load Command Info struct. This is a wrapper
 *              for the load_command structure defined in loader.h which also
 *              includes the offset of the load command in the Mach-O, and the
 *              index.
 */
struct __libhelper_mach_load_command_info {
    mach_load_command_t     *lc;        /* load command */

    uint32_t                 offset;    /* offset of the LC in the Mach-O */
    uint32_t                 index;     /* index in the LC list */
};
typedef struct __libhelper_mach_load_command_info       mach_load_command_info_t;

/**
 *  \brief      Create a new Mach-O Load Command Info structure.
 * 
 *  \returns    An allocated `mach_load_command_info_t` structure
 */
extern mach_load_command_info_t *
mach_load_command_info_create ();

/**
 *  \brief      Parse a Load Command at a given offset and create an info struct
 *              for that load command. 
 * 
 *  \param data     Pointer to the macho.
 *  \param offset   Offset of the load command.
 * 
 *  \returns    A parsed load command info, or NULL.
 */
extern mach_load_command_info_t *
mach_load_command_info_load (const char         *data, 
                             uint32_t            offset);

/**
 *  \brief      Get the name of a given load command, e.g. LC_SEGMENT_64.
 * 
 *  \param lc       Load command to fetch the name of.
 * 
 *  \returns    The name of the load command.
 */
extern char *
mach_load_command_get_name (mach_load_command_t *lc);

/**
 *  \brief      Find a Load Command Info within a macho by its command type. This
 *              will find the first command in the list with this command type. This
 *              does not include Segment command or Dylib commands, so it's unlikely
 *              there would be duplicate commands here.
 * 
 *  \param macho    Macho to search through.
 *  \param cmd      Command type of search for.
 * 
 *  \returns    The Load command info for the given type, or NULL.
 */
extern mach_load_command_info_t *
mach_load_command_find_command_by_type (macho_t     *macho,
                                        uint32_t     cmd);

/**
 *  \brief      Load a string from a load command. These strings are represented as
 *              unions at the end of a load command structure, with an offset pointing
 *              from the base of the load command. The command size includes the string
 *              length. See `lc_str` in loader.h.
 * 
 *  \param macho            Macho containing the command and string.
 *  \param cmdsize          Size of the load command.
 *  \param struct_size      Size of the load command struct.
 *  \param cmd_offset       Offset of the load command in the macho.
 *  \param str_offset       Offset of the string in the load command.
 * 
 *  \returns    The string at the end of the load command.
 */
extern char *
mach_load_command_load_string (macho_t              *macho,
                               uint32_t              cmdsize,
                               uint32_t              struct_size,
                               uint32_t              cmd_offset,
                               uint32_t              str_offset);


/******************************************************************************
* Libhelper Mach-O Segment Command Parser.
*
* Functions for parsing/handling the Segment Command's from a Mach-O.
*******************************************************************************/

/**
 *  Virtual memory protection types
 */
#define VM_PROT_READ                0x00000001
#define VM_PROT_WRITE               0x00000002
#define VM_PROT_EXEC                0x00000004

/**
 *  \brief      Redefine segment_command_64 as a libhelper type.
 */
typedef struct segment_command_64       mach_segment_command_64_t;

/**
 *  \brief      Redefine segment_command as a libhelper type.
 */
typedef struct segment_command          mach_segment_command_32_t;

/**
 *  \brief      Libhelper info wrapper for a Segment Command structure.
 *              Contains the segment command, a virtual memory padding, the
 *              offset of the segcmd in the mach-o, and a list of sections in
 *              the segment. This structure applies to 64-bit segment commands.
 */
struct __libhelper_mach_segment_info_64 {
    mach_segment_command_64_t   *segcmd;        /* segment command */
    uint64_t                     vmpadding;     /* vm-address padding */
    uint32_t                     offset;        /* segcmd offset in macho */

    HSList                      *sections;      /* list of sections */
};
typedef struct __libhelper_mach_segment_info_64    mach_segment_info_64_t;

/**
 *  \brief      The 32-bit variant of the mach_segment_info_64_t struct. The
 *              segment_command structure is slightly different for 32-bit, 
 *              as all values have to be no larger than uint32_t.
 */
struct __libhelper_mach_segment_info_32 {
    mach_segment_command_32_t   *segcmd;        /* segment command 32 */
    uint32_t                     vmpadding;     /* ideally the padding should 
                                                    also be 32bit */
    uint32_t                     offset;        /* segcmd offset in macho */

    HSList                      *sections;      /* list of sections */
};
typedef struct __libhelper_mach_segment_info_32     mach_segment_info_32_t;

/**
 *  NOTE:       Libhelper does not have wrappers for the section structures as
 *              there isn't any additional metadata to add to make parsing
 *              them easier. 
 * 
 *              In terms of documentation, the `section` struct is already
 *              well-documented inside loader.h and also on my own blog:
 *              
 *              https://h3adsh0tzz.com/2020/01/macho-file-format/#segment-commands
 */

/**
 *  \brief      Redefine section_64 as a libhelper type.
 */
typedef struct section_64               mach_section_64_t;

/**
 *  \brief      Redefine section as a libhelper type.
 */
typedef struct section                  mach_section_t;

/**
 *  \brief      
 */
extern mach_segment_command_64_t *
mach_segment_command_64_load (unsigned char    *data,
                              uint32_t          offset);

/**
 * 
 */
extern mach_segment_info_64_t *
mach_segment_info_64_load (unsigned char       *data,
                           uint32_t             offset);



/**
 * 
 */
extern mach_section_64_t *
mach_section_64_load (unsigned char             *data,
                      uint32_t                   offset);

#ifdef __cplusplus
}
#endif

#endif /* __libhelper_macho_h__ */