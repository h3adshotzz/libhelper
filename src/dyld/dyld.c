//===------------------------------ macho -----------------------------===//
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

#include "libhelper-dyld/dyld.h"

/***********************************************************************
* DYLD Shared Cache Header functions.
***********************************************************************/

dyld_cache_header_t *dyld_cache_header_create ()
{
    dyld_cache_header_t *ret = malloc (sizeof (dyld_cache_header_t));
    memset (ret, '\0', sizeof (dyld_cache_header_t));
    return ret;
}

/*dyld_cache_header_t *dyld_cache_header_load (dyld_cache_t *dyld)
{

}*/