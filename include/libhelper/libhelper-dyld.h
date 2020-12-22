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

#ifndef LIBHELPER_DYLD_H
#define LIBHELPER_DYLD_H

#ifdef cplusplus
extern "C" {
#endif

/**
 *                  === The Libhelper Project ===
 *                          Mach-O Parser
 *
 *  DYLD Parser aimed at being able to parse an iOS DYLD Shared Cache file
 *  and splitting it into induvidual libraries, like dsc_extractor but
 *  good. 
 * 
 *  ----------------
 *  Original Author:
 *      Harry Moulton, @h3adsh0tzz  -   me@h3adsh0tzz.com.
 *
 */

#include "libhelper/libhelper.h"


/***********************************************************************
* DYLD Shared Cache header.
***********************************************************************/

/**
 * 	DYLD Shared Cache Header magics
 */
#define DYLD_MAGIC_I386					"dyld_v1   i386"
#define DYLD_MAGIC_x86_64				"dyld_v1   x86_64"
#define DYLD_MAGIC_x86_64h				"dyld_v1   x86_64h"
#define DYLD_MAGIC_armv5				"dyld_v1   armv5"
#define DYLD_MAGIC_armv6				"dyld_v1   armv6"
#define DYLD_MAGIC_armv7				"dyld_v1   armv7"
#define DYLD_MAGIC_arm64				"dyld_v1   arm64"
#define DYLD_MAGIC_arm64_32				"dyld_v1arm64_32"
#define DYLD_MAGIC_arm64e				"dyld_v1   arm64e"


/**
 *  Header for the DYLD Shared Cache file found on iOS devices.
 * 
 */
struct dyld_cache_header {
    char		magic[16];				// e.g. "dyld_v0    i386"
	uint32_t	mappingOffset;			// file offset to first dyld_cache_mapping_info
	uint32_t	mappingCount;			// number of dyld_cache_mapping_info entries
	uint32_t	imagesOffset;			// file offset to first dyld_cache_image_info
	uint32_t	imagesCount;			// number of dyld_cache_image_info entries
	uint64_t	dyldBaseAddress;		// base address of dyld when cache was built
	uint64_t	codeSignatureOffset;	// file offset of code signature blob
	uint64_t	codeSignatureSize;		// size of code signature blob (zero means to end of file)
	uint64_t	slideInfoOffset;		// file offset of kernel slid info
	uint64_t	slideInfoSize;			// size of kernel slid info
	uint64_t	localSymbolsOffset;		// file offset of where local symbols are stored
	uint64_t	localSymbolsSize;		// size of local symbols information
	uint8_t		uuid[16];				// unique value for each shared cache file
	uint64_t	cacheType;				// 0 for development, 1 for production
	uint32_t	branchPoolsOffset;		// file offset to table of uint64_t pool addresses
	uint32_t	branchPoolsCount;	    // number of uint64_t entries
	uint64_t	accelerateInfoAddr;		// (unslid) address of optimization info
	uint64_t	accelerateInfoSize;		// size of optimization info
	uint64_t	imagesTextOffset;		// file offset to first dyld_cache_image_text_info
	uint64_t	imagesTextCount;		// number of dyld_cache_image_text_info entries   
};
typedef struct dyld_cache_header        dyld_cache_header_t;


/**
 * 	Cache Mapping Info
 */
struct dyld_cache_mapping_info {
	uint64_t	address;
	uint64_t	size;
	uint64_t	fileoff;
	uint32_t	maxProt;
	uint32_t	initProt;
};
typedef struct dyld_cache_mapping_info		dyld_cache_mapping_info_t;

/**
 * 	Cache Image Info
 */
struct dyld_cache_image_info {
	uint64_t	address;
	uint64_t	modTime;
	uint64_t	inode;
	uint32_t	pathFileOffset;
	uint32_t	pad;
};
typedef struct dyld_cache_image_info		dyld_cache_image_info_t;

/***********************************************************************
* DYLD Shared Cache Parser.
***********************************************************************/

/**
 * 
 */
typedef struct dyld_cache_t {

	/* file data */
	char		*path;		// file path

	/* raw file properties */
	uint8_t		*data;		// ptr to the dyld in memory
	uint32_t	 size;		// size of dyld
	uint32_t	 vmbase;	// virtual memory base addr.
	
	/* Parsed DYLD Cache properties */
	dyld_cache_header_t		*header;

} dyld_cache_t;


// dyld cache file parser
extern dyld_cache_t 				*dyld_cache_load 					(const char *filename);
extern dyld_cache_t                	*dyld_cache_create_from_file        (file_t *file);

extern void 						 dyld_read_bytes 					(dyld_cache_t *dyld, uint32_t offset, void *buffer, size_t size);
extern void 						*dyld_load_bytes 					(dyld_cache_t *dyld, uint32_t size, uint32_t offset);
extern void 						*dyld_get_bytes 					(dyld_cache_t *dyld, uint32_t offset);


// dyld cache header
extern dyld_cache_header_t         	*dyld_cache_header_create           ();
extern int                           dyld_shared_cache_verify_header    (unsigned char *dyld_ptr);

#ifdef cplusplus
}
#endif

#endif /* libhelper_dyld_h */