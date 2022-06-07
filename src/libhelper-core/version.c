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

/* libhelper-core: version.c */

#include <libhelper.h>
#include "version.h"

char *libhelper_get_version ()
{
    return LIBHELPER_VERSION;
}

char *libhelper_get_version_string ()
{
    return LIBHELPER_VERS_WITH_ARCH;
}

char *libhelper_get_build_type ()
{
    return LIBHELPER_VERSION_TYPE;
}

char *libhelper_get_build_target ()
{
    return BUILD_TARGET;
}

char *libhelper_get_build_target_formatted ()
{
    return BUILD_TARGET_CAP;
}

char *libhelper_get_build_arch ()
{
    return BUILD_ARCH;
}

char *libhelper_get_build_arch_formatted ()
{
    return BUILD_ARCH_CAP;
}