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

    /**
     *  Segment
     */
    GSList *segment_names = mach_segment_get_list (macho);
    for (int i = 0; i < g_slist_length (segment_names); i++) {
        g_print ("%s, ", g_slist_nth_data (segment_names, i));
    }
    g_print ("\n\n");

    if (argv[2]) {
        mach_segment_command_dump (mach_segment_command_search (macho, argv[2]));
    }

    /**
     *  Other load commands
     *
    GSList *lc_names = mach_load_command_get_list (macho);
    for (int i = 0; i < g_slist_length (lc_names); i++) {
        mach_load_command_t *lc = g_slist_nth_data (lc_names, i);
        g_print("%s, ", mach_load_command_get_string (lc));
    }
    g_print ("\n\n");*/


    /**
     *  Segment sections
     */
    mach_segment_command_64_t *segment = mach_segment_command_search(macho, g_slist_nth_data (segment_names, 1));
    mach_segment_command_dump (segment);

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

    uint32_t offset = 0x778c000;
    for (int i = 0; i < segment->nsects; i++) {

        g_print ("Loading %d bytes from 0x%x\n", sizeof(mach_section_64_t), offset);
        mach_section_64_t *sect = (mach_section_64_t *) file_load_bytes (macho->file, sizeof(mach_section_64_t), offset);

        g_print ("section: %s, size: %d bytes, offset: 0x%x\n", sect->sectname, sect->size, sect->offset);
        offset += sizeof(mach_section_64_t);

        g_print ("\n\n");
    }

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
