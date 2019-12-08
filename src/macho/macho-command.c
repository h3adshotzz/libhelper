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

#include "macho/macho-command.h"


//////////////////////////////////////////////////////////////////////////
//                  Base Mach-O Load commands                           //
//////////////////////////////////////////////////////////////////////////


/**
 *  Function:   mach_load_command_create
 *  ------------------------------------
 * 
 *  Creates a new Mach-O Load Command structure and assigns sufficient memory. Should
 *  be called to safely create a new raw Load Command structure.
 * 
 *  returns:    A mach_load_command_t structure with sufficient allocated memory.
 * 
 */
mach_load_command_t *mach_load_command_create ()
{
    mach_load_command_t *lc = malloc(MACH_LOAD_COMMAND_SIZE);
    memset (lc, '\0', sizeof(mach_load_command_t));
    return lc;
}


/**
 *  Function:   mach_command_info_create
 *  ------------------------------------
 * 
 *  Creates a new Mach-O Load Command Info structure and assigns sufficient memory. 
 *  Should be called to safely create a new Load Command Info structure.
 * 
 *  returns:    A mach_command_info_t structure with sufficient allocated memory.
 * 
 */
mach_command_info_t *mach_command_info_create ()
{
    mach_command_info_t *cmd = malloc(MACH_COMMAND_INFO_SIZE);
    memset (cmd, '\0', sizeof(mach_command_info_t));
    return cmd;
}


/**
 *  Function:   mach_command_info_load
 *  ----------------------------------
 * 
 *  Loads a raw Mach-O Load Command from a given offset in a verified Mach-O file, and
 *  returns the resulting structure.
 *  
 *  file:       The verified Mach-O file.
 *  offset:     The offset of the Load Command within the file.
 * 
 *  returns:    A verified Mach Command Info structure.
 * 
 */
mach_command_info_t *mach_command_info_load (file_t *file, off_t offset)
{
    mach_command_info_t *ret = mach_command_info_create ();
    mach_load_command_t *lc = mach_load_command_create ();

    lc = (mach_load_command_t *) file_load_bytes (file, sizeof(mach_load_command_t), offset);
    if (!lc) {
        g_print ("[*] Error: Unable to find Load Command at offset 0x%llx\n", offset);
        return NULL;
    }

    ret->lc = lc;
    ret->off = offset;
    ret->type = lc->cmd;

    return ret;
}


/**
 *  Function:   mach_load_command_info_print
 *  ----------------------------------------
 * 
 *  Prints a given mach_command_info_t struct in a formatted way. 
 * 
 *  cmd:        The Mach Command Info to print
 * 
 */
void mach_load_command_info_print (mach_command_info_t *cmd)
{
    mach_load_command_print (cmd, LC_INFO);
    g_print ("--- Meta:\n");
    g_print ("  Type:\t0x%x\n", cmd->type);
    g_print ("Offset:\t0x%llx\n", cmd->off);
}


/**
 *  Function:   mach_load_command_print
 *  -----------------------------------
 * 
 *  Prints a given struct, either mach_load_command_t or mach_command_info_t
 *  in a formatted way, guided by a flag.
 * 
 *  cmd:        The Mach Load Command (mach_load_command_t / mach_command_info_t)
 *  flag:       LC_RAW / LC_INFO
 * 
 */
void mach_load_command_print (void *cmd, int flag)
{
    // flag = 0     -   cmd is mach_load_command_t
    // flag = 1     -   cmd is mach_command_info_t

    mach_load_command_t *lc = mach_load_command_create ();
    if (flag == LC_RAW) {
        lc = (mach_load_command_t *) cmd;
    } else if (flag == LC_INFO) {
        mach_command_info_t *inf = (mach_command_info_t *) cmd;
        lc = (mach_load_command_t *) inf->lc;
    } else {
        g_print ("[*] Error: Unknown Load Command print flag: 0x%x\n", flag);
        return;
    }

    g_print ("     Command:\t%s\n", mach_load_command_get_string (lc));
    g_print ("Command Size:\t%d\n", lc->cmdsize);
}


/**
 *  Function:   mach_load_command_get_string
 *  ----------------------------------------
 * 
 *  Returns a string representation of the Load Command Type, for example
 *  LC_SOURCE_VERSION.
 * 
 *  lc:         The Load Command to translate to a string
 * 
 *  returns:    The string representation of the Load Command.
 * 
 */
char *mach_load_command_get_string (mach_load_command_t *lc)
{
    if (!lc->cmd) {
        g_print ("[*] Error: lc->cmd not valid\n");
        exit (0);
    }
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
        case LC_ROUTINES:
            cmd_str = "LC_ROUTINES";
            break;
        case LC_SUB_FRAMEWORK:
            cmd_str = "LC_SUB_FRAMEWORK";
            break;
        case LC_SUB_UMBRELLA:
            cmd_str = "LC_SUB_UMBRELLA";
            break;
        case LC_SUB_CLIENT:
            cmd_str = "LC_SUB_CLIENT";
            break;
        case LC_SUB_LIBRARY:
            cmd_str = "LC_SUB_LIBRARY";
            break;
        case LC_TWOLEVEL_HINTS:
            cmd_str = "LC_TWOLEVEL_HINTS";
            break;
        case LC_PREBIND_CKSUM:
            cmd_str = "LC_PREBIND_CKSUM";
            break;
        case LC_LOAD_WEAK_DYLIB:
            cmd_str = "LC_LOAD_WEAK_DYLIB";
            break;
        case LC_SEGMENT_64:
            cmd_str = "LC_SEGMENT_64";
            break;
        case LC_ROUTINES_64:
            cmd_str = "LC_ROUTINES_64";
            break;
        case LC_UUID:
            cmd_str = "LC_UUID";
            break;
        case LC_RPATH:
            cmd_str = "LC_RPATH";
            break;
        case LC_CODE_SIGNATURE:
            cmd_str = "LC_CODE_SIGNATURE";
            break;
        case LC_SEGMENT_SPLIT_INFO:
            cmd_str = "LC_SEGMENT_SPLIT_INFO";
            break;
        case LC_REEXPORT_DYLIB:
            cmd_str = "LC_REEXPORT_DYLIB";
            break;
        case LC_LAZY_LOAD_DYLIB:
            cmd_str = "LC_LAZY_LOAD_DYLIB";
            break;
        case LC_ENCRYPTION_INFO:
            cmd_str = "LC_ENCRYPTION_INFO";
            break;
        case LC_DYLD_INFO:
            cmd_str = "LC_DYLD_INFO";
            break;
        case LC_DYLD_INFO_ONLY:
            cmd_str = "LC_DYLD_INFO_ONLY";
            break;
        case LC_LOAD_UPWARD_DYLIB:
            cmd_str = "LC_LOAD_UPWARD_DYLIB";
            break;
        case LC_VERSION_MIN_MACOSX:
            cmd_str = "LC_VERSION_MIN_MACOSX";
            break;
        case LC_VERSION_MIN_IPHONEOS:
            cmd_str = "LC_VERSION_MIN_IPHONEOS";
            break;
        case LC_FUNCTION_STARTS:
            cmd_str = "LC_FUNCTION_STARTS";
            break;
        case LC_DYLD_ENVIRONMENT:
            cmd_str = "LC_DYLD_ENVIRONMENT";
            break;
        case LC_MAIN:
            cmd_str = "LC_MAIN";
            break;
        case LC_DATA_IN_CODE:
            cmd_str = "LC_DATA_IN_CODE";
            break;
        case LC_SOURCE_VERSION:
            cmd_str = "LC_SOURCE_VERSION";
            break;
        case LC_DYLIB_CODE_SIGN_DRS:
            cmd_str = "LC_DYLIB_CODE_SIGN_DRS";
            break;
        case LC_ENCRYPTION_INFO_64:
            cmd_str = "LC_ENCRYPTION_INFO_64";
            break;
        case LC_LINKER_OPTION:
            cmd_str = "LC_LINKER_OPTION";
            break;
        case LC_LINKER_OPTIMIZATION_HINT:
            cmd_str = "LC_LINKER_OPTIMIZATION_HINT";
            break;
        case LC_VERSION_MIN_TVOS:
            cmd_str = "LC_VERSION_MIN_TVOS";
            break;
        case LC_VERSION_MIN_WATCHOS:
            cmd_str = "LC_VERSION_MIN_WATCHOS";
            break;
        case LC_NOTE:
            cmd_str = "LC_NOTE";
            break;
        case LC_BUILD_VERSION:
            cmd_str = "LC_BUILD_VERSION";
            break;
        case LC_DYLD_EXPORTS_TRIE:
            cmd_str = "LC_DYLD_EXPORTS_TRIE";
            break;
        case LC_DYLD_CHAINED_FIXUPS:
            cmd_str = "LC_DYLD_CHAINED_FIXUPS";
            break;
        default:
            cmd_str = "LC_UNKNOWN";
            break;
    }
    return cmd_str;
}


//////////////////////////////////////////////////////////////////////////
//                     Specific Load Commands                           //
//////////////////////////////////////////////////////////////////////////

/**
 *  Function:   mach_lc_find_source_version_cmd
 *  ------------------------------------
 * 
 *  Finds and creates a mach_source_version_command_t struct from a given macho
 *  by looping through each of the load commands in the macho->lcmds GSList. Load
 *  Command like LC_SOURCE_VERSION only occur once in each Mach-O file, so there
 *  is relatively no danger of loading the wrong one.
 * 
 *  macho:      The Mach-O file containing an LC_SOURCE_VERSION command.
 * 
 *  returns:    A mach_source_version_command_t structure with sufficient allocated memory.
 * 
 */
mach_source_version_command_t *mach_lc_find_source_version_cmd (macho_t *macho)
{
    size_t size = sizeof (mach_source_version_command_t);
    mach_source_version_command_t *ret = malloc (size);

    GSList *cmds = macho->lcmds;
    for (int i = 0; i < g_slist_length (cmds); i++) {
        mach_command_info_t *tmp = (mach_command_info_t *) g_slist_nth_data (cmds, i);
        if (tmp->type == LC_SOURCE_VERSION) {
            ret = (mach_source_version_command_t *) file_load_bytes (macho->file, size, tmp->off);
            
            if (!ret) {
                g_print ("[*] Error: Failed to load LC_SOURCE_VERSION command from offset: 0x%llx\n");
                return NULL;
            } else {
                return ret;
            }
        }
    }

    return NULL;
}


/**
 *  Function:   mach_lc_source_version_string
 * 
 *  Takes a LC_SOURCE_VERSION command and unpacks the version string from the
 *  uint64_t into a readable string.
 * 
 *  svc:        The LC_SOURCE_VERSION Command.
 * 
 *  returns:    The unpacked version string.
 *      
 */
char *mach_lc_source_version_string (mach_source_version_command_t *svc)
{
    char *ret = malloc(20);
    uint64_t a, b, c, d, e;

    if (svc->cmdsize != sizeof(mach_source_version_command_t)) {
        g_print ("Incorrect size\n");
    }

    a = (svc->version >> 40) & 0xffffff;
    b = (svc->version >> 30) & 0x3ff;
    c = (svc->version >> 20) & 0x3ff;
    d = (svc->version >> 10) & 0x3ff;
    e = svc->version & 0x3ff;

    if (e != 0) {
        snprintf (ret, 20, "%llu.%llu.%llu.%llu.%llu", a, b, c, d, e);
    } else if (d != 0) {
        snprintf (ret, 16, "%llu.%llu.%llu.%llu", a, b, c, d);
    } else if (c != 0) {
        snprintf (ret, 12, "%llu.%llu.%llu", a, b, c);
    } else {
        snprintf (ret, 8, "%llu.%llu", a, b);
    }

    return ret;
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


/**
 *  Function:   mach_lc_find_uuid_cmd
 *  ---------------------------------
 * 
 *  Finds and creates a mach_uuid_command_t struct from a given macho
 *  by looping through each of the load commands in the macho->lcmds GSList. Load
 *  Commands like LC_UUID only occur once in each Mach-O file, so there is relatively 
 *  no danger of loading the wrong one.
 * 
 *  macho:      The Mach-O file containing an LC_UUID command.
 * 
 *  returns:    A mach_uuid_command_t structure with sufficient allocated memory.
 *      
 */
mach_uuid_command_t *mach_lc_find_uuid_cmd (macho_t *macho)
{
    size_t size = sizeof (mach_uuid_command_t);
    mach_uuid_command_t *ret = malloc (size);

    GSList *cmds = macho->lcmds;
    for (int i = 0; i < g_slist_length (cmds); i++) {
        mach_command_info_t *tmp = (mach_command_info_t *) g_slist_nth_data (cmds, i);
        if (tmp->type == LC_UUID) {
            ret = (mach_uuid_command_t *) file_load_bytes (macho->file, size, tmp->off);
            
            if (!ret) {
                g_print ("[*] Error: Failed to load LC_UUID command from offset: 0x%llx\n");
                return NULL;
            } else {
                return ret;
            }
        }
    }

    return NULL;
}



/**
 *  Function:   mach_lc_uuid_string
 * 
 *  Takes a LC_UUID command and unpacks the uuid string from the uint8_t into a 
 *  readable string.
 * 
 *  svc:        The LC_UUID Command.
 * 
 *  returns:    The unpacked UUID string.
 *      
 */
char *mach_lc_uuid_string (mach_uuid_command_t *uuid)
{
    if (uuid->cmdsize != sizeof(mach_uuid_command_t)) {
        g_print ("Incorrect size\n");
        return NULL;
    }

    size_t size = sizeof(uint8_t) * 128;
    char *ret = malloc (size);
    snprintf (ret, size, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                            (unsigned int)uuid->uuid[0], (unsigned int)uuid->uuid[1],
                            (unsigned int)uuid->uuid[2],  (unsigned int)uuid->uuid[3],
                            (unsigned int)uuid->uuid[4],  (unsigned int)uuid->uuid[5],
                            (unsigned int)uuid->uuid[6],  (unsigned int)uuid->uuid[7],
                            (unsigned int)uuid->uuid[8],  (unsigned int)uuid->uuid[9],
                            (unsigned int)uuid->uuid[10], (unsigned int)uuid->uuid[11],
                            (unsigned int)uuid->uuid[12], (unsigned int)uuid->uuid[13],
                            (unsigned int)uuid->uuid[14], (unsigned int)uuid->uuid[15]);

    return ret;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


/**
 *  The Symtab command can stay here, but handling symbols
 *  should be in symbols.c
 */

mach_command_info_t *mach_lc_find_given_cmd (macho_t *macho, int cmd)
{
    GSList *cmds = macho->lcmds;
    for (int i = 0; i < g_slist_length (cmds); i++) {
        mach_command_info_t *tmp = (mach_load_command_t *) g_slist_nth_data (cmds, i);
        if (tmp->type == cmd) {
            return tmp;
        }
    }
    return NULL;
}

/**
 * 
 */
mach_symtab_command_t *mach_lc_find_symtab_cmd (macho_t *macho)
{
    size_t size = sizeof (mach_symtab_command_t);
    mach_symtab_command_t *ret = malloc (size);
    
    mach_command_info_t *cmdinfo = mach_lc_find_given_cmd (macho, LC_SYMTAB);
    ret = (mach_symtab_command_t *) file_load_bytes (macho->file, size, cmdinfo->off);

    g_print ("LC_SYMTAB: %d\n", LC_SYMTAB);
    g_print ("test symtab: 0x%llx\n", ret->nsyms);

    return ret;
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


/**
 * 
 */
mach_dysymtab_command_t *mach_lc_find_dysymtab_cmd (macho_t *macho)
{
    size_t size = sizeof (mach_dysymtab_command_t);
    mach_dysymtab_command_t *ret = malloc (size);
    
    mach_command_info_t *cmdinfo = mach_lc_find_given_cmd (macho, LC_SYMTAB);
    ret = (mach_dysymtab_command_t *) file_load_bytes (macho->file, size, cmdinfo->off);

    g_print ("LC_DYSYMTAB: %d\n", LC_DYSYMTAB);
    g_print ("test symtab: 0x%llx\n", ret->cmdsize);

    return ret;
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

char *mach_lc_load_dylib_format_version (uint32_t vers)
{
    char *buf = malloc(10);
    snprintf (buf, 10, "%d.%d.%d", vers >> 16, (vers >> 8) & 0xf, vers & 0xf);
    return buf;
}

char *mach_lc_dylib_get_type_string (mach_dylib_command_t *dylib)
{
    switch (dylib->cmd) {
    case LC_ID_DYLIB:
        return "LC_ID_DYLIB";
        break;
    case LC_LOAD_DYLIB:
        return "LC_LOAD_DYLIB";
        break;
    case LC_LOAD_WEAK_DYLIB:
        return "LC_LOAD_WEAK_DYLIB";
        break;
    case LC_REEXPORT_DYLIB:
        return "LC_REEXPORT_DYLIB";
        break;
    default:
        return "(null)";
        break;
    }
}