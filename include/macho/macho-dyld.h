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

#ifndef LIBHELPER_MACHO_DYLD_H
#define LIBHELPER_MACHO_DYLD_H

/**
 *                  === The Libhelper Project ===
 *                          Mach-O Parser
 * 
 *  Documentation relating to the macho-dyld.h header file:          |
 *                                                                      |
 *                                                                      |
 * 
 *  ----------------
 *  Original Author:
 *      Harry Moulton, @h3adsh0tzz  -   me@h3adsh0tzz.com.
 * 
 */

/**
 *  The Dynamic Loader Info Load Command is defined in macho-command.h
 * 
 */
#include "macho-command.h"

#include <glib-2.0/glib.h>

typedef struct dyld_rebase_info {
    GSList      *opcodes;       /* List of Opcodes */
    GSList      *actions;       /* List of Actions */
} dyld_rebase_info;


/**
 * 
 * 
 */


#endif /* libhelper_macho_dyld_h */