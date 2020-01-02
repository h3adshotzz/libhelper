//===--------------------------- macho_command ------------------------===//
//
//                          Libhelper Mach-O Parser
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
//  me@h3adsh0tzz.com.
//
//
//===------------------------------------------------------------------===//

#ifndef LIBHELPER_MACHO_COMMAND_TYPES_LL_H
#define LIBHELPER_MACHO_COMMAND_TYPES_LL_H

#include "libhelper-macho-ll/macho-command.h"
#include "libhelper-macho-ll/macho.h"
#include "libhelper/strutils.h"


mach_command_info_t *mach_lc_find_given_cmd (macho_t *macho, int cmd);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


/*
 * 	The source_version_command is an optional load command containing
 * 	the version of the sources used to build the binary.
 */
typedef struct mach_source_version_command_t {
    uint32_t  	cmd;			/* LC_SOURCE_VERSION */
    uint32_t  	cmdsize;		/* 16 */
    uint64_t  	version;		/* A.B.C.D.E packed as a24.b10.c10.d10.e10 */
} mach_source_version_command_t;


mach_source_version_command_t 	*mach_lc_find_source_version_cmd (macho_t *macho);
char 							*mach_lc_source_version_string (mach_source_version_command_t *svc);


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


/**
 * 	The build_version_command contains the min OS version on which this
 * 	binary was built to run for its platform.
 */
typedef struct mach_build_version_command_t {
    uint32_t	cmd;		/* LC_BUILD_VERSION */
    uint32_t	cmdsize;	/* sizeof(struct build_version_command) plus */
                                /* ntools * sizeof(struct build_tool_version) */
    uint32_t	platform;	/* platform */
    uint32_t	minos;		/* X.Y.Z is encoded in nibbles xxxx.yy.zz */
    uint32_t	sdk;		/* X.Y.Z is encoded in nibbles xxxx.yy.zz */
    uint32_t	ntools;		/* number of tool entries following this */
} mach_build_version_command_t;


/**
 * 	The build_tool_version are found after the mach_build_version_command_t
 * 	in the Mach-O file. The `ntools` prop defines how many build_tool_version
 * 	structs are present. 
 * 
 * 	It defines a build tool, and it's version. For example:
 * 		LD 520.0.0
 */
struct build_tool_version
{
	uint32_t	tool;		/* enum for the tool */
    uint32_t	version;	/* version number of the tool */
};


/**
 * 	This is a neater version of build_tool_version that has an actual char *
 * 	for the tool name, and then the build version as is found in build_tool_version.
 * 
 */
typedef struct build_tool_info_t {
	char		*tool;
	uint32_t	version;
} build_tool_info_t;


/**
 * 	This struct brings all the Build version types together. It contains the 
 * 	original build version Load Command, but also string reps of the platform
 * 	minos, sdk, the number of build tools, and a HSList of tools.
 * 
 */
typedef struct mach_build_version_info_t {
	mach_build_version_command_t *cmd;
	
	char 		*platform;
	char		*minos;
	char		*sdk;

	uint32_t	 ntools;
	HSList 		*tools;
} mach_build_version_info_t;

/* Known values for the platform field above. */
#define PLATFORM_MACOS 1
#define PLATFORM_IOS 2
#define PLATFORM_TVOS 3
#define PLATFORM_WATCHOS 4
#define PLATFORM_BRIDGEOS 5
#define PLATFORM_MACCATALYST 6
#define PLATFORM_IOSSIMULATOR 7
#define PLATFORM_TVOSSIMULATOR 8
#define PLATFORM_WATCHOSSIMULATOR 9
#define PLATFORM_DRIVERKIT 10

/* Known values for the tool field above. */
#define TOOL_CLANG 1
#define TOOL_SWIFT 2
#define TOOL_LD	3


mach_build_version_info_t 		*mach_lc_build_version_info (mach_build_version_command_t *bvc, off_t offset, macho_t *macho);


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/**
 * 	The mach_uuid_command_t is an optional load command containing
 * 	the UUID of the binary.
 */
typedef struct mach_uuid_command_t {
	uint32_t	cmd;			/* LC_UUID */
	uint32_t	cmdsize;		/* sizeof(mach_uuid_command_t) */
	uint8_t		uuid[16];		/* 128-bit UUID */
} mach_uuid_command_t;


mach_uuid_command_t 		*mach_lc_find_uuid_cmd (macho_t *macho);
char 						*mach_lc_uuid_string (mach_uuid_command_t *cmd);


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

#endif /* libhelper_macho_command_type_ll_h_ */