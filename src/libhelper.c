/**
 *     libhelper
 *     Copyright (C) 2019, @h3adsh0tzz
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include "libhelper.h"

/**
 *  The point of libhelper is to have some utilities to help with different
 *  projects. Some of the stuff i want to add:
 *      - ASN1 parser
 *      - MachO parser
 *      - File handling with glib
 *   
 *  I want to have build args to only build parts of libhelper that are required
 *  for a certain project, e.g. libhelper-macho, libhelper-asn1 etc.
 * 
 *  Also i need to sort out some sort of a versioning system.
 * 
 * 
 */


char *libhelper_version_string ()
{
    char *vers = NULL;
    char *platform = NULL;

#ifdef __APPLE__
    platform = "Darwin";
#else
    platform = "Linux";
#endif


#ifdef __x86_64__
    vers = " Libhelper Version 1.0.0 Beta 1; " __TIMESTAMP__ "; LIBHELPER_DEVELOPMENT_X86_64; x86_64";
#elif __arm__
    vers = " Libhelper Version 1.0.0 Beta 1; " __TIMESTAMP__ "; LIBHELPER_DEVELOPMENT_ARM64; arm64";
#else 
    vers = " Libhelper Version 1.0.0 Beta 1; " __TIMESTAMP__ "; LIBHELPER_DEVELOPMENT; unknown_architecture";
#endif

    return strappend (platform, vers);
}