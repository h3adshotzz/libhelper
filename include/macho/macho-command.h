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

#ifndef LIBHELPER_MACHO_COMMAND_H
#define LIBHELPER_MACHO_COMMAND_H

/**
 *                  === The Libhelper Project ===
 *                          Mach-O Parser
 * 
 *  Documentation relating to the macho-command.h header file:          |
 *                                                                      |
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
 *      (HIGH PRIORITY)
 *      - Move over everything currently done in src/macho/command.c
 *      - Ensure docuemntation is included here as well, not just the .c
 *      - Test extensively to ensure nothing was broken when moving code
 * 
 *      (MEDIUM PRIORITY)
 *      - Add additional Load Commands.
 * 
 *  When the entire header functionality is complete, write the above
 *  documentation, which will also go on:
 * 
 *      https://h3adsh0tzz.com/docs/libhelper-macho/
 * 
 */

#include <stdint.h>
#include "consts/macho-command-const.h"
#include "macho-symbol.h"
#include "macho.h"
#include "file.h"


/**
 *  Flags for the Load Command print functions.
 * 
 *  LC_RAW      Prints a raw Load Command Struct.
 *  LC_INFO     Prints a Load Command Info Struct.
 */
#define     LC_RAW      0x0
#define     LC_INFO     0x1


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
	uint32_t cmd;					/* type of load command */
	uint32_t cmdsize;				/* total size of command in bytes */
} mach_load_command_t;


/**
 * 	Mach-O Load Command Info structure.
 * 
 * 	Used to carry the offset of the load command in the file with the load command structure.
 * 	I guess it is architecture specific, because the offset is a uint64_t.
 */
typedef struct mach_command_info_t {
	mach_load_command_t		*lc;		/* load command structure */
	uint32_t				type;		/* load command type */
	uint64_t				off;		/* offset within the file */
} mach_command_info_t;

#define MACH_LOAD_COMMAND_SIZE		sizeof(mach_load_command_t)
#define MACH_COMMAND_INFO_SIZE		sizeof(mach_command_info_t)


/*
 * 	The source_version_command is an optional load command containing
 * 	the version of the sources used to build the binary.
 */
typedef struct mach_source_version_command_t {
    uint32_t  	cmd;			/* LC_SOURCE_VERSION */
    uint32_t  	cmdsize;		/* 16 */
    uint64_t  	version;		/* A.B.C.D.E packed as a24.b10.c10.d10.e10 */
} mach_source_version_command_t;


/**
 * 	The mach_uuid_command_t is an optional load command containing
 * 	the UUID of the binary.
 */
typedef struct mach_uuid_command_t {
	uint32_t	cmd;			/* LC_UUID */
	uint32_t	cmdsize;		/* sizeof(mach_uuid_command_t) */
	uint8_t		uuid[16];		/* 128-bit UUID */
} mach_uuid_command_t;


/**
 * 	The mach_dyld_info_command_t contains file offsets and size of the new
 * 	compressed form of the information the dyld needs to load the image.
 * 	On macOS the Dynamic Linker uses the information in this command.
 * 
 * 	[Note]
 * 		These are the docs from loader.h, I'll write them in my own words
 * 		as I learn what they do.
 * 
 */
typedef struct mach_dyld_info_command_t {

	/**
	 * 	Base Mach-O Load Command data.
	 */
	uint32_t	cmd;			/* LC_DYLD_INFO or LC_DYLD_INFO_ONLY */
	uint32_t	cmdsize;		/* sizeof (mach_dyld_info_command_t) */

	/**
	 * 	Dynamic Linker rebases an image whenever dyld loads it at an address
	 * 	different from its preffered address. The rebase information is a
	 * 	stream of byte sized opcodes whose symbolic names start with REBASE_OPCODE_..
	 * 	
	 * 	Conceptually, the rebase information is a table of tuples:
	 * 		<seg-index, seg-offset, type>
	 * 
	 * 	The opcodes are a compressed way to encode teh table by only encoding
	 * 	when a column changes. In addition, simple patterns like "every n'th
	 * 	offset for m times" can be encoded in a few bytes.
	 * 
	 */
	uint32_t	rebase_off;		/* file offset to rebase info */
	uint32_t	rebase_size;	/* size of rebase info */

	/**
	 * 	Dynamic Linker binds an image during the loading process, if the image
	 * 	requires any pointers to be initialised to symbols in other images. The
	 * 	bind information is a stream of byte sized opcodes whose symbolic
	 * 	names start with BIND_OPCODE_...
	 * 
	 * 	Conceptually, the bind information is a table of tuples:
	 * 		<seg-index, seg-offset, type, symbol-library-ordinal, symbol-name, addend>
	 * 
	 * 	The opcodes are a compressed way to encode teh table by only encoding
	 * 	when a column changes. In addition simple patterns like for runs of
	 * 	pointers initialised to the same value can be encoded in a few bytes.
	 * 
	 */
	uint32_t	bind_off;		/* file offset to binding info */
	uint32_t	bind_size;		/* size of binding info */

	/**
	 * 	Some C++ programs require dyld to unique symbols so that all images
	 * 	in the process use the same copy of some code/data. This step is done
	 * 	after binding. The content of the weak_bind info is an opcode stream
	 * 	like the bind_info. But it is sorted alphabetically by symbol name. This
	 * 	enables dyld to walk all images with weak binding information in order
	 * 	and look for collisions. If there are no collisions, dyld does no updating.
	 * 	That means that some fixups are also encoded in the bind_info. For instnace,
	 * 	all calls the "operator new" are first bound to libstdc++.dylib using
	 * 	the information in bind_info. Then if some image overrides operator new
	 * 	that is detected when the weak_bind information is processed and teh call
	 * 	to operator new is then rebound.
	 * 
	 */
	uint32_t	weak_bind_off;		/* file offset to weak binding info */
	uint32_t	weak_bind_size;		/* size of weak binding info */

	/*
     * 	Some uses of external symbols do not need to be bound immediately.
     * 	Instead they can be lazily bound on first use.  The lazy_bind
     * 	are contains a stream of BIND opcodes to bind all lazy symbols.
     * 	Normal use is that dyld ignores the lazy_bind section when
     * 	loading an image.  Instead the static linker arranged for the
     * 	lazy pointer to initially point to a helper function which 
     * 	pushes the offset into the lazy_bind area for the symbol
     * 	needing to be bound, then jumps to dyld which simply adds
     * 	the offset to lazy_bind_off to get the information on what 
     * 	to bind.  
     */
    uint32_t   lazy_bind_off;	/* file offset to lazy binding info */
    uint32_t   lazy_bind_size;  /* size of lazy binding infs */

	/*
     * 	The symbols exported by a dylib are encoded in a trie.  This
     * 	is a compact representation that factors out common prefixes.
     * 	It also reduces LINKEDIT pages in RAM because it encodes all  
     * 	information (name, address, flags) in one small, contiguous range.
     * 	The export area is a stream of nodes.  The first node sequentially
     * 	is the start node for the trie.  
     *
     * 	Nodes for a symbol start with a uleb128 that is the length of
     * 	the exported symbol information for the string so far.
     * 	If there is no exported symbol, the node starts with a zero byte. 
     * 	If there is exported info, it follows the length.  
	 *
	 * 	First is a uleb128 containing flags. Normally, it is followed by
     * 	a uleb128 encoded offset which is location of the content named
     * 	by the symbol from the mach_header for the image.  If the flags
     * 	is EXPORT_SYMBOL_FLAGS_REEXPORT, then following the flags is
     * 	a uleb128 encoded library ordinal, then a zero terminated
     * 	UTF8 string.  If the string is zero length, then the symbol
     * 	is re-export from the specified dylib with the same name.
	 * 	If the flags is EXPORT_SYMBOL_FLAGS_STUB_AND_RESOLVER, then following
	 * 	the flags is two uleb128s: the stub offset and the resolver offset.
	 * 	The stub is used by non-lazy pointers.  The resolver is used
	 * 	by lazy pointers and must be called to get the actual address to use.
     *
     * 	After the optional exported symbol information is a byte of
     * 	how many edges (0-255) that this node has leaving it, 
     * 	followed by each edge.
     * 	Each edge is a zero terminated UTF8 of the addition chars
     * 	in the symbol, followed by a uleb128 offset for the node that
     * 	edge points to.
     *  
     */
    uint32_t   export_off;	/* file offset to lazy binding info */
    uint32_t   export_size;	/* size of lazy binding infs */
} mach_dyld_info_command_t;


/*
 * 	A program that uses a dynamic linker contains a dylinker_command to identify
 * 	the name of the dynamic linker (LC_LOAD_DYLINKER).  And a dynamic linker
 * 	contains a dylinker_command to identify the dynamic linker (LC_ID_DYLINKER).
 * 	A file can have at most one of these.
 * 	This struct is also used for the LC_DYLD_ENVIRONMENT load command and
 * 	contains string for dyld to treat like environment variable.
 * 
 */
typedef struct mach_dylinker_command_t {
	uint32_t	cmd;
	uint32_t	cmdsize;
	
} mach_dylinker_command_t; 


/**
 * 	Dynamicly linked Shared Library command.
 * 
 * 	This identifies any dynamically shared linked libraries that an
 * 	executable requires.
 * 
 * 	The 'dylib' struct contains the lib properties.
 * 
 * 	The 'dylib_command' is the load command structure.
 * 
 * 	The dylib name string is stored just after the load command structure.
 * 	The offset prop is from the start of the load command structure, so
 * 	the size of the string is:
 * 		s = cmdsize - (sizeof(uint32_t) * 4);
 * 
 */

typedef struct dylib_vers_t {
	uint32_t			a;			/* XXXX.00.00 */
	uint32_t			b;			/* 0000.XX.00 */
	uint32_t			c;			/* 0000.00.XX */
} dylib_vers_t;

struct dylib {
	uint32_t		offset;		/* Offset of the library name in the string table */
#ifndef __LP64__
	char			*ptr;		/* pointer to the string */
#endif
	
	uint32_t		timestamp;				/* lib build time stamp */
	uint32_t		current_version;		/* lib current version numbre */
	uint32_t		compatibility_version;	/* lib compatibility vers numb */
};

#include "strutils.h"

typedef struct mach_dylib_command_t {
	uint32_t		cmd;		/* LC_ID_DYLIB, LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, LC_REEXPORT_DYLIB */
	uint32_t		cmdsize;	/* Includes pathname string */
	struct dylib	dylib;
} mach_dylib_command_t;

typedef struct mach_dylib_command_info_t {
	mach_dylib_command_t	*dylib;
	uint32_t				 type;
	char					*name;
} mach_dylib_command_info_t;



//////////////////////////////////////////////////////////////////////////
//                       Function Definitions                           //
//////////////////////////////////////////////////////////////////////////

// TODO: Add Docs

/**
 *  Functions for creating and loading mach_load_command_info_t and 
 *  mach_command_info_t structs safely.
 * 
 */
mach_load_command_t *mach_load_command_create ();
mach_command_info_t *mach_command_info_create ();
mach_command_info_t *mach_command_info_load (file_t *file, off_t offset);


/**
 *  Functions for printing Load Command data, and calculating string
 *  representations for particular properties.
 */ 
void mach_load_command_info_print (mach_command_info_t *cmd);
void mach_load_command_print (void *cmd, int flag);
char *mach_load_command_get_string (mach_load_command_t *lc);


/**
 * 	LC_SOURCE_VERSION functions
 */
mach_source_version_command_t *mach_lc_find_source_version_cmd (macho_t *macho);
char *mach_lc_source_version_string (mach_source_version_command_t *svc);


/**
 * 	LC_UUID functions
 */
mach_uuid_command_t *mach_lc_find_uuid_cmd (macho_t *macho);
char *mach_lc_uuid_string (mach_uuid_command_t *cmd);


/**
 * 	LC_SYMTAB
 */
mach_command_info_t *mach_lc_find_given_cmd (macho_t *macho, int cmd);
mach_symtab_command_t *mach_lc_find_symtab_cmd (macho_t *macho);


/**
 * 	LC_DYSYMTAB
 */
mach_dysymtab_command_t *mach_lc_find_dysymtab_cmd (macho_t *macho);


/**
 * 	LC_ID_DYLIB, LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, LC_REEXPORT_DYLIB
 */
char *mach_lc_load_dylib_format_version (uint32_t vers);
char *mach_lc_dylib_get_type_string (mach_dylib_command_t *dylib);

#endif /* libhelper_macho_command_h */