#include <libhelper.h>
#include <libhelper-macho.h>
#include <libhelper-hlibc.h>
#include <libhelper-file.h>

int main (int argc, char *argv[])
{
    char *filename = (char *) argv[1];
    macho_t *macho = macho_64_load (filename);

    printf ("CPU: %s (%s)\n",
            mach_header_get_cpu_string (macho->header->cputype,
                                        macho->header->cpusubtype),
            mach_header_get_cpu_descriptor (macho->header->cputype,
                                            macho->header->cpusubtype));

    printf ("cputype: 0x%08x, cpusubtype: 0x%08x\n", macho->header->cputype,
            macho->header->cpusubtype);

    return 0;
}
