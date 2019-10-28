#include <libhelper.h>
#include <file.h>

#include <glib-2.0/glib.h>

int main (int argc, char* argv[])
{
    g_print ("tesT: %s\n", libhelper_version_string());


    g_print ("testing libhelper file handling...\n");

    /* Load the file in argv[1]; */
    file_t *file = file_create ();
    file = file_open (argv[1]);

    unsigned char *data = NULL;
    size_t size = 0;
    file_read (file, &data, &size);

    g_print ("file size: %d bytes\n", size);

    return 0;
}