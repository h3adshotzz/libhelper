#include <libhelper.h>
#include <file.h>
#include <macho.h>
#include <ctype.h>

#include <glib-2.0/glib.h>

int main (int argc, char *argv[])
{
    file_t *f = file_load (argv[1]);
    macho_t *mach = macho_load (f);
    mach_header_dump_test (mach->header);

    return 0;
}