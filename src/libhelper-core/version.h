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

/* libhelper-core: version.h */

#ifndef __LIBHELPER_VERSION_H__
#define __LIBHELPER_VERSION_H__

/* Libhelper Build Version, i.e. 3.1.2 */
#define LIBHELPER_BUILD_VERSION_MAJOR       "3"
#define LIBHELPER_BUILD_VERSION_MINOR       "1"
#define LIBHELPER_BUILD_VERSION_REVISION    "0"
#define LIBHELPER_BUILD_VERSION             "3.1.0"

/* Libhelper Build Type, i.e. INTERNAL, BETA, RELEASE */
#define LIBHELPER_BUILD_TYPE                "DEBUG"

/* Libhelper Source Version, i.e. 312.56.12 */
#define LIBHELPER_SOURCE_VERSION_MAJOR      "36"
#define LIBHELPER_SOURCE_VERSION_MINOR      "32"

#define LIBHELPER_SOURCE_VERSION            "libhelper-310.36.32"

/**
 *  Libhelper platform string. Correct value is set at compile time by the compiler.
 */
#ifdef __APPLE__
#	define BUILD_TARGET				"darwin"
#	define BUILD_TARGET_CAP			"Darwin"
#else
#	define BUILD_TARGET				"linux"
#	define BUILD_TARGET_CAP			"Linux"
#endif

/**
 *	NOTE: As of libhelper-2000.16.4, support for 32-bit architecture is
 *		  no longer available.
 */
#ifdef __x86_64__
#	define BUILD_ARCH				"x86_64"
#	define BUILD_ARCH_CAP			"X86_64"
#elif __arm64__
#	define BUILD_ARCH				"arm64"
#	define BUILD_ARCH_CAP			"ARM64"
#else
#	error "Unsupported Archiecture."
#endif

#endif /* __libhelper_version_h__ */