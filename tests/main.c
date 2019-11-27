#include <libhelper.h>
#include <macho/macho-header.h>
#include <macho/macho-command.h>

int main (int argc, char* argv[])
{
    g_print ("libhelper-beta: %s\n", libhelper_version_string());

    if (argc < 2) {
        g_print ("[*] Error: libhelper-test requries args\n");
        exit (0);
    }

    file_t *f = file_load (argv[1]);

    mach_header_t *header = mach_header_create ();
    header = mach_header_load (f);

    mach_header_print_summary (header);

    if ((header->flags & MH_PIE) == MH_PIE) {
        g_print ("MH_PIE\n");
    } else {
        g_print ("mh_dunno\n");
    }


    // LOAD COMMAND TESTING
    off_t offset = MACH_HEADER_SIZE;
    for (int i = 0; i < (int) header->ncmds; i++) {
        mach_command_info_t *lc = mach_command_info_load (f, offset);
        mach_load_command_info_print (lc);

        offset += lc->lc->cmdsize;
    }

    return 0;
}
