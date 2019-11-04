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
    unsigned char *buf = NULL;
    unsigned int size = 0;
    int err = file_read (argv[1], &buf, &size);

    //g_print ("magic: 0x%x\n", macho_read_magic(buf, 0));

    mach_header_dump_test(buf, 0);

    return 0;
}