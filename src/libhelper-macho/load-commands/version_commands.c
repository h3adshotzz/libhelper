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
//  Copyright (C) 2019-2020, Is This On?, @h3adsh0tzz
//  Copyright (C) 2021-2022, Is This On? Holdings Limited
//  
//  Contact: Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/**
 *  This file includes handlers for:
 *      - source_version_command
 *      - build_version_command
 *      - version_min_command
 */

#include "libhelper-logger.h"
#include "libhelper-macho.h"

static char *
unpack_nibble_encoded_version (uint32_t vers)
{
    char *ret = malloc (10);
    if (vers == 0) {
        ret = "(null)";
    } else {
        if ((vers & 0xff) == 0) snprintf (ret, 10, "%u.%u", vers >> 16, (vers >> 8) & 0xff);
        else snprintf (ret, 10, "%u.%u.%u", vers >> 16, (vers >> 8) & 0xff, vers & 0xff);
    }
    debugf ("nibble_encoded_version: %s\n", ret);
    return ret;
}

static char *
unpack_abcde_encoded_version (uint64_t vers)
{
    char *ret = calloc (1, 20);
    uint64_t a, b, c, d, e;

    a = (vers >> 40) & 0xffffff;
    b = (vers >> 30) & 0x3ff;
    c = (vers >> 20) & 0x3ff;
    d = (vers >> 10) & 0x3ff;
    e = vers & 0x3ff;

    if (e != 0) snprintf (ret, 20, "%llu.%llu.%llu.%llu.%llu", a, b, c, d, e);
    else if (d != 0) snprintf (ret, 16, "%llu.%llu.%llu.%llu", a, b, c, d);
    else if (c != 0) snprintf (ret, 12, "%llu.%llu.%llu", a, b, c);
    else snprintf (ret, 8, "%llu.%llu", a, b);
    
    debugf ("abcde_encoded_version: %s\n", ret);
    return ret;
}

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

    /* set the minos and sdk versions */
    info->minos = unpack_nibble_encoded_version (bvc->minos);
    info->sdk = unpack_nibble_encoded_version (bvc->sdk);

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

char *
mach_load_command_get_source_version_string (mach_source_version_command_t *svc)
{
    return unpack_abcde_encoded_version (svc->version);
}

char *
mach_load_command_get_min_version (mach_version_min_command_t *vmc)
{
    return unpack_abcde_encoded_version (vmc->version);
}

char *
mach_load_command_get_min_version_sdk (mach_version_min_command_t *vmc)
{
    return unpack_abcde_encoded_version (vmc->sdk);
}