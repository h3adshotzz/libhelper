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
//  Copyright (C) 2021, Is This On? Holdings
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-core: version.h */

#ifndef __LIBHELPER_VERSION_H__
#define __LIBHELPER_VERSION_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Libhelper version strings, generated by tools/libhelper_version.py on build.
 */
#define LIBHELPER_VERSION                   "libhelper-3000.00.00~0"//###LIBHELPER_VERSION_STRING###
#define LIBHELPER_VERSION_TYPE              "DEBUG"//###LIBHELPER_VERSION_TYPE_STRING###

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

/**
 *  Full Libhelper version string. This is documented at <link>.
 */
#define LIBHELPER_VERS_WITH_ARCH     LIBHELPER_VERSION "/" LIBHELPER_VERSION_TYPE "_" BUILD_ARCH_CAP " " BUILD_ARCH


#ifdef __cplusplus
}
#endif

#endif /* __libhelper_version_h__ */