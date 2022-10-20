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
//  Copyright (C) 2022, Is This On? Holdings Limited
//
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#ifndef __LIBHELPER_IMAGE4_H__
#define __LIBHELPER_IMAGE4_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Determine whether to use CommonCrypto or OpenSSL based on the platform we're
 * compiling on. If on macOS, use CommonCrypto.framework, otherwise use OpenSSL.
 */
#if defined(__APPLE__) && defined(__MACH__)
#    define LIBHELPER_IMAGE4_CRYPTO_LIB_COMMONCRYPTO
#    include <CommonCrypto/CommonCrypto.h>
#elif
#    define LIBHELPER_IMAGE4_CRYPTO_LIB_OPENSSL
#    include <openssl/aes.h>
#    include <openssl/sha.h>
#endif

#include "libhelper-hlibc.h"
#include "libhelper-asn1.h"


/**
 *  Image4 component type collection.
 */
#define IMAGE_TYPE_DIAG					"diag"	// diagnostics
#define IMAGE_TYPE_LLB					"illb"	// iboot first-stage loader
#define IMAGE_TYPE_IBOOT				"ibot"	// iboot second-stage loader
#define IMAGE_TYPE_IBSS					"ibss"	// iboot single stage
#define IMAGE_TYPE_IBEC					"ibec"	// iboot epoch change
#define IMAGE_TYPE_DEVTREE				"dtre"	// darwin device tree
#define IMAGE_TYPE_RAMDISK				"rdsk"	// darwin ram disk for restore
#define IMAGE_TYPE_KERNELCACHE			"krnl"	// darwin kernel cache
#define IMAGE_TYPE_LOGO					"logo"	// boot logo image
#define IMAGE_TYPE_RECMODE				"recm"	// recovery mode image
#define IMAGE_TYPE_NEEDSERVICE			"nsrv"	// need service image
#define IMAGE_TYPE_GLYPHCHRG			"glyC"	// glyph charge image
#define IMAGE_TYPE_GLYPHPLUGIN			"glyP"	// glyph plug in image
#define IMAGE_TYPE_BATTERYCHARGING0		"chg0"  // battery charging image - bright
#define IMAGE_TYPE_BATTERYCHARGING1		"chg1"  // battery charging image - dim
#define IMAGE_TYPE_BATTERYLOW0			"bat0"	// battery low image - empty
#define IMAGE_TYPE_BATTERYLOW1			"bat1"	// battery low image - red (composed onto empty)
#define IMAGE_TYPE_BATTERYFULL			"batF"	// battery full image list
#define IMAGE_TYPE_OS_RESTORE			"rosi"	// OS image for restore
#define IMAGE_TYPE_SEP_OS				"sepi"	// SEP OS image
#define IMAGE_TYPE_SEP_OS_RESTORE		"rsep"	// SEP OS image for restore
#define IMAGE_TYPE_HAMMER				"hmmr"	// PE's Hammer test

/**
 *  \brief		Image4 Component Types
 */
typedef enum _img4type		img4type_t;
enum _img4type {
	IMAGE4_COMP_TYPE_UNKNOWN,
	IMAGE4_COMP_TYPE_IMG4,
	IMAGE4_COMP_TYPE_IM4P,
	IMAGE4_COMP_TYPE_IM4M,
	IMAGE4_COMP_TYPE_IM4R
};

/**
 *  \brief		KBAG Types.
 */
typedef enum _kbagtype		kbag_type_t;
enum _kbagtype {
	IMAGE4_KBAG_PRODUCTION,
	IMAGE4_KBAG_DEVELOPMENT,
	IMAGE4_KBAG_UNKNOWN,
};

/**
 * 	\brief		Image4 KBAG structure.
 */
typedef struct _kbag		kbag_t;
struct _kbag {
	kbag_type_t		type; 		/* PRODUCTION, DEVELOPMENT, UNKNOWN */
	uint8_t 		iv[16]; 	/* KBAG IV ASN1 Tag */
	uint8_t 		key[32]; 	/* KBAG Key ASN1 Tag */
};

/**
 * 	\brief		Image4 IM4P Info Structure
 */
typedef struct _im4p 		im4p_t;
struct _im4p
{
	uint32_t	 	 offset; 	/* IM4P offset within Image4 */
	uint32_t	 	 size; 		/* IM4P Size */
	char 			*comp; 		/* IM4P Component Name */
	char 			*desc; 		/* IM4P Component Description */

	uint32_t  		 flags; 	/* Flags */
	HSList 			*kbags; 	/* Collection of parsed KBAGs */
};


/* Flags for im4p_t */
#define IM4P_FLAG_FILE_ENCRYPTED 			(1 << 1) 	/* IM4P payload is encrypted */
#define IM4P_FLAG_FILE_NOT_ENCRYPTED 		(1 << 2)	/* IM4P payload is not encrypted */
#define IM4P_FLAG_FILE_COMPRESSED_LZSS 		(1 << 3)	/* IM4P payload is compressed with lzss */
#define IM4P_FLAG_FILE_COMPRESSED_BVX2 		(1 << 4)	/* IM4P payload is compressed with bvx2 */
#define IM4P_FLAG_FILE_COMPRESSED_NONE 		(1 << 5)	/* IM4P payload is not compressed */
#define IM4P_FLAG_INCLUDES_KBAG				(1 << 6)	/* IM4P payload contains a KBAG */
#define IM4P_FLAG_NO_KBAG					(1 << 7)	/* IM4P payload does not contain a KBAG */


typedef struct _manifest_entry		manifest_entry_t;
struct _manifest_entry
{
	char			*name;
	char			*data;
};

typedef struct _manifest			manifest_t;
struct _manifest
{
	char			*name;		/* manifest name */
	HSList			*entries;	/* manifest entries */
};

/**
 * 
 */
typedef struct _im4m				im4m_t;
struct _im4m 
{
	uint32_t	 	 offset; 	/* IM4M offset within Image4 */
	uint32_t	 	 size; 		/* IM4M Size */
	uint32_t		 flags;		/* Flags */

	uint64_t		 version;
	HSList			*manp;		/* IM4M Manifest Properties */
};



/**
 * 	\brief		Image4 Parsed File Structure
 */
typedef struct _image4				image4_t;
struct _image4
{
	/* File properties */
	unsigned char 	*data; 		/* Raw file data */
	uint32_t 		 size; 		/* File size */
	char 			*path; 		/* Filepath */

	/* Flags */
	uint32_t 		 flags;

	/* Components */
	img4type_t 		 type;
	im4p_t 			*im4p;
	im4m_t			*im4m;
	//im4r
};

/* Flags for image4_t */
#define IMAGE4_FLAG_INCLUDES_IM4P 			(1 << 15)	/* Image4 includes an IM4P */
#define IMAGE4_FLAG_INCLUDES_IM4M 			(1 << 16)	/* Image4 includes an IM4M */
#define IMAGE4_FLAG_INCLUDES_IM4R 			(1 << 17)	/* Image4 includes an IM4R */
#define IMAGE4_FLAG_SINGLE_COMPONENT		(1 << 18)	/* Image4 only includes a single component */
#define IMAGE4_FLAG_MODIFIED_DATA 			(1 << 19)	/* Image4 data has been modified by 


/**
 * 	\brief		Create a new image4_t structure and allocate memory.
 * 
 * 	\returns 	Allocated image4_t structure.
 */
extern image4_t *
image4_create ();

/**
 * 	\brief		Load an Image4 file from a given filepath and parse it into an
 * 				image4_t structure.
 * 
 * 	\param path		Path to the Image4 file to load.
 * 
 * 	\returns	A loaded and parsed image4_t structure, or NULL if there was an
 * 				error.
 */
extern image4_t *
image4_load (const char *path);

/**
 * 	\brief		Calculate from a given image4_t the type of component, e.g. IMG4,
 * 				IM4P, IM4R, etc.
 * 
 * 	\param image4	Image4 to analyse.
 * 
 * 	\return		An img4type_t type.
 */
extern img4type_t
image4_get_file_type (image4_t *image4);

/**
 * 	\brief		Get the name of a given img4type_t, e.g. "IM4M" for IMAGE4_COMP_TYPE_IM4M.
 * 
 * 	\param type		img4type_t to return a string for.
 * 
 * 	\return		A string for the given img4type_t.
 */
extern char *
image4_get_file_type_name (img4type_t type);

extern char *
image4_get_file_type_description (image4_t *image4);

/**
 * 	\brief		Get the name of the firmware component contained within an image, e.g. "ibot"
 * 
 * 	\param image4	Image4 to analyse.
 * 
 * 	\return		The string of the component contained within the image.
 */
extern char *
image4_get_component_type_name (image4_t *image4);

/**
 * 	\brief		Get a string description of the component contained within the given image.
 * 				e.g. for iBEC, "iBoot Epoch Change (iBEC)".
 * 
 * 	\param image4	Image4 to analyse.
 * 
 * 	\return		A string description of the component contained within the image.
 */
extern char *
image4_get_component_type_description (image4_t *image4);

/**
 * 	\brief		Get the type of compression used on the payload contained within a given
 * 				im4p.
 * 
 * 	\param buf		An IM4P payload buffer.
 * 
 * 	\return		The type of compression used, e.g. IMAGE4_FILE_COMPRESSED_LZSS.
 */
extern int
image4_get_compression_type (char *buf);

im4p_t *
image4_parse_im4p (unsigned char *buf);

im4m_t *
image4_parse_im4m (unsigned char *buf);




#ifdef __cplusplus
}
#endif

#endif /* __libhelper_image4_h__ */
