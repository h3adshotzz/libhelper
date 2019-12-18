#include <libhelper/libhelper.h>
#include <libhelper-macho/macho.h>
#include <libhelper-macho/macho-header.h>
#include <libhelper-macho/macho-command.h>
#include <libhelper-macho/macho-segment.h>
#include <libhelper-macho/macho-symbol.h>
#include <libhelper-macho/macho-thread_state.h>

int main (int argc, char* argv[])
{
    printf ("libhelper-beta: %s\n", libhelper_version_string());

    if (argc < 2) {
        printf ("[*] Error: libhelper-test requries args\n");
        exit (0);
    }

    file_t *f = file_load (argv[1]);

    
    /**
     *  Test's the macho_t functionality
     */
    macho_t *macho = macho_load (f);
    mach_header_print_summary (macho->header);

    if ((macho->header->flags & MH_PIE) == MH_PIE) {
        printf ("MH_PIE\n");
    } else {
        printf ("mh_dunno\n");
    }


    // LOAD COMMAND TESTING
    /*for (int i = 0; i < g_slist_length (macho->lcmds); i++) {
        mach_command_info_t *info = g_slist_nth_data (macho->lcmds, i);

        if ((info->type == LC_DYLD_INFO_ONLY) || (info->type == LC_DYLD_INFO)) {
            mach_dyld_info_command_t *cmd = (mach_dyld_info_command_t *) file_load_bytes (macho->file, info->lc->cmdsize, info->off);

            if (cmd->cmd == LC_DYLD_INFO_ONLY) g_print ("== Command:\tLC_DYLD_INFO_ONLY\n");
            if (cmd->cmd == LC_DYLD_INFO) g_print ("== Command:\tLC_DYLD_INFO\n");

            g_print ("  Command Size:\t\t%d\n", cmd->cmdsize);
            
            g_print ("  Rebase Offset:\t0x%08x\n", cmd->rebase_off);
            g_print ("  Rebase Size:\t\t%d\n", cmd->rebase_size);

            g_print ("  Bind Offset:\t\t0x%08x\n", cmd->bind_off);
            g_print ("  Bind Size:\t\t%d\n", cmd->bind_size);

            g_print ("  Weak Bind Offset:\t0x%08x\n", cmd->weak_bind_off);
            g_print ("  Weak Bind Size:\t%d\n", cmd->weak_bind_size);

            g_print ("  Lazy Bind Offset:\t0x%08x\n", cmd->lazy_bind_off);
            g_print ("  Lazy Bind Size:\t%d\n", cmd->lazy_bind_size);

            g_print ("  Export Offset:\t0x%08x\n", cmd->export_off);
            g_print ("  Export Size:\t\t%d\n", cmd->export_size);

        } else if (info->type == LC_SYMTAB) {
            mach_symtab_command_t *cmd = (mach_symtab_command_t *) file_load_bytes (macho->file, info->lc->cmdsize, info->off);

            g_print ("== Command:\tLC_SYMTAB\n");

            g_print ("  Symbol Offset:\t0x%08x\n", cmd->symoff);
            g_print ("  Symbol Count:\t\t%d\n", cmd->nsyms);

            g_print ("  String Offset:\t0x%08x\n", cmd->stroff);
            g_print ("  String Count:\t\t%d\n", cmd->strsize);
        
        }

        g_print ("---------------\n");
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


    mach_dysymtab_command_t *dysymtab_table = mach_lc_find_dysymtab_cmd (macho);
    g_print ("== Dynamic Symbol Table ==\n");
    g_print ("           cmd: 0x%x\n", dysymtab_table->cmd);
    g_print ("       cmdsize: 0x%x\n", dysymtab_table->cmdsize);
    g_print ("     ilocalsym: 0x%x\n", dysymtab_table->ilocalsym);
    g_print ("     nlocalsym: 0x%x\n", dysymtab_table->nlocalsym);
    g_print ("    iextdefsym: 0x%x\n", dysymtab_table->iextdefsym);
    g_print ("    nextdefsym: 0x%x\n", dysymtab_table->nextdefsym);
    g_print ("     iundefsym: 0x%x\n", dysymtab_table->iundefsym);
    g_print ("     nundefsym: 0x%x\n", dysymtab_table->nundefsym);
    g_print ("        tocoff: 0x%x\n", dysymtab_table->tocoff);
    g_print ("          ntoc: 0x%x\n", dysymtab_table->ntoc);
    g_print ("     modtaboff: 0x%x\n", dysymtab_table->modtaboff);
    g_print ("       nmodtab: 0x%x\n", dysymtab_table->nmodtab);
    g_print ("  extrefsymoff: 0x%x\n", dysymtab_table->extrefsymoff);
    g_print ("   nextrefsyms: 0x%x\n", dysymtab_table->nextdefsym);
    g_print ("indirectsymoff: 0x%x\n", dysymtab_table->indirectsymoff);
    g_print (" nindirectsyms: 0x%x\n", dysymtab_table->nindirectsyms);
    g_print ("     extreloff: 0x%x\n", dysymtab_table->extrefsymoff);
    g_print ("       nextrel: 0x%x\n", dysymtab_table->nextrel);
    g_print ("     locreloff: 0x%x\n", dysymtab_table->locreloff);
    g_print ("       nlocrel: 0x%x\n", dysymtab_table->nlocrel);

    //mach_dynamic_symbol_table_t *dysymtab_test = mach_dynamic_symtab_load_symbols (macho, dysymtab_table);
    */

    mach_command_info_t *cmd = mach_lc_find_given_cmd (macho, LC_UNIXTHREAD);
    mach_load_command_info_print (cmd);

    mach_load_command_t *lc = cmd->lc;
    off_t offset = cmd->off + sizeof(lc);
    arm_thread_state64_t *cpu = (arm_thread_state64_t *) file_load_bytes (macho->file, lc->cmdsize, offset);

    printf (
		       "\t    x0  0x%016llx x1  0x%016llx x2  0x%016llx\n"
		       "\t    x3  0x%016llx x4  0x%016llx x5  0x%016llx\n"
		       "\t    x6  0x%016llx x7  0x%016llx x8  0x%016llx\n"
		       "\t    x9  0x%016llx x10 0x%016llx x11 0x%016llx\n"
		       "\t    x12 0x%016llx x13 0x%016llx x14 0x%016llx\n"
		       "\t    x15 0x%016llx x16 0x%016llx x17 0x%016llx\n"
		       "\t    x18 0x%016llx x19 0x%016llx x20 0x%016llx\n"
		       "\t    x21 0x%016llx x22 0x%016llx x23 0x%016llx\n"
		       "\t    x24 0x%016llx x25 0x%016llx x26 0x%016llx\n"
		       "\t    x27 0x%016llx x28 0x%016llx  fp 0x%016llx\n"
		       "\t     lr 0x%016llx sp  0x%016llx  pc 0x%016llx\n"
		       "\t   cpsr 0x%08x\n",
			cpu->__x[0], cpu->__x[1], cpu->__x[2], cpu->__x[3],
			cpu->__x[4], cpu->__x[5], cpu->__x[6], cpu->__x[7],
			cpu->__x[8], cpu->__x[9], cpu->__x[10], cpu->__x[11],
			cpu->__x[12], cpu->__x[13], cpu->__x[14], cpu->__x[15],
			cpu->__x[16], cpu->__x[17], cpu->__x[18], cpu->__x[19],
			cpu->__x[20], cpu->__x[21], cpu->__x[22], cpu->__x[23],
			cpu->__x[24], cpu->__x[25], cpu->__x[26], cpu->__x[27],
			cpu->__x[28], cpu->__fp, cpu->__lr, cpu->__sp, cpu->__pc,
			cpu->__cpsr);


    return 0;
}