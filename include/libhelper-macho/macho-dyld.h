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
#include "libhelper-macho/macho-command.h"


typedef struct dyld_rebase_info {
    HSList      *opcodes;       /* List of Opcodes */
    HSList      *actions;       /* List of Actions */
} dyld_rebase_info;


/**
 *  [Notes]
 * 
 *  These are my notes for the Dynamic Loader info.
 * 
 *  The rebase info, for example, has the following hex value.
 * 
 *      11 23 00 60 10 00 00 00
 * 
 *  But the following translated value (opcodes).
 * 
 *      11      REBASE_OPCODE_SET_TYPE_IMM                      type (1, REBASE_TYPE_POINTER)
 *      23      REBASE_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB       segment (3)
 *      00      uleb128                                         offset (0)
 *      60      REBASE_OPCODE_DO_REBASE_ULEB_TIMES              ...
 *      10      uleb128                                         count (16)
 *      -------
 *      00      REBASE_OPCODE_DONE
 * 
 *  And the actions list looks like:
 *      
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003000     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003008     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003010     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003018     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003020     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003028     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003030     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003038     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003040     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003048     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003050     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003058     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003060     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003068     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003070     Pointer
 *      00004000    ...     __DATA __la_symbol_ptr      0x100003078     Pointer   
 * 
 */


#endif /* libhelper_macho_dyld_h */