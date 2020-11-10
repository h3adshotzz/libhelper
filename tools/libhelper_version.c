#include <stdio.h>
#include <libhelper/libhelper.h>


int main ()
{
    printf ("%s Libhelper Version %s~%s (%s)\n", BUILD_TARGET_CAP, LIBHELPER_VERSION, LIBHELPER_VERSION_TYPE,
							LIBHELPER_VERSION_LONG);
    
    printf ("  Build Time:\t\t" __TIMESTAMP__ "\n");
    printf ("  Default Target:\t%s-%s\n", BUILD_TARGET, BUILD_ARCH);
    printf ("  Libhelper:\t\t%s\n", LIBHELPER_VERSION_LONG);

    return 0;
}
