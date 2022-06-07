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

mach_build_version_info_t *
mach_load_command_build_version_info (mach_build_version_command_t *bvc, uint32_t offset, macho_t *macho)
{
    mach_build_version_info_t *info = calloc (1, sizeof (mach_build_version_info_t));

    /* set the platform */
    switch (bvc->platform) {
        case PLATFORM_MACOS:
            info->platform = "macOS";
            break;
        case PLATFORM_IOS:
            info->platform = "iOS";
            break;
        case PLATFORM_TVOS:
            info->platform = "TvOS";
            break;
        case PLATFORM_WATCHOS:
            info->platform = "WatchOS";
            break;
        case PLATFORM_BRIDGEOS:
            info->platform = "BridgeOS";
            break;
        case PLATFORM_MACCATALYST:
            info->platform = "macOS Catalyst";
            break;
        case PLATFORM_IOSSIMULATOR:
            info->platform = "iOS Simulator";
            break;
        case PLATFORM_TVOSSIMULATOR:
            info->platform = "TvOS Simulator";
            break;
        case PLATFORM_WATCHOSSIMULATOR:
            info->platform = "WatchOS Simulator";
            break;
        case PLATFORM_DRIVERKIT:
            info->platform = "DriverKit";
            break;
        default:
            info->platform = "(null)";
            break;
    }

    /* set the minos */
    char *minos_tmp = malloc (10);
    if ((bvc->minos & 0xff) == 0) {
        snprintf (minos_tmp, 10, "%u.%u", bvc->minos >> 16, (bvc->minos >> 8) & 0xff);
    } else {
        snprintf (minos_tmp, 10, "%u.%u", bvc->minos >> 16, (bvc->minos >> 8) & 0xff);
    }
    info->minos = minos_tmp;

    /* set the sdk */
    char *sdk_tmp = malloc (10);
    if (bvc->sdk == 0) {
        sdk_tmp = "(null)";
    } else {
        if ((bvc->sdk & 0xff) == 0) {
            snprintf (sdk_tmp, 10, "%u.%u", bvc->sdk >> 16, (bvc->sdk >> 8) & 0xff);
        } else {
            snprintf (sdk_tmp, 10, "%u.%u.%u", bvc->sdk >> 16, (bvc->sdk >> 8) & 0xff, bvc->sdk & 0xff);
        }
    }
    info->sdk = sdk_tmp;

    /* set the tools */
    info->ntools = bvc->ntools;
    off_t next_off = offset + sizeof(mach_build_version_command_t);
    for (uint32_t i = 0; i < info->ntools; i++) {

        struct build_tool_version *btv = (struct build_tool_version *) macho_load_bytes (macho, sizeof(struct build_tool_version), next_off);
        build_tool_info_t *inf = malloc (sizeof(build_tool_info_t));

        switch (btv->tool) {
            case TOOL_CLANG:
                inf->tool = "Clang";
                break;
            case TOOL_LD:
                inf->tool = "LD";
                break;
            case TOOL_SWIFT:
                inf->tool = "Swift";
                break;
            default:
                inf->tool = "(null)";
                debugf ("undefined tool: %d\n", btv->tool);
                break;
        }

        inf->version = btv->version;

        info->tools = h_slist_append (info->tools, inf);

        next_off += sizeof(mach_build_version_command_t);
    }

    /* return */
    return (info) ? info : NULL;
}
