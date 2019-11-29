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


#endif /* libhelper_macho_command_h */