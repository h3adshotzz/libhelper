#include <libhelper.h>
#include <file.h>
#include <macho.h>

#include <glib-2.0/glib.h>

int main (int argc, char* argv[])
{
    g_print ("tesT: %s\n", libhelper_version_string());


    g_print ("testing libhelper file handling...\n");

    /* Args */
    g_print ("args\n");
    for (int i = 0; i < argc; i++) {
        g_print ("[%d]: %s\n", i, argv[i]);
    }

    /* Test the macho functionality */
    file_t *macho = file_load (argv[1]);
    
    g_print ("macho->path: %s\nmacho->size: %d\n", macho->path, macho->size);

    mach_header_t *mach_header = mach_header_load (file_load_bytes(macho, 32, 0));
    mach_header_dump_test (mach_header);

    size_t offset = sizeof (mach_header_t);
    size_t cmd_size = sizeof (mach_load_command_t);

    g_print ("\n\n");

    for (int i = 0; i < mach_header->ncmds; i++) {

        mach_load_command_t *lc = (mach_load_command_t *) file_load_bytes (macho, sizeof(cmd_size), offset);


        /**
         *  LC_SEGMENT_64 == 0x19
         */
        if (lc->cmd == 0x19) {
            g_print ("LC_SEGEMNT_64\n");

            mach_segment_command_64_t *segcmd = file_load_bytes (macho, lc->cmdsize, offset);

            // mach_segment_dump ()
            g_print ("name: %s\n", segcmd->segname);
            g_print ("cmd: 0x%x\n", segcmd->cmd);
            g_print ("name: %s\n", segcmd->cmdsize);
            g_print ("name: %s\n", segcmd->segname);
            g_print ("name: %s\n", segcmd->segname);
            g_print ("name: %s\n", segcmd->segname);
            g_print ("name: %s\n", segcmd->segname);
            g_print ("name: %s\n", segcmd->segname);
            g_print ("name: %s\n", segcmd->segname);
            g_print ("name: %s\n", segcmd->segname);
            g_print ("name: %s\n", segcmd->segname);
            g_print ("name: %s\n", segcmd->segname);


        } else if (lc->cmd == 0x1) {

            g_print ("LC_SEGEMNT\n");

            mach_segment_command_t *segcmd = file_load_bytes (macho, lc->cmdsize, offset);

            g_print ("name: %s\n", segcmd->segname);

        } else {
            g_print ("LC Type: 0x%x\n", lc->cmd);
            g_print ("LC Size: %d\n\n", lc->cmdsize);
        }

        offset += lc->cmdsize;

        g_print ("\n");
    }

    fclose(macho->desc);


    return 0;
}
