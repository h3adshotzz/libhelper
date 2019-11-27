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

#ifndef LIBHELPER_MACHO_H
#define LIBHELPER_MACHO_H

/**
 *                  === The Libhelper Project ===
 *                          Mach-O Parser
 * 
 *  Libhelper contains a fully-featured (eventually) Mach-O parser.     |
 *                                                                      |
 *                                                                      |
 *                                                                      |
 * 
 */


/**
 *  MY NOTES FOR SPLITTING MACHO.H
 * 
 *  - macho_t requires everything so needs to be seperated from mach_header_t
 *  
 * 
 */

#include <glib-2.0/glib.h>

/**
 *  Header:     macho-header.h
 *  --------------------------
 * 
 *  This header contains definitions, structs and functions relating to the
 *  layout, format, loading and parsing of the Mach-O header. 
 * 
 */
#include "macho/macho-header.h"

#endif