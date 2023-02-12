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
//  Copyright (C) 2023, Is This On? Holdings Limited
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

/* Mach-O Symbol definitions */
#include "macho/symbols.h"

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
 *  \brief      Architecture type for libhelper wrapper structures that can
 *              represent both 64-bit and 32-bit types.
 */
typedef int                             arch_t;

#define LIBHELPER_ARCH_UNKNOWN      ((arch_t) -1)
#define LIBHELPER_ARCH_64           ((arch_t) 1)
#define LIBHELPER_ARCH_32           ((arch_t) 2)

/**
 *  \brief      Libhelper definitions for mach_header_64 and mach_header.
 */
typedef struct mach_header			mach_header_32_t;
typedef struct mach_header_64		mach_header_t;

/**
 *  \brief      High-level Mach-O File Structure containing all raw file data
 *              and parsed properties of a Mach-O file. 
 */ 
typedef struct __libhelper_macho        macho_t;
struct __libhelper_macho {
    /* Raw file properties */
    uint32_t             size;          /* size of mach-o */
    uint32_t             offset;        /* start of data */
    uint8_t             *data;          /* pointer to mach-o in memory */

    /* File properties */
    const char          *path;          /* original filepath */
    arch_t               arch;          /* file arch */

    /* Parsed Mach-O properties */
    mach_header_t       *header;        /* mach-o header */
    HSList              *lcmds;         /* list of all load commands (including LC_SEGMENT) */
    HSList              *scmds;         /* list of segment commands */
    HSList              *dylibs;        /* list of dynamic libraries */
    HSList              *fileset;       /* list of fileset entry mach-o's */
    HSList              *symbols;       /* list of symbols */
    HSList              *strings;       /* list of strings */
};

/* TODO: Implement 32-bit mach-o */  

/******************************************************************************
* Libhelper Mach-O Header Parser.
*
* Functions for parsing/handling the Header from a Mach-O.
*******************************************************************************/

/**
 *  \brief      Copy and verify the header bytes from a given `macho_t`'s raw data
 *              property into a new `mach_header_t` structure. 
 * 
 *  \param macho    A parsed `macho_t` to work from.
 * 
 *  \returns    Returns a new `mach_header_t` structure with the header bytes
 *              copied.
 */
extern mach_header_t *
mach_header_load (macho_t                  *macho);

/* mach_header_32_load () */

/**
 *  \brief      Verify a Mach Headers magic bytes.
 * 
 *  \param magic    Magic bytes from a file/buffer.
 * 
 *  \returns    Returns a `mach_header_type_t` that matches the magic value that
 *              was passed. This ignores endianness, so whether the magic is a
 *              FEEDFACE or CEFAEDFE, the result would be a 64-bit macho, etc.
 * 
 *              If the magic is unrecognised, the `MH_TYPE_UNKNOWN` result is 
 *              returned.
 */
extern mach_header_type_t
mach_header_verify (uint32_t                magic);

/**
 *  \brief      Generate a human-readable string to describe the CPU type and
 *              subtype. 32-bit ARM CPUs are collectively referred to as "arm32", 
 *              64-bit ARM CPUs are referred to either as "arm64" for basic, or 
 *              "arm64e" for SoC's with extensions such as Pointer Authentication 
 *              or Memory Tagging Extension (MTE). "arm64_32" refers to arm64
 *              CPUs running in 32-bit mode, and Mac Intel chips are x86_64.
 * 
 *              Future additons to arm64 that do not yet have support in libhelper
 *              would be identified as "arm64_unknown", so it is advised you add
 *              some sort of handling for this event.
 * 
 *  \param cpu_type     CPU type identifier.
 *  \param cpu_subtype  CPU subtype identifier.
 * 
 *  \returns    Returns a string descriptor for the combined CPU type and subtype,
 *              assuming that they were both originally paired together. If the
 *              combination was unrecognised, the string "unknown_cpu" is returned.
 */
extern char *
mach_header_get_cpu_string (cpu_type_t      cpu_type,
                            cpu_subtype_t   cpu_subtype);

/**
 *  \brief      Generate a human-readable descriptor for the CPU type and subtype.
 *              This differs from `mach_header_get_cpu_string` as it generates a
 *              string with the CPU architecture and extension names. For example,
 *              a binary built for, what Apple calls, arm64e with Pointer Authentication
 *              and Memory Tagging Extension (MTE), the resulting string would be:
 *
 *                  "ARMv8.5-A, MTE, PAC"
 *
 *              Whereas a simple arm64 binary would have a string of:
 *
 *                  "ARMv8-A"
 *
 *  \param cpu_type     CPU type identifier
 *  \param cpu_subtype  CPU subtype identifier
 *
 *  \returns    Returns a string descriptor for the CPU architecture and architecture
 *              extensions, assuming they were both originally paired together. If
 *              the combination was unrecognised, the string "unknown_cpu" is returned.
 */
extern char *
mach_header_get_cpu_descriptor (cpu_type_t      cpu_type,
                                cpu_subtype_t   cpu_subtype);

/**
 *  \brief      Generate a human-readable string to describe a Mach-O file's
 *              format/type. These types are defined in header-types.h or the
 *              system loader.h. The format for these strings is "Mach <file type>
 *              (<macro_type>)".
 * 
 *  \param type     Type flag.
 * 
 *  \returns    Returns a string descriptor for the type flag. If the type is 
 *              not recognised, the string "Mach Unknown" will be returned instead.
 */
extern char *
mach_header_get_file_type_string (uint32_t  type);

/**
 *  \brief      Generate a short human-readable string to describe a Mach-O's file
 *              format/type. The format for these shortened strings is, using the
 *              MACH_TYPE_OBJECT as an example, simply "Object".
 * 
 *  \param type     Mach-O type flag.
 * 
 *  \returns    Returns a short string descriptor for the type flag. If the type
 *              is not recognised, the string "Unknown" will be returned instead.
 */
extern char *
mach_header_get_file_type_short (uint32_t   type);


/******************************************************************************
* Libhelper Mach-O General Parser.
*
* These functions define the API for parsing an entire Mach-O file. Parsing for
* other elements, like Load & Segment commands, libraries and code-signing is
* defined further down.
*******************************************************************************/

/**
 *  \brief      Check the architecture of a presumed Mach-O file before loading,
 *              so as to know whether to call `macho_64_load()` or `macho_32_load()`.
 * 
 *  \param filename     Filename / path to load from.
 * 
 *  \returns    Returns a `mach_header_type_t` to identify what architecture the
 *              file is, or whether to use the FAT/Universal binary loading API
 *              instead.
 */
extern mach_header_type_t
macho_check_arch (const char *filename);

/**
 *  \brief      Load a Mach-O from a given filename / filepath. This function acts
 *              as a wrapper for `macho_64_create_from_buffer()`. The entire file
 *              is parsed and all `macho_t` fields populated.
 * 
 *              It is assumed that you check the architecture of the file before
 *              using this to load it. Please use `macho_check_arch()` to verify
 *              that a file is indeed a 64-bit Mach-O. 
 * 
 *  \param filename     Filename / path to load from.
 * 
 *  \returns    Returns a `macho_t` struct for the parsed 64-bit Mach-O, or NULL
 *              if there was an error.
 */
extern macho_t *
macho_64_load (const char                     *filename);

/**
 *  \brief      Parse a 64-bit Mach-O. Take a given buffer and detect the Mach-O
 *              type, verify and parse into a `macho_t` structure. It is not required
 *              to pass a size as the size of the macho can be worked out from 
 *              information in the header.
 * 
 *  \param data         Pointer to the macho in memory.
 * 
 *  \returns    Returns a `macho_t` struct for the parsed 64-bit Mach-O, or NULL
 *              if there was an error.
 */
extern macho_t *
macho_64_create_from_buffer (unsigned char *data);

/* macho_32_create_from_buffer */

/**
 *  \brief      Allocate a new buffer and copy data from a given offset in the macho
 *              to that new buffer. This should be used when needing to modify data,
 *              or creating another structure.
 * 
 *  \param macho        The macho structure to copy from.
 *  \param size         Amount of bytes to copy.
 *  \param offset       Offset to copy from.
 * 
 *  \returns    Returns an allocated buffer of size `size`, from macho base + offset.
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
 *  \returns    Returns a pointer to `(macho->data + offset)`.
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
 *  \brief      Redefinition of `load_command` structure as a Libhelper type.
 */
typedef struct load_command         mach_load_command_t;

/**
 *  \brief      Libhelper Mach-O Load Command Info struct. This is a wrapper
 *              for the load_command structure defined in loader.h which also
 *              includes the offset of the load command in the Mach-O, and the
 *              index.
 */
typedef struct __libhelper_mach_load_command_info       mach_load_command_info_t;
struct __libhelper_mach_load_command_info {
    mach_load_command_t     *lc;        /* load command */

    uint32_t                 offset;    /* offset of the LC in the Mach-O */
    uint32_t                 index;     /* index in the LC list */
};

/**
 *  \brief      Parse a Load Command from the pointer to `(data + offset)` and
 *              create a `mach_load_command_info_t` structure.
 * 
 *  \param data     Pointer to the macho.
 *  \param offset   Offset of the load command.
 * 
 *  \returns    Returns a `mach_load_command_info_t` structure, or NULL if
 *              parsing was unsuccessful.
 */
extern mach_load_command_info_t *
mach_load_command_info_load (const char         *data, 
                             uint32_t            offset);

/**
 *  \brief      Fetch the human-readable name for a Load command identifier.
 *              For example, the load command `0x19` is an `LC_SEGMENT_64`, so
 *              the string "LC_SEGMENT_64" would be returned.
 * 
 *  \param lc       Load command to fetch the name of.
 * 
 *  \returns    Returns the name of the Load command, or "LC_UNKNOWN" is the name
 *              could not be determined.
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
 *  \returns    Returns the Load command info for the given type, or NULL.
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
 *  \returns    Returns the string at the end of the load command.
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
 *              the segment. This structure can represent both 32-bit and
 *              64-bit segment commands by setting the arch flag.
 */
typedef struct __libhelper_mach_segment_info    mach_segment_info_t;
struct __libhelper_mach_segment_info {
    void                        *segcmd;        /* segment command */
    uint64_t                     vmpadding;     /* vm-address padding */
    uint32_t                     offset;        /* segcmd offset in macho */
    arch_t                       arch;          /* architecture of segment command */

    HSList                      *sections;      /* list of sections */
};

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
typedef struct section                  mach_section_32_t;

/**
 *  \brief      Load a segment info structure from a specified offset within
 *              given macho data pointer. As the structure is described, it is
 *              architecture-agnostic, so the `arch` flag needs to be checked
 *              before accessing segcmd.
 * 
 *  \param data     Pointer to the macho.
 *  \param offset   Offset of the load command.
 * 
 *  \returns    The segment command info structure, or NULL.
 */
extern mach_segment_info_t *
mach_segment_info_load (unsigned char          *data,
                           uint32_t             offset);

/**
 *  \brief      Search, in a list of Segment command info's, for a Segment command
 *              that matches the provided `segname` and return it.
 * 
 *  \param segments List of segments.
 *  \param segname  Segment name to search for.
 * 
 *  \returns    The segment command info structure, or NULL.
 */
extern mach_segment_info_t *
mach_segment_info_search (HSList                *segments,
                          char                  *segname);

/**
 *  \brief      Fetch a 32-bit segment command structure from a given segment
 *              info.
 * 
 *  \param info Segment command info structure to search.
 * 
 *  \returns    A 32-bit segment command, or NULL if `info` was invalid or if the
 *              command was not 32-bit.
 */
extern mach_segment_command_32_t *
mach_segment_command_32_from_info (mach_segment_info_t *info);

/**
 *  \brief      Fetch a 64-bit segment command structure from a given segment
 *              info. Will return NULL if a 64-bit segment command cannot be
 *              found.
 * 
 *  \param info Segment command info structure to search.
 * 
 *  \returns    A 64-bit segment command, or NULL if `info` was invalid or if the
 *              command was not 64-bit.
 */
extern mach_segment_command_64_t *
mach_segment_command_64_from_info (mach_segment_info_t *info);


/**
 *  \brief      Load a 64-bit segment command structure from a specified offset
 *              within a given macho data pointer.
 * 
 *  \param data     Pointer to the macho.
 *  \param offset   Offset of the load command.
 * 
 *  \returns    A 64-bit segment command structure.
 */
extern mach_segment_command_64_t *
mach_segment_command_64_load (unsigned char    *data,
                              uint32_t          offset);

/**
 *  \brief      Load a 32-bit segment command structure from a specified offset
 *              within a given macho data pointer.
 * 
 *  \param data     Pointer to the macho.
 *  \param offset   Offset of the load command.
 * 
 *  \returns    A 32-bit segment command structure.
 */
extern mach_segment_command_32_t *
mach_segment_command_32_load (unsigned char    *data,
                              uint32_t          offset);

/**
 *  \brief      Return a string representing the memory protection for a given
 *              vm_prot_t flag.
 * 
 *  \param prot     Memory protection flag.
 * 
 *  \returns    A string representing the memory protection specified by `prot`.
 *              For example, a read, execute protection would generated "r-x".
 */
extern char *
mach_segment_read_vm_protection (vm_prot_t prot);

/**
 *  \brief      Load a 64-bit section structure from a specified offset within a 
 *              given macho data pointer.
 * 
 *  \param data     Pointer to the macho.
 *  \param offset   Offset of the section.
 * 
 *  \returns    A 64-bit section structure.
 */
extern mach_section_64_t *
mach_section_64_load (unsigned char             *data,
                      uint32_t                   offset);

/**
 *  \brief      Load a 32-bit section structure from a specified offset within a 
 *              given macho data pointer.
 * 
 *  \param data     Pointer to the macho.
 *  \param offset   Offset of the section.
 * 
 *  \returns    A 32-bit section structure.
 */
extern mach_section_32_t *
mach_section_32_load (unsigned char             *data,
                      uint32_t                   offset);

/**
 *  \brief      Search for a 64-bit section with a given name in a given segment
 *              info structure.
 * 
 *  \param info     Segment info structure to search.
 *  \param sectname Section name to search for.
 * 
 *  \returns    Returns a `mach_section_64_t` with a matching section name to the
 *              provided `sectname`, or NULL if a match could not be found.
 */
extern mach_section_64_t *
mach_section_64_search_in_segment (mach_segment_info_t  *info, 
                                   char                 *sectname);

/**
 *  \brief      Search for a 32-bit section with a given name in a given segment
 *              info structure.
 * 
 *  \param info     Segment info structure to search.
 *  \param sectname Section name to search for.
 * 
 *  \returns    Returns a `mach_section_32_t` with a matching section name to the
 *              provided `sectname`, or NULL if a match could not be found.
 */
extern mach_section_32_t *
mach_section_32_search_in_segment (mach_segment_info_t  *info, 
                                   char                 *sectname); 

/**
 *  \brief      Search for a 32-bit section structure within a given list of segments
 *              by a given segment and section name.
 *
 *              NOTE:   The architecture of sections is the same as that of the parent
 *                      segment, therefore you would not get a 32-bit section in a 
 *                      64-bit segment, and vice versa.
 * 
 *  \param segments List of segments.
 *  \param segname  Segment name to search for.
 *  \param section  Section name to search for.
 * 
 *  \returns    Returns a `mach_section_32_t` structure matching the search criteria,
 *              or NULL if a match could not be found.
 */
extern mach_section_32_t *
mach_section_32_search (HSList                  *segments,
                        char                    *segname,
                        char                    *sectname);

/**
 *  \brief      Search for a 64-bit section structure within a given list of segments
 *              by a given segment and section name.
 *
 *              NOTE:   The architecture of sections is the same as that of the parent
 *                      segment, therefore you would not get a 64-bit section in a 
 *                      32-bit segment, and vice versa.
 * 
 *  \param segments List of segments.
 *  \param segname  Segment name to search for.
 *  \param section  Section name to search for.
 * 
 *  \returns    Returns a `mach_section_64_t` structure matching the search criteria,
 *              or NULL if a match could not be found.
 */
extern mach_section_64_t *
mach_section_64_search (HSList                  *segments,
                        char                    *segname,
                        char                    *sectname);

/**
 *  \brief      Search for a section (architecture-agnostic) at a given index by treating
 *              the sections as one giant list. 
 * 
 *  \param segments List of segment commands.
 *  \param index    Index for section.
 * 
 *  \returns    Returns the section structure, as a `void *`, at the given index by treating
 *              all the sections as a single list. This is not architecture-specific, so
 *              return value must be cast to appropriate type. If the result is not value,
 *              the return value will be NULL. 
 */
extern void *
mach_section_find_at_index (HSList              *segments, 
                            int                  index);


/******************************************************************************
* Libhelper Mach-O additional load command parsing.
*
* Functions for parsing/handling the Additional Load Command's from a Mach-O.
*******************************************************************************/

/**
 *  \brief      Redefinition of source command as a libhelper type.
 */
typedef struct source_version_command           mach_source_version_command_t;

/**
 *  \brief      Search a Mach-O for the `LC_SOURCE_VERSION` Load Command, load it into
 *              a structure and return it back.
 *
 *  \param macho    The Mach-O to search.
 *
 *  \returns    Returns a `mach_source_version_command_t` structure, or NULL if the
 *              operation failed.
 */
extern mach_source_version_command_t *
mach_load_command_find_source_version_command (macho_t  *macho);

/**
 *  \brief      Unpack a version string from an `LC_SOURCE_VERSION` Load Command and
 *              return the result as a formatted string.
 *
 *  \param svc      The Source Version Load Command.
 *
 *  \returns    Returns a formatted string for the unpacked source version, or NULL
 *              if the operation failed.
 */
extern char *
mach_load_command_get_source_version_string (mach_source_version_command_t      *svc);

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of fvmlib load command as a libhelper type.
 *
 *              NOTE:   This Load Command is marked obsolete by Apple, therefore is
 *                      not supported by the libhelper-macho parser.
 */
typedef struct fvmlib_command                       mach_fvmlib_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of entry_point_command load command as libhelper type.
 */
typedef struct entry_point_command                  mach_entry_point_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of dylib_command as a libhelper type.
 */
typedef struct dylib_command                        mach_dylib_command_t;

/**
 *  \brief      A Dylib Load Command Info struct containing the parsed library path
 *              name and the type of dylib it is.
 */
typedef struct __libhelper_mach_dylib_command_info  mach_dylib_command_info_t;
struct __libhelper_mach_dylib_command_info {
    mach_dylib_command_t    *dylib;     /* dylib load command */
    uint32_t                 type;      /* dylib type */
    char                    *name;      /* dylib path name (parsed) */
};

/**
 *  \brief      Get a formatted string representing a Dynamic Libraries version
 *              number.
 *
 *  \param vers     Version number.
 *
 *  \returns    Returns a string representing the version number in the format a.b.c.d.e,
 *              or NULL if the operation failed.
 */
extern char *
mach_load_command_dylib_format_version (uint32_t                vers);

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of sub_framework_command as a libhelper type.
 *
 *              There are no helper functions for this load command as the only
 *              additional property is the subframeworks name, and this can be
 *              fetched with `mach_load_command_get_name()`.
 */
typedef struct sub_framework_command                mach_sub_framework_command_t;

/**
 *  \brief      Redefinition of sub_client_command as a libhelper type.
 *
 *              There are no helper functions for this load command as the only
 *              additional property is the subclients name, and this can be
 *              fetched with `mach_load_command_get_name()`.
 */
typedef struct sub_client_command                   mach_sub_client_command_t;

/**
 *  \brief      Redefinition of sub_umbrella_command as a libhelper type.
 *
 *              There are no helper functions for this load command as the only
 *              additional property is the subumbrella name, and this can be
 *              fetched with `mach_load_command_get_name()`.
 */
typedef struct sub_umbrella_command                 mach_sub_umbrella_command_t;

/**
 *  \brief      Redefinition of sub_library_command as a libhelper type.
 *
 *              There are no helper functions for this load command as the only
 *              additional property is the sublibrary name, and this can be
 *              fetched with `mach_load_command_get_name()`.
 */
typedef struct sub_library_command                  mach_sub_library_command_t;


/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of prebound_dylib_command as a libhelper type.
 *
 *              TODO:   This Load Command needs more investigation into how it
 *                      works and a helper/info struct implemented if required.
 */
typedef struct prebound_dylib_command               mach_prebound_dylib_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of dylinker_command as a libhelper type.
 *
 *              There are no helper functions for this load command as the only
 *              additional property is the dynamic linkers pathname, and this can
 *              be fetched with `mach_load_command_get_name()`.
 */
typedef struct dylinker_command                     mach_dylinker_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of symtab_command as a libhelper type.
 *
 *              There are no helper functions for this load command.
 */
typedef struct symtab_command                       mach_symtab_command_t;

/**
 *  \brief      Libhelper wrapper for the symtab command and list of symbols
 *              in the symbol table.
 *  
 */
typedef struct __libhelper_mach_symbol_table        mach_symbol_table_t;
struct __libhelper_mach_symbol_table {
    mach_symtab_command_t       *cmd;       /* LC_SYMTAB */
    HSList                      *symbols;   /* list of symbols */
};

extern mach_symtab_command_t *
mach_symtab_command_load (macho_t *macho, uint32_t offset);

extern char *
mach_symbol_table_find_symbol_name (macho_t *macho, nlist *sym, mach_symtab_command_t *table);

#define LIBHELPER_MACHO_SYMBOL_NO_NAME          "(no name)"


/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of dysymtab_command as a libhelper type.
 *
 *              There are no helper functions for this load command.
 */
typedef struct dysymtab_command                     mach_dysymtab_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of uuid_command as a libhelper type.
 *
 */
typedef struct uuid_command                         mach_uuid_command_t;

extern char *
mach_load_command_uuid_parse_string (mach_uuid_command_t        *uuid);

/////////////////////////////////////////////////////////////////////////////////

/**
 * \brief       Redefinition of thread_command as a libhelper type.
 */
typedef struct thread_command                       mach_thread_state_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of rpath_command as a libhelper type.
 *
 *              There are no helper functions for this load command.
 */
typedef struct rpath_command                        mach_rpath_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of build_version_command as a libhelper type.
 *
 */
typedef struct build_version_command                mach_build_version_command_t;

/**
 *
 */
typedef struct __libhelper_build_version_info       mach_build_version_info_t;
struct __libhelper_build_version_info {
    mach_build_version_command_t        *cmd;

    char        *platform;
    char        *minos;
    char        *sdk;

    uint32_t     ntools;
    HSList      *tools;
};

/**
 * 	This is a neater version of build_tool_version that has an actual char *
 * 	for the tool name, and then the build version as is found in build_tool_version.
 *
 */
typedef struct __libhelper_build_tool_info          build_tool_info_t;
struct __libhelper_build_tool_info {
    char        *tool;
    uint32_t     version;
};

extern mach_build_version_info_t *
mach_load_command_build_version_info (mach_build_version_command_t *bvc, uint32_t offset, macho_t *macho);

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of dyld_info_command as a libhelper type.
 */
typedef struct dyld_info_command                    mach_dyld_info_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redefinition of linkedit_data_command as libhelper type.
 */
typedef struct linkedit_data_command                mach_linkedit_data_command_t;

/////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief      Redfinition of fileset_entry_command as libhelper type.
 */
typedef struct fileset_entry_command                mach_fileset_entry_command_t;

typedef struct __libhelper_fileset_entry_info       mach_fileset_entry_info_t;
struct __libhelper_fileset_entry_info
{
    mach_fileset_entry_command_t    *cmd;
    macho_t                         *macho;
    uint32_t                         offset;
};

/////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif /* __libhelper_macho_h__ */


















