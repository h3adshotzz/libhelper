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

#include "macho/macho-symbol.h"
#include "macho/macho-segment.h"


/**
 * 
 * 
 */
mach_symtab_command_t *mach_symtab_command_create ()
{
    mach_symtab_command_t *symt = malloc(sizeof(mach_symtab_command_t));
    memset (symt, '\0', sizeof(mach_symtab_command_t));
    return symt;
}


/**
 * 
 * 
 */
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


/**
 * 
 * 
 */
char *mach_symtab_find_symbol_name (file_t *file, nlist *sym, mach_symtab_command_t *cmd)
{
    /**
     *  The offset of the symbol name is symbol->off + nlist->n_strx
     */
    size_t s = cmd->strsize - sym->n_strx;
    off_t off = cmd->stroff + sym->n_strx;

    char *tmp = file_load_bytes (file, s, off);
    GString *curr = g_string_new ("");

    int found = 0, i = 0;
    while (!found) {
        if (i >= s) break;
        if (tmp[i] != 0x0) {
            curr = g_string_append_c (curr, tmp[i]);
            i++;
        } else {
            if (curr->str && curr->len > 0) {
                found = 1;
                return curr->str;
            }
            break;
        }
    }

    return "(no name)";
}   


/**
 * 
 * 
 */
mach_symbol_table_t *mach_symtab_load_symbols (macho_t *macho, mach_symtab_command_t *symbol_table)
{
    file_t *file = macho->file;

    /*
    
        sym name: __mh_execute_header
        sym index: 0x2
        sym type: 15
        sym section: 0x1

        000001Eb    String table index:     __mh_execute_header
        0F          Type
                    0E                      N_SECT
                    01                      N_EXT
        01          Section Index           1 (__TEXT,__text)
        0010        Description
                    0010                    REFERENCE_DYNAMICALLY
        0           Value                   (IGNORE)


        TODO:
            - Functions to unpack type, sect, desc and value
            - Function to print the symbol in a formatted way.

        00000000    Symbol Name:    __main
        00000000    Type:
                    0E              N_SECT
                    01              N_EXT
        00000001    Section         1 (__TEXT,__text)
        00000010    Description
                    0010            REFERENCE_DYNAMICALLY

    */


    g_print ("\n[*] Trying to load symbol table:\n\n");

    size_t s = symbol_table->nsyms;
    off_t off = symbol_table->symoff;

    for (int i = 0; i < s; i++) {
        nlist *tmp = (nlist *) file_load_bytes (file, sizeof(nlist), off);

        char *name = mach_symtab_find_symbol_name (file, tmp, symbol_table);

        // THIS WILL MOVE TO A SEPERATE FUNCTION
        g_print ("0x%08x \tSymbol Name:\t%s\n", tmp->n_strx, name);
        g_print ("0x%08x \tType:\n", tmp->n_type);


        if ((tmp->n_type & N_STAB) == N_STAB) g_print ("\t\t0x%02x\tN_STAB\n", N_STAB);

        if ((tmp->n_type & N_PEXT) == N_PEXT) g_print ("\t\t0x%02x\tN_PEXT\n", N_PEXT);

        // The N_TYPE can also have differnet types, for example
            // N_UNDF (0x0), N_ABS (0x2), N_SECT (0xe), N_PBUD (0xc)
            // and N_INDR (0xa)
            if ((tmp->n_type & N_UNDF) == N_UNDF) g_print ("\t\t0x%02x\tN_UNDF\n", N_UNDF);

            if ((tmp->n_type & N_ABS) == N_ABS) g_print ("\t\t0x%02x\tN_ABS\n", N_ABS);

            if ((tmp->n_type & N_SECT) == N_SECT) g_print ("\t\t0x%02x\tN_SECT\n", N_SECT);

            if ((tmp->n_type & N_PBUD) == N_PBUD) g_print ("\t\t0x%02x\tN_PBUD\n", N_PBUD);

            if ((tmp->n_type & N_INDR) == N_INDR) g_print ("\t\t0x%02x\tN_INDR\n", N_INDR);

        if ((tmp->n_type & N_EXT) == N_EXT) g_print ("\t\t0x%02x\tN_EXT\n", N_EXT); 


        if (tmp->n_sect) {
            // CHANGE FUNCTION TO ACCEPT MACHO INSTEAD OF FILE
            mach_section_64_t *section = mach_find_section (macho, tmp->n_sect);
            g_print ("0x%08x \tSection:\t%d (%s,%s)\n", tmp->n_sect, tmp->n_sect, section->segname, section->sectname);
        } else {
            g_print ("0x%08x \tSection:\tNO_SECT\n");
        }

        g_print ("\n");

        //g_print ("sym name: %s\n", name);
        //g_print ("sym index: 0x%x\n", tmp->n_strx);
        //g_print ("sym type: %d\n", tmp->n_type);
        //g_print ("sym section: 0x%x\n\n", tmp->n_sect);
        //g_print ("sym desc: %d\n", tmp->n_desc);
        //g_print ("sym val: %lu\n", tmp->n_value);

        off += sizeof(nlist);
    }

    return NULL;
}