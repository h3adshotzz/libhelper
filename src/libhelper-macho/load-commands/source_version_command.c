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

mach_source_version_command_t *
mach_load_command_find_source_version_command (macho_t *macho)
{
    size_t size = sizeof (mach_source_version_command_t);
    mach_source_version_command_t *ret = calloc (1, size);

    /* search the macho->lcmds list for the SVC */
    for (int i = 0; i < h_slist_length (macho->lcmds); i++) {
        mach_load_command_info_t *inf = (mach_load_command_info_t *) h_slist_nth_data (macho->lcmds, i);
        if (inf->lc->cmd == LC_SOURCE_VERSION) {
            ret = (mach_source_version_command_t *)
                    macho_load_bytes (macho, size, inf->offset);
            break;
        }
    }

    return (ret) ? ret : NULL;
}

char *
mach_load_command_get_source_version_string (mach_source_version_command_t *svc)
{
    char *ret = calloc(1, 20);
    uint64_t a, b, c, d, e;

    if (svc->cmdsize != sizeof (mach_source_version_command_t))
        goto svc_error;

    a = (svc->version >> 40) & 0xffffff;
    b = (svc->version >> 30) & 0x3ff;
    c = (svc->version >> 20) & 0x3ff;
    d = (svc->version >> 10) & 0x3ff;
    e = svc->version & 0x3ff;

    if (e != 0) {
        snprintf (ret, 20, "%llu.%llu.%llu.%llu.%llu", a, b, c, d, e);
    } else if (d != 0) {
        snprintf (ret, 16, "%llu.%llu.%llu.%llu", a, b, c, d);
    } else if (c != 0) {
        snprintf (ret, 12, "%llu.%llu.%llu", a, b, c);
    } else {
        snprintf (ret, 8, "%llu.%llu", a, b);
    }

    return ret;

svc_error:
    errorf ("mach_load_command_get_source_version_string: could not load source version string.\n");
    return NULL;
}
