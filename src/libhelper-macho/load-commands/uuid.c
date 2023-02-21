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
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//  Copyright (C) 2020, Is This On?, @h3adsh0tzz
//  Copyright (C) 2021, Is This On? Holdings Limited
//
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#include "libhelper-logger.h"
#include "libhelper-macho.h"

char *
mach_load_command_uuid_parse_string (mach_uuid_command_t *uuid)
{
    if (uuid->cmdsize != sizeof (mach_uuid_command_t)) {
        return NULL;
    }

    uint32_t size = sizeof (uint8_t) * 128;
    char *ret = calloc (1, size);
    snprintf (ret, size, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                            (unsigned int)uuid->uuid[0], (unsigned int)uuid->uuid[1],
                            (unsigned int)uuid->uuid[2],  (unsigned int)uuid->uuid[3],
                            (unsigned int)uuid->uuid[4],  (unsigned int)uuid->uuid[5],
                            (unsigned int)uuid->uuid[6],  (unsigned int)uuid->uuid[7],
                            (unsigned int)uuid->uuid[8],  (unsigned int)uuid->uuid[9],
                            (unsigned int)uuid->uuid[10], (unsigned int)uuid->uuid[11],
                            (unsigned int)uuid->uuid[12], (unsigned int)uuid->uuid[13],
                            (unsigned int)uuid->uuid[14], (unsigned int)uuid->uuid[15]);

    return (ret) ? ret : NULL;
}

char *
mach_load_command_uuid_string_from_macho (macho_t *macho)
{
    mach_load_command_info_t *info = mach_load_command_find_command_by_type (macho, LC_UUID);
    if (!info) return "(null)";

    mach_uuid_command_t *uuid = (mach_uuid_command_t *) info->lc;
    return mach_load_command_uuid_parse_string (uuid);
}