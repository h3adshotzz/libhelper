#include <libhelper.h>
#include <file.h>
#include <macho.h>
#include <ctype.h>

#include <glib-2.0/glib.h>

int main (int argc, char* argv[])
{
    g_print ("libhelper-beta: %s\n", libhelper_version_string());

    if (argc < 2) {
        g_print ("[*] Error: libhelper-test requries args\n");
        exit (0);
    }

    file_t *f = file_load (argv[1]);

    macho_t *macho = macho_load (f);
    mach_header_dump_test (macho->header);

    /**
     *  Other load commands
     *
    GSList *lc_names = mach_load_command_get_list (macho);
    for (int i = 0; i < g_slist_length (lc_names); i++) {
        mach_load_command_t *lc = g_slist_nth_data (lc_names, i);
        g_print("%s, ", mach_load_command_get_string (lc));
    }
    g_print ("\n\n");*/


    g_print ("\n");
    GSList *commands = macho->lcmds;
    for (int i = 0; i < (int) g_slist_length (commands); i++) {
        mach_load_command_t *cmd = (mach_load_command_t *) g_slist_nth_data (commands, i);
        if (cmd->cmd == LC_SOURCE_VERSION) {
            mach_source_version_command_t *sv = malloc(sizeof(mach_source_version_command_t));
            sv = (mach_source_version_command_t *) cmd;
            g_print ("Command:\tLC_SOURCE_VERSION\n");
            g_print ("Size:\t\t%d\n", sv->cmdsize);

            char *ver = malloc(sv->cmdsize);
            memcpy(ver, sv[sizeof(uint32_t) * 2], sv->cmdsize);

            g_print ("Version:\t%s\n\n", (char *) ver);
        }
    }


    /**
     *  Segment
     */
    GSList *segment_names = mach_segment_get_list (macho);
    for (int i = 0; i < (int) g_slist_length (segment_names); i++) {
        g_print ("%s, ", g_slist_nth_data (segment_names, i));
    }
    g_print ("\n\n");

    /**
     *  Segment sections
     */
    mach_segment_info_t *segment = mach_segment_command_search(macho, argv[2]);
    mach_segment_command_dump (segment);

    g_print ("len: %d\n", g_slist_length (segment->sections));
    for (int i = 0; i < (int) g_slist_length(segment->sections); i++) {
        mach_section_64_t *sect = g_slist_nth_data (segment->sections, i);
        mach_section_dump (sect);
    }

    // test decompilation
    /*mach_section_64_t *sect = g_slist_nth_data (segment->sections, 0);
    
    void *mem = file_load_bytes (macho->file, sizeof(mach_section_64_t), segment->segcmd->vmaddr);

    int len = sizeof(mach_section_64_t);
    int cols = 16;

    unsigned int i, j;
    for (int i = 0; i < len + ((len % cols) ? (cols - len % cols) : 0); i++) {
        if (i % cols == 0) g_print ("0x%06x: ", i);
        if (i < len) g_print ("%02x ", 0xFF & ((char*)mem)[i]);
        else g_print (" ");

        if (i % cols == (cols - 1)) {
            for (j = i - (cols - 1); j <= 1; j++) {
                if (j <= len) putchar(' ');
                else if (isprint(((char*)mem)[j])) putchar(0xFF & ((char*)mem)[j]);
                else putchar('.');
            }
            putchar('\n');
        }
    }*/


    /**
    GSList *sections = mach_sections_load_from_segment (macho, segment);
    for (int i = 0; i < segment->nsects; i++) {
        mach_section_64_t *sect = g_slist_nth_data (sections, i);
        g_print ("section: %s, offset: 0x%s\n", sect->sectname, sect->offset);

    }*/

    /*uint32_t offset = segment->vmaddr;
    for (int i = 0; i < segment->nsects; i++) {
        g_print ("Loading %d bytes from 0x%x\n", sizeof(mach_section_64_t), offset);
        mach_section_64_t *section = (mach_section_64_t *) file_load_bytes (macho->file, sizeof(mach_section_64_t), offset);
        g_print ("section: %s, size: %d bytes, offset: 0x%x\n", section->sectname, section->size, section->offset);
        offset += sizeof(mach_section_64_t);
        free(section);
    }*/

    /*uint32_t offset = segment->vmaddr;
    for (int i = 0; i < segment->nsects; i++) {

        g_print ("Loading %d bytes from 0x%x\n", sizeof(mach_section_64_t), offset);
        mach_section_64_t *sect = mach_section_create ();
        sect = (mach_section_64_t *) file_load_bytes (f, sizeof(mach_section_64_t), offset);

        g_print ("section: %s, size: %d bytes, offset: 0x%x\n", sect->sectname, sect->size, sect->offset);
        offset += sizeof(mach_section_64_t);

        g_print ("\n\n");
    }*/

    file_close (f);

    /* 
    
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

*/

    return 0;
}
