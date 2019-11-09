#include <libhelper.h>
#include <file.h>
#include <macho.h>

#include <glib-2.0/glib.h>

int main (int argc, char* argv[])
{
    g_print ("libhelper-beta: %s\n", libhelper_version_string());

    /* Test the macho functionality */
    file_t *macho = file_load (argv[1]);
    
    /* macho.c testing */
    mach_header_t *mach_header = mach_header_load (file_load_bytes(macho, 32, 0));
    mach_header_dump_test (mach_header);

    off_t offset = sizeof(mach_header_t);
    for (int i = 0; i < mach_header->ncmds; i++) {
        mach_load_command_t *lc = mach_load_command_load (macho, offset);
        mach_load_command_dump (lc);

        offset += lc->cmdsize;
    }


    /*size_t offset = sizeof (mach_header_t);
    size_t cmd_size = sizeof (mach_load_command_t);

    g_print ("\n\n");

    int ncmds = mach_header->ncmds;
    for (int i = 0; i < ncmds; i++) {

        mach_load_command_t *lc = (mach_load_command_t *) file_load_bytes (macho, sizeof(cmd_size), offset);


        **
         *  LC_SEGMENT_64 == 0x19
         *
        if (lc->cmd == LC_SEGMENT_64) {

            mach_segment_command_64_t *segcmd = (mach_segment_command_64_t *) file_load_bytes (macho, sizeof(mach_segment_command_64_t), offset);

            g_print ("=== LC_SEGMENT_64 (%s) ===\n", segcmd->segname);
            g_print ("Command:\t\tLC_SEGMENT_64\n");
            g_print ("Command Size:\t\t%d\n", segcmd->cmdsize);

            g_print ("Segment Name:\t\t%s\n", segcmd->segname);
            g_print ("VM Address:\t\t0x%x\n", segcmd->vmaddr);
            g_print ("VM Size:\t\t%d\n", segcmd->vmsize);
            g_print ("File Offset:\t\t0x%x\n", segcmd->fileoff);
            g_print ("File Size:\t\t%d\n", segcmd->filesize);
            g_print ("Max VM Protection:\t%d\n", segcmd->maxprot);
            g_print ("Initial VM Protection:\t%d\n", segcmd->initprot);
            g_print ("Number of Sections:\t%d\n", segcmd->nsects);
            g_print ("Flags:\t\t\t\t%d\n\n", segcmd->flags);
            
            g_print ("Section headers:\n");
            off_t segoffset = segcmd->vmaddr;
            for (int k = 0; k < segcmd->nsects; k++) {
                section_64_t *sect = (section_64_t *) file_load_bytes (macho, sizeof(section_64_t), segoffset);

                g_print ("\tSection64 Header (%s, 0x%x)\n", sect->sectname, sect->offset);

                segoffset += sizeof (section_64_t);
            }
            if (segoffset == segcmd->vmaddr) {
                g_print ("\tNo section header data\n");
            }

        } else if (lc->cmd == LC_SEGMENT) {

            g_print ("LC_SEGEMNT\n");
 
            mach_segment_command_t *segcmd = file_load_bytes (macho, lc->cmdsize, offset);

            g_print ("name: %s\n", segcmd->segname);

        } else {
            g_print ("LC Type: 0x%x\n", lc->cmd);
            g_print ("LC Size: %d\n\n", lc->cmdsize);
        }

        offset += lc->cmdsize;

        g_print ("\n-----------------------------\n\n");
    }
*/
    fclose(macho->desc);


    return 0;
}
