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


mach_load_command_t *mach_load_command_create ()
{
    mach_load_command_t *lc = malloc(sizeof(mach_load_command_t));
    memset (lc, '\0', sizeof(mach_load_command_t));
    return lc;
}

mach_load_command_t *mach_load_command_load (file_t *file, off_t offset)
{
    mach_load_command_t *lc = mach_load_command_create ();
    lc = (mach_load_command_t *) file_load_bytes (file, sizeof(mach_load_command_t), offset);

    if (!lc) {
        g_print ("[*] Error: Problem with loading the Mach Load Command for offset 0x%x\n", offset);
        return NULL;
    }

    return lc;
}

void mach_load_command_dump (mach_load_command_t *lc)
{
    char *cmd_str = "";
    switch (lc->cmd) {
        case LC_SEGMENT:
            cmd_str = "LC_SEGMENT";
            break;
        case LC_SYMTAB:
            cmd_str = "LC_SYMTAB";
            break;
        case LC_SYMSEG:
            cmd_str = "LC_SYMSEG";
            break;
        case LC_THREAD:
            cmd_str = "LC_THREAD";
            break;
        case LC_UNIXTHREAD:
            cmd_str = "LC_UNIXTHREAD";
            break;
        case LC_LOADFVMLIB:
            cmd_str = "LC_LOADFVMLIB";
            break;
        case LC_IDFVMLIB:
            cmd_str = "LC_IDFVMLIB";
            break;
        case LC_IDENT:
            cmd_str = "LC_IDENT";
            break;
        case LC_FVMFILE:
            cmd_str = "LC_FVMFILE";
            break;
        case LC_PREPAGE:
            cmd_str = "LC_PREPAGE";
            break;
        case LC_DYSYMTAB:
            cmd_str = "LC_DYSYMTAB";
            break;
        case LC_LOAD_DYLIB:
            cmd_str = "LC_LOAD_DYLIB";
            break;
        case LC_ID_DYLIB:
            cmd_str = "LC_ID_DYLIB";
            break;
        case LC_LOAD_DYLINKER:
            cmd_str = "LC_LOAD_DYLINKER";
            break;
        case LC_ID_DYLINKER:
            cmd_str = "LC_ID_DYLINKER";
            break;
        case LC_PREBOUND_DYLIB:
            cmd_str = "LC_PREBOUND_DYLIB";
            break;
        case LC_SEGMENT_64:
            cmd_str = "LC_SEGMENT_64";
            break;
        default:
            cmd_str = "LC_UKNOWN";
            break;
    }

    g_print ("Command:\t\t%s\n", cmd_str);
    g_print ("Command Size:\t\t%d\n", lc->cmdsize);
}