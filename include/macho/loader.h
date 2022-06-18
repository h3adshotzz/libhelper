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
//  This file is not a direct copy of the loader.h found in XNU. Instead,
//  certain definitions have been picked depending on libhelpers requirements.
//
//  XNU Version: xnu-7195.50.7.100.1
//  Link: https://github.com/apple/darwin-xnu/blob/main/EXTERNAL_HEADERS/mach-o/loader.h
//
//  Full documentation for each definition will be linked to.
//

#ifndef __LIBHELPER_MACHO_XNU_LOADER_H__
#define __LIBHELPER_MACHO_XNU_LOADER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "macho/header-types.h"
#include "macho/command-types.h"
#include "macho/segment-flags.h"

/******************************************************************************
* Libhelper Mach-O Machine Definitions.
* 
* The following definitions are taken from the machine.h system header. Unlike
* loader.h, there is not point copying the entire file into libhelper, instead
* we just "borrow" the required types to make loader.h work. 
*
* If libhelper is build with LIBHELPER_MACHO_USE_SYSTEM_HEADERS then machine.h
* is included by the system loader.h anyway.
* 
*******************************************************************************/

/**
 *  \brief      These CPU type definitions are originally defined in machine.h, 
 * 				but for the sake of keeping things simple they are redefined here.
 */
typedef int                             cpu_type_t;
typedef int                             cpu_subtype_t;
typedef int                             cpu_threadtype_t;

typedef int								vm_prot_t;

/**
 *  \brief      Linux does not have OSSwapInt32(), instead it has bswap_32. If 
 * 				the build platform is linux we should redefine bswap_32 as 
 * 				OSSwapInt32 and include byteswap.h/
 */
#ifdef __APPLE__
#   define OSSwapInt32(x)               _OSSwapInt32(x)
#else
#   include <byteswap.h>
#   define OSSwapInt32(x)               bswap_32(x)
#endif

/**
 *  Capability bits used in the definition of cpu_type. These are used to
 *  calculate the value of the 64bit CPU Type's by performing a logical OR
 *  between the 32bit variant, and the architecture mask.
 * 
 *  E.g. CPU_TYPE_ARM64 = (CPU_TYPE_ARM | CPU_ARCH_ABI64)
 * 
 */
#define CPU_ARCH_MASK                   0xff000000          /* mask for architecture bits */
#define CPU_ARCH_ABI64                  0x01000000          /* 64 bit ABI */
#define CPU_ARCH_ABI64_32               0x02000000          /* ABI for 64-bit hardware with 32-bit types. */

#define CPU_SUBTYPE_MASK                0xff000000          /* mask for feature flags */
#define CPU_SUBTYPE_LIB64               0x80000000          /* 64 bit libraries */
#define CPU_SUBTYPE_PTRAUTH_ABI         0x80000000          /* pointer authentication (PAC) with versioned ABI */

/**
 *  Machine types. There are many more of these defined in `machine.h`, however
 *  we do not need them all as libhelper supports x86_64 and arm64. There
 *  is also support for working with 32-bit ARM files, despite libhelper not
 *  being compatible build-wise.
 * 
 */
#define CPU_TYPE_ANY                    ((cpu_type_t) -1)

#define CPU_TYPE_X86                    ((cpu_type_t) 7)
#define CPU_TYPE_X86_64                 (CPU_TYPE_X86 | CPU_ARCH_ABI64)

#define CPU_TYPE_ARM                    ((cpu_type_t) 12)
#define CPU_TYPE_ARM64                  (CPU_TYPE_ARM | CPU_ARCH_ABI64)
#define CPU_TYPE_ARM64_32               (CPU_TYPE_ARM | CPU_ARCH_ABI64_32)

/**
 *  Machine subtypes. The case is the same as with machine types, they're taken
 *  from the `machine.h` header. As we only defined x86 and ARM cpu types, not
 *  all CPU types will be required here.
 * 
 */
#define CPU_SUBTYPE_ANY                 ((cpu_subtype_t) -1)

/* x86 Subtypes */
#define CPU_SUBTYPE_X86_ALL             ((cpu_subtype_t) 3)
#define CPU_SUBTYPE_X86_64_ALL          ((cpu_subtype_t) 3)
#define CPU_SUBTYPE_X86_ARCH1           ((cpu_subtype_t) 4)
#define CPU_SUBTYPE_X86_64_H            ((cpu_subtype_t) 8)     /* Haswell feature subset */

/* Arm Subtypes */
#define CPU_SUBTYPE_ARM_ALL             ((cpu_subtype_t) 0)
#define CPU_SUBTYPE_ARM_V4T             ((cpu_subtype_t) 5)
#define CPU_SUBTYPE_ARM_V6              ((cpu_subtype_t) 6)
#define CPU_SUBTYPE_ARM_V5TEJ           ((cpu_subtype_t) 7)
#define CPU_SUBTYPE_ARM_XSCALE          ((cpu_subtype_t) 8)
#define CPU_SUBTYPE_ARM_V7              ((cpu_subtype_t) 9)     /* ARMv7-A and ARMv7-R */
#define CPU_SUBTYPE_ARM_V7F             ((cpu_subtype_t) 10)    /* Cortex A9 */
#define CPU_SUBTYPE_ARM_V7S             ((cpu_subtype_t) 11)    /* Swift */
#define CPU_SUBTYPE_ARM_V7K             ((cpu_subtype_t) 12)
#define CPU_SUBTYPE_ARM_V8              ((cpu_subtype_t) 13)
#define CPU_SUBTYPE_ARM_V6M             ((cpu_subtype_t) 14)    /* Not meant to be run under xnu */
#define CPU_SUBTYPE_ARM_V7M             ((cpu_subtype_t) 15)    /* Not meant to be run under xnu */
#define CPU_SUBTYPE_ARM_V7EM            ((cpu_subtype_t) 16)    /* Not meant to be run under xnu */
#define CPU_SUBTYPE_ARM_V8M             ((cpu_subtype_t) 17)    /* Not meant to be run under xnu */

/* AArch64 Subtypes */
#define CPU_SUBTYPE_ARM64_ALL           ((cpu_subtype_t) 0)
#define CPU_SUBTYPE_ARM64_V8            ((cpu_subtype_t) 1)
#define CPU_SUBTYPE_ARM64E              ((cpu_subtype_t) 2)     /* Armv8.5? */

/*  AArch64-32 Subtypes */
#define CPU_SUBTYPE_ARM64_32_ALL        ((cpu_subtype_t) 0)
#define CPU_SUBTYPE_ARM64_32_V8         ((cpu_subtype_t) 1)

/**
 *  CPU Subtype feature flags for pointer authentication on AArch64 platforms,
 *  and libhelper experimental MTE (Memory Tagging Extension) mask.
 */
#define CPU_SUBTYPE_ARM64E_MTE_MASK                 0xc0000000
#define CPU_SUBTYPE_ARM64_PTR_AUTH_MASK             0x0f000000
#define CPU_SUBTYPE_ARM64_PTR_AUTH_VERSION(x)       (((x) & CPU_SUBTYPE_ARM64_PTR_AUTH_MASK) >> 24)

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

/******************************************************************************
* Libhelper Mach-O Loader Definitions.
* 
* The following definitions are taken from the loader.h system header. Unlike
* previous versions of libhelper, the system mach-o definitions are separate
* from some of the libhelper wrappers. This allows us to use the system loader.h
* if we'd like.
*
* If libhelper is build with LIBHELPER_MACHO_USE_SYSTEM_HEADERS then the system
* loader.h is used. 
* 
*******************************************************************************/

/**
 *  \brief		The 32-bit Mach header appears at the very beginning of the 
 * 				object file for 32-bit architectures.
 */
struct mach_header {
	uint32_t	    magic;		    /* mach magic number identifier */
	cpu_type_t	    cputype;	    /* cpu specifier */
	cpu_subtype_t   cpusubtype;	    /* machine specifier */
	uint32_t	    filetype;	    /* type of file */
	uint32_t	    ncmds;		    /* number of load commands */
	uint32_t	    sizeofcmds;	    /* the size of all the load commands */
	uint32_t	    flags;		    /* flags */
};

/* Constant for the magic field for the mach_header (32-bit) */
#define MH_MAGIC            0xfeedface      /* the mach magic number */
#define MH_CIGAM            0xcefaedfe      /* OSSwapInt(MH_MAGIC) */

/**
 * \brief		The 64-bit mach header appears at the very beginning of 
 *				object files for 64-bit architectures.
 */
struct mach_header_64 {
	uint32_t	    magic;		    /* mach magic number identifier */
	cpu_type_t	    cputype;	    /* cpu specifier */
	cpu_subtype_t	cpusubtype;	    /* machine specifier */
	uint32_t	    filetype;	    /* type of file */
	uint32_t	    ncmds;		    /* number of load commands */
	uint32_t	    sizeofcmds;	    /* the size of all the load commands */
	uint32_t	    flags;		    /* flags */
	uint32_t	    reserved;	    /* reserved */
};

/* Constant for the magic field for the mach_header (32-bit) */
#define MH_MAGIC_64        0xfeedfacf       /* the mach magic number */
#define MH_CIGAM_64        0xcffaedfe       /* OSSwapInt(MH_MAGIC_64) */

/**
 * \brief       A variable length string in a load command is represented by
 *              an lc_str union. The strings are stored just after the load
 *              command structure and the offset is from the start of the load
 *              command structure. The size of the string is reflected in the
 *              cmdsize field of the load command. Once again any padded bytes
 *              to bring the cmdsize field to a multiple of 4 bytes must be zero.
 */
union lc_str {
	uint32_t	  offset;	      /* offset to the string */
#ifndef __LP64__
	char		 *ptr;	          /* pointer to the string */
#endif
};

/**
 * 	\brief		The Load Commands in a Mach-O file directly follow the Mach
 * 				Header. The total size of the load commands section will be
 * 				given as `sizeofcmds` in the header. This structure defines
 * 				the base of the load commands, all of which must have a cmd
 * 				identifier and a cmdsize. Each command type has it's own
 * 				structure, but this allows for all cmds to be identified by
 * 				having these common bytes at the start.
 * 
 * 				More documentation on the Load Commands can be found inside
 * 				XNU loader.h.
 */
struct load_command {
	uint32_t		cmd;			/* load command type */
	uint32_t		cmdsize;		/* load command size */
};

/**
 * 	\brief		Segment Load Commands indicate a part of the file to be mapped
 * 				into the task's address space. The size of the segment in memory,
 * 				represented by `vmsize`, can be equal to, or larger than, the
 * 				amount of bytes to map from the file (`filesize`). 
 * 
 * 				The file is mapped from `fileoff` to `vmaddr`, any extra memory
 * 				after mapping is filled with zeros. The initial and maximum
 * 				memory protection is defined by `initprot` and `maxprot`. If the
 * 				segment has any sections they will directly follow this structure
 * 				and they are reflected in the `cmdsize`. 
 */
struct segment_command { /* for 32-bit architectures */
	uint32_t		cmd;			/* LC_SEGMENT */
	uint32_t		cmdsize;		/* includes sizeof section structs */
	char			segname[16];	/* segment name */
	uint32_t		vmaddr;			/* memory address of this segment */
	uint32_t		vmsize;			/* memory size of this segment */
	uint32_t		fileoff;		/* file offset of this segment */
	uint32_t		filesize;		/* amount to map from the file */
	vm_prot_t		maxprot;		/* maximum VM protection */
	vm_prot_t		initprot;		/* initial VM protection */
	uint32_t		nsects;			/* number of sections in segment */
	uint32_t		flags;			/* flags */
};

/**
 * 	\brief		Segment Load Command (64-bit) indicate a part of the file to be
 * 				mapped into a 64-bit task's address space. If the 64-bit segment
 * 				has sections, then section_64 structures directly follow this
 * 				structure, with their size also represented in `cmdsize`.
 */
struct segment_command_64 {
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
};

/**
 * 	\brief		A Segment Load Command will contain, at the end, zero or more
 * 				sections. These "sections" split up the text and data inside 
 * 				a segment and describe it. For example, the __TEXT segment may
 * 				have a __text and __stubs section. 
 * 
 * 				When it comes to the actual section structure, it contains the
 * 				name of the segment it belongs to, the name of itself (if two
 * 				sections have the same name, they're linked together), address,
 * 				size, alignment, offset, flags and reserved values.
 */
struct section_64 {
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
};

/**
 * 	\brief		Defines a 32-bit section structure. Same as section_64, but the 
 * 				address and size values are 32-bits instead of 64-bits.
 */
struct section { /* for 32-bit architectures */
	char		sectname[16];	/* name of this section */
	char		segname[16];	/* segment this section goes in */
	uint32_t	addr;			/* memory address of this section */
	uint32_t	size;			/* size in bytes of this section */
	uint32_t	offset;			/* file offset of this section */
	uint32_t	align;			/* section alignment (power of 2) */
	uint32_t	reloff;			/* file offset of relocation entries */
	uint32_t	nreloc;			/* number of relocation entries */
	uint32_t	flags;			/* flags (section type and attributes)*/
	uint32_t	reserved1;		/* reserved (for offset or index) */
	uint32_t	reserved2;		/* reserved (for count or sizeof) */
};

/**
 *  \brief      Fixed Virtual Memory shared library load command.
 *
 *              NOTE: This command is marked obsolete by Apple.
 */
struct fvmlib {
    union lc_str        name;           /* library's target pathname */
    uint32_t            minor_version;  /* library's minor version number */
    uint32_t            header_addr;    /* library's header address */
};

/**
 *  \brief      Fixed Virtual Memory command implementation.
 *
 *              NOTE: This command is marked obsolete by Apple.
 */
struct fvmlib_command {
    uint32_t            cmd;            /* LC_IDFVMLIB or LC_LOADFVMLIB */
    uint32_t            cmdsize;        /* includes pathname string */
    struct fvmlib       fvmlib;         /* the library identification */
};

/**
 *  \brief      The entry_point_command is a replacement for the thread_command. It
 *              is used for main executables to specify the location (file offset)
 *              of main().
 */
struct entry_point_command {
    uint32_t    cmd;            /* LC_MAIN, only used in MH_EXECUTE types */
    uint32_t    cmdsize;        /* 24 */
    uint32_t    entryoff;       /* file (__TEXT) offset of main() */
    uint32_t    stacksize;      /* if not zero, initial stack size */
};

/**
 *  \brief      The thread_command defines the default CPU state.
 */
struct thread_command {
    uint32_t cmd;               /* LC_THREAD or LC_UNIXTHREAD */
    uint32_t cmdsize;           /* sizeof (thread_command) */
    uint32_t flavour;           /* thread state flavour */
    uint32_t count;             /* thread state size in bytes */
    uint64_t regs[29];          /* registers x0-x29 */
    uint64_t fp;                /* frame pointer */
    uint64_t lr;                /* link register */
    uint64_t sp;                /* stack pointer */
    uint64_t pc;                /* program counter */
    uint32_t cpsr;              /* current program status register */
};

/**
 *  \brief      The source_version_command is an optional load command containing
 *              the version of the sources used to build the binary.
 */
struct source_version_command {
    uint32_t    cmd;            /* LC_SOURCE_VERSION */
    uint32_t    cmdsize;        /* 16 */
    uint64_t    version;        /* A.B.C.D.E packed as a24.b10.c10.d10.e10 */
};

/**
 *  \brief      Dynamicly Linked Shared Libraries are identified by their pathname
 *              and the compatibility number. The pathname must match and the
 *              compatibility number in the user of the library must be greater than
 *              or equal to the library being used. The timestamp is to record the
 *              time at which the library was built and copied, so it can be determined
 *              at runtime if this was the library used to build the binary.
 */
struct dylib {
    union lc_str    name;                   /* library's path name */
    uint32_t        timestamp;              /* library's build timestamp */
    uint32_t        current_version;        /* library's current version number */
    uint32_t        compatibility_version;  /* library's compatibility vers number */
};

/**
 *  \brief      A Dynamicly Linked Shared Library Load Command. An object that uses
 *              a dynamically linked shared library also contains a dylib_command,
 *              LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, or LC_REEXPORT_DYLIB for each library
 *              it uses.
 */
struct dylib_command {
    uint32_t        cmd;        /* LC_ID_DYLIB, LC_LOAD_WEAK_DYLIB, LC_REEXPORT_DYLIB */
    uint32_t        cmdsize;    /* includes pathname string */
    struct dylib    dylib;      /* the library identification */
};

/**
 *  \brief      A Dynamically Linked Shared Library can be a "subframework" or an
 *              "umbrella" framework. These are linked with "-umbrella umbrella_name".
 *              A "subframework" can only be linked against by its umbrella framework
 *              or other subframeworks that are part of that umbrella. Otherwise an
 *              error is thrown at compile-time.
 */
struct sub_framework_command {
    uint32_t        cmd;        /* LC_SUB_FRAMEWORK */
    uint32_t        cmdsize;    /* includes umbrella string */
    union lc_str    umbrella;   /* the subframework name */
};

/**
 *  \brief      A Dynamically Linked Shared Library that is already a subframework of
 *              an umbrella framework can allow clients other than the umbrella framework
 *              or other subframeworks in the same umbrella.
 */
struct sub_client_command {
    uint32_t        cmd;        /* LC_SUB_CLIENT */
    uint32_t        cmdsize;    /* includes client string */
    union lc_str    client;     /* the sub client name */
};

/**
 *  \brief      A Dynamically Linked Shared Library may be a sub umbrella of an umbrella
 *              framework. More information on linking available in the system loader.h
 *              file.
 */
struct sub_umbrella_command {
    uint32_t        cmd;            /* LC_SUB_UMBRELLA */
    uint32_t        cmdsize;        /* includes sub_umbrella string */
    union lc_str    sub_umbrella;   /* the sub umbrella name */
};

/**
 *  \brief      A Dynamically Linked Shared Library may be a sub library of another
 *              shared library. More information on linking available in the system loader.h
 *              file.
 */
struct sub_library_command {
    uint32_t        cmd;            /* LC_SUB_LIBRARY */
    uint32_t        cmdsize;        /* includes sub_library string */
    union lc_str    sub_library;    /* the sub library name */
};

/**
 *  \brief      A Mach-O of the type MH_EXECUTE that is prebound to its Dynamic Libraries
 *              will have an LC_PREBOUND_DYLIB command for each prebound dylib. It contains
 *              a bit vector for the modules in the library.
 *
 *              The bits indicate which modules are bound (1) and which are not (0) from
 *              the library. The bit for module 0 is the low bit of the first byte, so
 *              the bit for the Nth module is: (linked_modules[N/8] >> N % 8) & 1.
 */
struct prebound_dylib_command {
    uint32_t        cmd;            /* LC_PREBOUND_DYLIB */
    uint32_t        cmdsize;        /* includes strings */
    union lc_str    name;           /* library's path name */
    uint32_t        nmodules;       /* number of modules in the library */
    union lc_str    linked_modules; /* bit vector of linked modules */
};

/**
 *  \brief      The Dynamic Linker used by a program is declared by this dylinker load
 *              command.
 */
struct dylinker_command {
    uint32_t        cmd;        /* LC_ID_DYLINKER, LC_LOAD_DYLINKER, LC_DYLD_ENVIRONMENT */
    uint32_t        cmdsize;    /* includes pathname string */
    union lc_str    name;       /* dynamic linkers path name */
};

/**
 *  \brief      The Symbol Table Command contains the offsets and sizes of the link-edit
 *              4.3BSD "stab" style symbol table information as described in the header
 *              files nlist.h and stab.h.
 */
struct symtab_command {
    uint32_t        cmd;        /* LC_SYMTAB */
    uint32_t        cmdsize;    /* sizeof (struct symtab_command) */
    uint32_t        symoff;     /* symbol table offset */
    uint32_t        nsyms;      /* number of symbol table entries */
    uint32_t        stroff;     /* string table offset */
    uint32_t        strsize;    /* number of string table entries */
};

/**
 *  \brief      The Dynamic Symbol Table Command has a *very* long description that I
 *              am not going to rewrite here, it can be found in the xnu loader.h.
 */
struct dysymtab_command {
    uint32_t        cmd;            /* LC_DYSYMTAB */
    uint32_t        cmdsize;        /* sizeof (struct dysymtab_command) */

    uint32_t        ilocalsym;      /* index to local symbols */
    uint32_t        nlocalsym;	    /* number of local symbols */

    uint32_t        iextdefsym;     /* index to externally defined symbols */
    uint32_t        nextdefsym;     /* number of externally defined symbols */

    uint32_t        iundefsym;      /* index to undefined symbols */
    uint32_t        nundefsym;      /* number of undefined symbols */

    uint32_t        tocoff;         /* file offset to table of contents */
    uint32_t        ntoc;           /* number of entries in table of contents */

    uint32_t        modtaboff;      /* file offset to module table */
    uint32_t        nmodtab;        /* number of module table entries */

    uint32_t        extrefsymoff;   /* offset to referenced symbol table */
    uint32_t        nextrefsyms;    /* number of referenced symbol table entries */

    uint32_t        indirectsymoff; /* file offset to the indirect symbol table */
    uint32_t        nindirectsyms;  /* number of indirect symbol table entries */

    uint32_t        extreloff;      /* offset to external relocation entries */
    uint32_t        nextrel;        /* number of external relocation entries */

    uint32_t        locreloff;      /* offset to local relocation entries */
    uint32_t        nlocrel;        /* number of local relocation entries */
};

/**
 * \brief       The twolevel_hints_command contains the offset and number of hints
 *              in the two-level namespace lookup hints table.
 */
struct twolevel_hints_command {
    uint32_t        cmd;            /* LC_TWOLEVEL_HINTS */
    uint32_t        cmdsize;        /* sizeof(struct twolevel_hints_command) */
    uint32_t        offset;	        /* offset to the hint table */
    uint32_t        nhints;         /* number of hints in the hint table */
};

/**
 *  \brief      Add desc.
 */
struct prebind_cksum_command {
    uint32_t        cmd;            /* LC_PREBIND_CKSUM */
    uint32_t        cmdsize;        /* sizeof(struct prebind_cksum_command) */
    uint32_t        cksum;          /* the check sum or zero */
};

/**
 * \brief       The uuid load command contains a single 128-bit unique random number
 *              that identifies an object produced by the static link editor.
 */
struct uuid_command {
    uint32_t        cmd;            /* LC_UUID */
    uint32_t        cmdsize;        /* sizeof(struct uuid_command) */
    uint8_t         uuid[16];       /* the 128-bit uuid */
};

/**
 * \brief       The rpath_command contains a path which at runtime should be added to
 *              the current run path used to find @rpath prefixed dylibs.
 */
struct rpath_command {
    uint32_t        cmd;            /* LC_RPATH */
    uint32_t        cmdsize;        /* includes string */
    union lc_str    path;           /* path to add to run path */
};

/**
 *  \brief      The linkedit data command contains the offsets and sizes of a blob
 *              of data in the __LINKEDIT segment. There are a few commands that use
 *              this structure, including:
 *
 *                  LC_CODE_SIGNATURE, LC_SEGMENT_SPLIT_INFO, LC_FUNCTION_STARTS,
 *                  LC_DATA_IN_CODE, LC_DYLIB_CODE_SIGN_DRS, LC_LINKER_OPTIMIZATION_HINT,
 *                  LC_DYLD_EXPORTS_TRIE or LC_DYLD_CHAINED_FIXUPS.
 */
struct linkedit_data_command {
    uint32_t        cmd;            /* A few possible command types */
    uint32_t        cmdsize;        /* sizeof (struct linkedit_data_command) */
    uint32_t        dataoff;        /* file offset of data in __LINKEDIT segment */
    uint32_t        datasize;       /* file size of data in __LINKEDIT segment */
};

/**
 * \brief       The version_min_command contains the min OS version on which this 
 *              binary was built to run. A few load commands use this structure:
 *
 *                  LC_VERSION_MIN_MACOSX, LC_VERSION_MIN_IPHONEOS, LC_VERSION_MIN_WATCHOS
 *                  or LC_VERSION_MIN_TVOS.
 */
struct version_min_command {
    uint32_t	    cmd;            /* A few possible command types */
    uint32_t	    cmdsize;	    /* sizeof(struct min_version_command) */
    uint32_t	    version;	    /* X.Y.Z is encoded in nibbles xxxx.yy.zz */
    uint32_t	    sdk;		    /* X.Y.Z is encoded in nibbles xxxx.yy.zz */
};

/**
 * \brief       The build_version_command contains the min OS version on which this
 *              binary was built to run for its platform.  The list of known platforms and
 *              tool values following it.
 */
struct build_version_command {
    uint32_t        cmd;            /* LC_BUILD_VERSION */
    uint32_t        cmdsize;        /* sizeof(struct build_version_command) plus */
                                    /* ntools * sizeof(struct build_tool_version) */
    uint32_t        platform;       /* platform */
    uint32_t        minos;          /* X.Y.Z is encoded in nibbles xxxx.yy.zz */
    uint32_t        sdk;            /* X.Y.Z is encoded in nibbles xxxx.yy.zz */
    uint32_t        ntools;         /* number of tool entries following this */
};

struct build_tool_version {
    uint32_t        tool;           /* enum for the tool */
    uint32_t        version;        /* version number of the tool */
};

/* values for the platform field */
#define PLATFORM_MACOS                  1
#define PLATFORM_IOS                    2
#define PLATFORM_TVOS                   3
#define PLATFORM_WATCHOS                4
#define PLATFORM_BRIDGEOS               5
#define PLATFORM_MACCATALYST            6
#define PLATFORM_IOSSIMULATOR           7
#define PLATFORM_TVOSSIMULATOR          8
#define PLATFORM_WATCHOSSIMULATOR       9
#define PLATFORM_DRIVERKIT              10

/* values for tools */
#define TOOL_CLANG                      1
#define TOOL_SWIFT                      2
#define TOOL_LD                         3

/**
 *  \brief      The dyld_info_command contains the file offsets and sizes of the new
 *              compressed form of the information dyld needs to load the image.
 *
 */
struct dyld_info_command {
    uint32_t        cmd;
    uint32_t        cmdsize;

    uint32_t        rebase_off;
    uint32_t        rebase_size;

    uint32_t        bind_off;
    uint32_t        bind_size;

    uint32_t        weak_bind_off;
    uint32_t        weak_bind_size;

    uint32_t        lazy_bind_off;
    uint32_t        lazy_bind_size;

    uint32_t        export_off;
    uint32_t        export_size;
};


#ifdef __cplusplus
}
#endif

#endif /* __libhelper_macho_xnu_loader_h__ */















