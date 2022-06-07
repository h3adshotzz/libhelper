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
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#include "libhelper-logger.h"
#include "libhelper-macho.h"

/**
 *  Load Command translation table.
 */
struct __libhelper_lc_string {
    uint32_t         lc;        /* Load command ID */
    char            *str;       /* Load command name */;
};

static struct __libhelper_lc_string lc_list[] =
{   
    { LC_SEGMENT,                       "LC_SEGMENT"                    },
    { LC_SYMTAB,                        "LC_SYMTAB"                     },
    { LC_SYMSEG,                        "LC_SYMSEG"                     },

    { LC_THREAD,                        "LC_THREAD"                     },
    { LC_UNIXTHREAD,                    "LC_UNIXTHREAD"                 },
    { LC_LOADFVMLIB,                    "LC_LOADFVMLIB"                 },
    { LC_IDFVMLIB,                      "LC_IDFVMLIB"                   },
    { LC_IDENT,                         "LC_IDENT"                      },
    { LC_FVMFILE,                       "LC_FVMFILE"                    },
    { LC_PREPAGE,                       "LC_PREPAGE"                    },
    { LC_DYSYMTAB,                      "LC_DYSYMTAB"                   },
    { LC_LOAD_DYLIB,                    "LC_LOAD_DYLIB"                 },
    { LC_ID_DYLIB,                      "LC_ID_DYLIB"                   },
    { LC_LOAD_DYLINKER,                 "LC_LOAD_DYLINKER"              },
    { LC_ID_DYLINKER,                   "LC_ID_DYLINKER"                },
    { LC_PREBOUND_DYLIB,                "LC_PREBOUND_DYLIB"             },
    { LC_ROUTINES,                      "LC_ROUTINES"                   },
    { LC_SUB_FRAMEWORK,                 "LC_SUB_FRAMEWORK"              },
    { LC_SUB_UMBRELLA,                  "LC_SUB_UMBRELLA"               },
    { LC_SUB_CLIENT,                    "LC_SUB_CLIENT"                 },
    { LC_SUB_LIBRARY,                   "LC_SUB_LIBRARY"                },
    { LC_TWOLEVEL_HINTS,                "LC_TWOLEVEL_HINTS"             },
    { LC_PREBIND_CKSUM,                 "LC_PREBIND_CKSUM"              },

    { LC_LOAD_WEAK_DYLIB,               "LC_LOAD_WEAK_DYLIB"            },

    { LC_SEGMENT_64,                    "LC_SEGMENT_64"                 },
    { LC_ROUTINES_64,                   "LC_ROUTINES_64"                },
    { LC_UUID,                          "LC_UUID"                       },
    { LC_RPATH,                         "LC_RPATH"                      },
    { LC_CODE_SIGNATURE,                "LC_CODE_SIGNATURE"             },
    { LC_SEGMENT_SPLIT_INFO,            "LC_SEGMENT_SPLIT_INFO"         },
    { LC_REEXPORT_DYLIB,                "LC_REEXPORT_DYLIB"             },
    { LC_LAZY_LOAD_DYLIB,               "LC_LAZY_LOAD_DYLIB"            },
    { LC_ENCRYPTION_INFO,               "LC_ENCRYPTION_INFO"            },
    { LC_DYLD_INFO,                     "LC_DYLD_INFO"                  },
    { LC_DYLD_INFO_ONLY,                "LC_DYLD_INFO_ONLY"             },
    { LC_LOAD_UPWARD_DYLIB,             "LC_LOAD_UPWARD_DYLIB"          },
    { LC_VERSION_MIN_MACOSX,            "LC_VERSION_MIN_MACOSX"         },
    { LC_VERSION_MIN_IPHONEOS,          "LC_VERSION_MIN_IPHONEOS"       },
    { LC_FUNCTION_STARTS,               "LC_FUNCTION_STARTS"            },
    { LC_DYLD_ENVIRONMENT,              "LC_DYLD_ENVIRONMENT"           },
    { LC_MAIN,                          "LC_MAIN"                       },
    { LC_DATA_IN_CODE,                  "LC_DATA_IN_CODE"               },
    { LC_SOURCE_VERSION,                "LC_SOURCE_VERSION"             },
    { LC_DYLIB_CODE_SIGN_DRS,           "LC_DYLIB_CODE_SIGN_DRS"        },
    { LC_ENCRYPTION_INFO_64,            "LC_ENCRYPTION_INFO_64"         },
    { LC_LINKER_OPTION,                 "LC_LINKER_OPTION"              },
    { LC_LINKER_OPTIMIZATION_HINT,      "LC_LINKER_OPTIMIZATION_HINT"   },
    { LC_VERSION_MIN_TVOS,              "LC_VERSION_MIN_TVOS"           },
    { LC_VERSION_MIN_WATCHOS,           "LC_VERSION_MIN_WATCHOS"        },
    { LC_NOTE,                          "LC_NOTE"                       },
    { LC_BUILD_VERSION,                 "LC_BUILD_VERSION"              },
    { LC_DYLD_EXPORTS_TRIE,             "LC_DYLD_EXPORTS_TRIE"          },
    { LC_DYLD_CHAINED_FIXUPS,           "LC_DYLD_CHAINED_FIXUPS"        },

    { LC_FILESET_ENTRY,                 "LC_FILESET_ENTRY"              }
};

#define LIBHELPER_LC_LIST_LEN              (sizeof (lc_list) / sizeof (lc_list[0])) - 1


mach_load_command_info_t *
mach_load_command_info_load (const char *data, uint32_t offset)
{
    mach_load_command_info_t *lc_inf = calloc (1, sizeof (mach_load_command_info_t));

    /* Load bytes from data + offset */
    lc_inf->lc = (mach_load_command_t *) (data + offset);
    if (!lc_inf->lc) {
        errorf ("mach_load_command_info_load: Could not create load command info at offset: 0x%08x\n", offset);
        return NULL;
    }

    lc_inf->offset = offset;
    return lc_inf;
}

char *
mach_load_command_get_name (mach_load_command_t *lc)
{
    /* Check that the load command is valid */
    if (!lc->cmd) {
        errorf ("mach_load_command_get_name: Could not get name for load command, LC is not valid.\n");
        goto lc_find_name_fail;
    }

    /* Check the LC name translation table */
    for (int i = 0; i < (int) LIBHELPER_LC_LIST_LEN; i++) {
        struct __libhelper_lc_string *cur = &lc_list[i];
        if (lc->cmd == cur->lc) return cur->str;
    }

lc_find_name_fail:
    return "LC_UNKNOWN";
}

mach_load_command_info_t *
mach_load_command_find_command_by_type (macho_t *macho, uint32_t cmd)
{
    HSList *lcmds = macho->lcmds;
    int c = h_slist_length (lcmds);

    for (int i = 0; i < c; i++) {
        mach_load_command_info_t *tmp = (mach_load_command_info_t *) h_slist_nth_data (macho->lcmds, i);
        if (tmp->lc->cmd == cmd) return tmp;
    }
    return NULL;
}

char *
mach_load_command_load_string (macho_t *macho, uint32_t cmdsize, uint32_t struct_size, 
                               uint32_t cmd_offset, uint32_t str_offset)
{
    return macho_load_bytes (macho, cmdsize - struct_size, cmd_offset + str_offset);
}