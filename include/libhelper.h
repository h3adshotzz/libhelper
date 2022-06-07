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
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-core: */

#ifndef __LIBHELPER_CORE_H__
#define __LIBHELPER_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/mman.h>

/******************************************************************************
* Libhelper Version helpers.
* 
* API for determining the version of libhelper and the architecture/os libhelper
* was compiled for.
* 
*******************************************************************************/

/**
 *  \brief      Get library version number.
 * 
 *  \returns    Libhelper version number in the format 'libhelper-0000.00.00~0'.
 */
extern char *
libhelper_get_version ();

/**
 *  \brief      Get library version string.
 * 
 *  \returns    Long libhelper version string. This is in the foramted:
 *              'libhelper-0000.00.00~0/BUILDTYPE_ARCH ARCH'
 */
extern char *
libhelper_get_version_string ();

/**
 *  \brief      Get libhelper build type.
 * 
 *  \returns    Libhelper build type, either RELEASE, DEVELOPMENT or DEBUG.
 */
extern char *
libhelper_get_build_type ();

/**
 *  \brief      Get libhelper build target.
 * 
 *  \returns    Libhelper build target, e.g. Darwin or Linux. The formatted func
 *              call will returned the build target with capitals.
 */
extern char *
libhelper_get_build_target ();
extern char *
libhelper_get_build_target_formatted ();

/**
 *  \brief      Get libhelper build architecture.
 *  
 *  \returns    Libhelper build architecture, e.g. AArch64 or x86_64. The formatted 
 *              func call will returned the build architecture with capitals.
 */
extern char *
libhelper_get_build_arch ();
extern char *
libhelper_get_build_arch_formatted ();


/******************************************************************************
* Libhelper Boyermoore Horspool memmem().
* 
* Boyermoore Horspool, or Horspool's Algorithim, is an algorithm for finding 
* substrings in a buffer.
* 
*******************************************************************************/

#define __UCHAR_MAX        255

/**
 *  \brief      Boyermoore Horspool memmem().
 * 
 *  In computer science, the Boyer–Moore–Horspool algorithm or Horspool's algorithm 
 *  is an algorithm for finding substrings in strings. It was published by Nigel 
 *  Horspool in 1980 as SBM. (Wikipedia).
 * 
 *  \param  haystack    Haystack.
 *  \param  h_len       Haystack length.
 *  \param  needle      Needle.
 *  \param  n_len       Needle length.
 * 
 *  \returns    Pointer to substring.
 */
extern unsigned char *
bh_memmem (const unsigned char      *haystack,
           size_t                    h_len,
           const unsigned char      *needle,
           size_t                    n_len);


#ifdef __cplusplus
}
#endif

#endif /* __libhelper_core_h__ */