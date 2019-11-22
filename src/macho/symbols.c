/**
 * 
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

#include "macho.h"


mach_symtab_command_t *mach_symtab_command_create ()
{
    mach_symtab_command_t *symt = malloc(sizeof(mach_symtab_command_t));
    memset (symt, '\0', sizeof(mach_symtab_command_t));
    return symt;
}

mach_symtab_command_t *mach_symtab_command_load (file_t *file, off_t offset)
{
    mach_symtab_command_t *symt = mach_symtab_command_create ();
    symt = (mach_symtab_command_t *) file_load_bytes (file, sizeof(mach_symtab_command_t), offset);

    if (!symt) {
        g_print ("[*] Error: Problem loading Mach Symbol Table at offset: 0x%llx\n", offset);
        return NULL;
    }

    return symt;
}


//////////////////////////////////////////////////////////////////////////
//                        Mach-O String Table                           //
//////////////////////////////////////////////////////////////////////////

GSList *mach_load_string_table (macho_t *macho)
{
    return NULL;
}
