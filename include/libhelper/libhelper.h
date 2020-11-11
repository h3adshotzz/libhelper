//===----------------------------- libhelper ----------------------------===//
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
//  me@h3adsh0tzz.com.
//
//
//===------------------------------------------------------------------===//

/***********************************************************************
* Libhelper API.
*
*   The Libhelper Library
*
***********************************************************************/
//===----------------------------- libhelper ----------------------------===//
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
//  me@h3adsh0tzz.com.
//
//
//===------------------------------------------------------------------===//

#ifndef LIBHELPER_H
#define LIBHELPER_H

#include <stdio.h>

#include "libhelper/strutils.h"
#include "libhelper/file.h"


/***********************************************************************
* Libhelper Version.
*
*   Four definitions for Libhelper version info. The long version string
*   has a similar format to XNU's versioning. There is then a more human
*   readable "marketing" version number with a traditional Major.Minor.Rev
*   format, and finally two more definitions with the version Tag both
*   normal and capatalised.
*
***********************************************************************/

#define LIBHELPER_VERSION			"1.1.0"
#define LIBHELPER_VERSION_LONG		"libhelper-1200.643.47"
#define LIBHELPER_VERSION_TYPE		"DEVELOPMENT"

#define LIBHELPER_DEBUG				1

int libhelper_is_debug ();
char *libhelper_version_string ();

/***********************************************************************
* Libhelper Platform.
*
*   The platform type, either "Darwin" for macOS/iOS et al. And "Linux"
*   for Linux-based systems. Only the correct one is defined, based on
*   the system used to compile Libhelper.
*
***********************************************************************/

#ifdef __APPLE__
#   define BUILD_TARGET         	"darwin"
#   define BUILD_TARGET_CAP     	"Darwin"
#else
#   define BUILD_TARGET         	"linux"
#   define BUILD_TARGET_CAP     	"Linux"
#endif

#ifdef __x86_64__
#   define BUILD_ARCH           	"x86_64"
#   define BUILD_ARCH_CAP          	"X86_64"
#elif __arm64__
#   define BUILD_ARCH           	"arm64"
#   define BUILD_ARCH_CAP          	"ARM64"
#elif __arm__
#   define BUILD_ARCH           	"arm"
#   define BUILD_ARCH_CAP         	"ARM"
#endif


/***********************************************************************
* Libhelper Architecture String.
*
*   The architecture type, either x86_64, arm64 or arm. It's prepended
*   with the long version, and the version tag. Only the correct one
*   is defined and is based on the system used to compile Libhelper.
*
***********************************************************************/

#define LIBHELPER_VERS_WITH_ARCH     LIBHELPER_VERSION_LONG "/" LIBHELPER_VERSION_TYPE "_" BUILD_ARCH_CAP " " BUILD_ARCH



#endif /* libhelper_h */
