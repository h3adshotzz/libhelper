#include <libhelper.h>
#include <macho/macho.h>
#include <macho/macho-header.h>
#include <macho/macho-command.h>
#include <macho/macho-segment.h>
#include <macho/macho-symbol.h>

int main (int argc, char* argv[])
{
    g_print ("libhelper-beta: %s\n", libhelper_version_string());

    if (argc < 2) {
        g_print ("[*] Error: libhelper-test requries args\n");
        exit (0);
    }

    file_t *f = file_load (argv[1]);

    
    /**
     *  Test's the macho_t functionality
     */
    macho_t *macho = macho_load (f);
    mach_header_print_summary (macho->header);

    if ((macho->header->flags & MH_PIE) == MH_PIE) {
        g_print ("MH_PIE\n");
    } else {
        g_print ("mh_dunno\n");
    }


    // LOAD COMMAND TESTING
    for (int i = 0; i < g_slist_length (macho->lcmds); i++) {
        mach_load_command_info_print ((mach_command_info_t *) g_slist_nth_data (macho->lcmds, i));
    }

    // Testing printing data from lc_source_version and lc_uuid
    g_print ("\nLC_SOURCE_VERSION: %s\n", mach_lc_source_version_string (mach_lc_find_source_version_cmd (macho)));
    g_print ("\nLC_UUID: %s\n", mach_lc_uuid_string (mach_lc_find_uuid_cmd (macho)));

    // Testing Segments
    for (int i = 0; i < g_slist_length (macho->scmds); i++) {
        mach_segment_info_t *seginfo = (mach_segment_info_t *) g_slist_nth_data (macho->scmds, i);
        mach_segment_command_dump (seginfo);

        g_print ("Sections:\n");
        for (int k = 0; k < g_slist_length (seginfo->sections); k++) {
            mach_section_64_t *sect = g_slist_nth_data (seginfo->sections, k);
            mach_section_print (sect);
        }
        g_print ("------------------------------\n");
    }


    // Testing symbol tables
    mach_symtab_command_t *symbol_table = mach_lc_find_symtab_cmd (macho);
    g_print ("== Symbol Table ==\n");
    g_print ("    cmd: 0x%x\n", symbol_table->cmd);
    g_print ("cmdsize: 0x%x\n", symbol_table->cmdsize);
    g_print (" symoff: 0x%x\n", symbol_table->symoff);
    g_print ("  nsyms: %d\n", symbol_table->nsyms);
    g_print (" stroff: 0x%x\n", symbol_table->stroff);
    g_print ("strsize: %d\n", symbol_table->strsize);

    //mach_symbol_table_t *symtab_test = mach_symtab_load_symbols (macho, symbol_table);


    return 0;
}
