#include <libhelper.h>
#include <file.h>

#include <mach-o/loader.h>

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

    /* Load the file in argv[1]; */
    file_t *file = file_create ();
    file = file_open (argv[1]);



    return 0;
}