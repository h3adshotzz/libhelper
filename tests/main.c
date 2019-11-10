#include <libhelper.h>
#include <file.h>
#include <macho.h>

#include <glib-2.0/glib.h>

int main (int argc, char* argv[])
{
    g_print ("libhelper-beta: %s\n", libhelper_version_string());

    file_t *f = file_load (argv[1]);

    macho_t *macho = macho_load (f);
    mach_header_dump_test (macho->header);

    GSList *segment_names = mach_segment_get_list (macho);
    for (int i = 0; i < g_slist_length (segment_names); i++) {
        g_print ("%s, ", g_slist_nth_data (segment_names, i));
    }
    g_print ("\n\n");

    mach_segment_command_dump (mach_segment_command_search (macho, argv[2]));

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
