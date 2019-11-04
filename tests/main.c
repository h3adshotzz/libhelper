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

    mach_header_t *mach_header = mach_header_load (buf);
    mach_header_dump_test (mach_header);

    size_t offset = sizeof (mach_header_t);

    for (int i = 0; i < mach_header->ncmds; i++) {

        offset += i * sizeof (mach_load_command_t);

        mach_load_command_t *lc = malloc (sizeof(mach_load_command_t));
        memcpy (&lc, &buf[offset], sizeof(mach_load_command_t));

        g_print ("LC Type: %d\n", lc->cmd);
        g_print ("LC Size: %d\n\n", lc->cmdsize);


        /**
         *  LC_SEGMENT_64 == 0x19
         */
        if (lc->cmd == 0x19) {


        } else {



        }
    }

    /*

    typedef struct mach_command_t {
	uint32_t	cmd;
	uint32_t	cmdsize;
	char		segname[16];
	uint32_t	vmaddr;
	uint32_t	vmsize;
	uint32_t	fileoff;
	uint32_t	filesize;
	uint32_t	maxprot;
	uint32_t	initprot;
	uint32_t	nsects;
	uint32_t	flags;
} mach_command_t;*/

    return 0;
}