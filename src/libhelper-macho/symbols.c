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
//  Copyrihgt (C) 2023, Is This On? holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#include "libhelper.h"
#include "libhelper-macho.h"
#include "libhelper-logger.h"

mach_symtab_command_t *
mach_symtab_command_load (macho_t *macho, uint32_t offset)
{
    /* create a symbol table struct */
    mach_symtab_command_t *table = malloc (sizeof (mach_symtab_command_t));
    memset (table, '\0', sizeof (mach_symtab_command_t));

    /* load the data from the macho into the table struct */
    table = (mach_symtab_command_t *) macho_load_bytes (macho, sizeof (mach_symtab_command_t), offset);

    if (!table) {
        errorf ("mach_symtab_command_load(): problem loading mach symbol table at offset: 0x%08x\n", offset);
        return NULL;
    }
    return table;
}

char *
mach_symbol_table_find_symbol_name (macho_t *macho, nlist *sym, mach_symtab_command_t *table)
{
    /* offset of the symbol table name is symbol->off + nlist->n_strx */
    uint32_t size = table->strsize - sym->n_strx;
    uint32_t offset = table->stroff + sym->n_strx;

    HString *curr = h_string_new ("");
    uint32_t found = 0, i = 0;
    char *tmp = malloc (size);

    memcpy (tmp, macho->data + offset, size);
    while (!found) {
        if (i >= size) break;
        if (tmp[i] != 0x0) {
            curr = h_string_append_c (curr, tmp[i]);
            i++;
        } else {
            if (curr->str && curr->len > 0) {
                found = 1;
                return curr->str;
            }
            break;
        }
    }
    return LIBHELPER_MACHO_SYMBOL_NO_NAME;
}

/*
char *
mach_symtab_load_symbols (macho_t *macho, mach_symtab_command_t *table)
{

}*/