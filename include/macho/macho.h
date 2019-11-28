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
 *  Documentation relating to the macho.h header file:          |
 *                                                                      |
 *                                                                      |
 * 
 *  ----------------
 *  Original Author:
 *      Harry Moulton, @h3adsh0tzz  -   me@h3adsh0tzz.com.
 * 
 */

#include "file.h"
#include "macho/macho-header.h"

/**
 *  Full Mach-O file representation containing all the properties of a
 *  Mach-O File.
 * 
 */
typedef struct macho_t {
	file_t 			*file;
	mach_header_t 	*header;
	GSList			*lcmds;				/* mach_command_info_t */
	GSList			*scmds;				/* mach_segment_info_t */
	GSList			*symbols;			/* mach_symbol_info_t */
	GSList			*strings;			/* list of strings, in order. */
} macho_t;


/**
 * 	Functions to safely create a macho_t structure and to load a Mach-O file
 * 	into a macho_t struct.
 * 
 */
macho_t *macho_create ();
macho_t *macho_load (file_t *file);


#endif /* libhelper_macho_h */