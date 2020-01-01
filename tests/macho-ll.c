//
//	I'm considering writing a lower-level Mach-O parser than what
//	I already have with libhelper-macho. The reason for this is that
//	HTool needs to be able to interact with kernelcaches more like
//	as if they're running.
//
//	The difference between libhelper-macho and libhelper-macho-ll is
//	that instead of using FILE and file_t for all interactions with
//	a Mach-O, I'll mmap the file into memory and handle it from there.
//
//	It's more work, but I think it will be better in the long run.
//	I hope it won't be more than a week's work.
//
//


#include <libhelper/libhelper.h>
#include <libhelper/file.h>

#include <libhelper-macho/macho-header.h>
#include <libhelper-macho/macho-command.h>

int main (int argc, char *argv[])
{
	// take the first arg as the file
	if (argc < 2) return -1;

	char *filename = argv[1];
	file_t *f = file_load (filename);

	// try to load
	uint32_t size = f->size;
	unsigned char *data = (unsigned char *) file_load_bytes (f, size, 0);

	uint32_t offset = 0;

	// load the header
	mach_header_64_t *header = malloc( sizeof(mach_header_64_t ));

	memcpy (header, &data[offset], sizeof(mach_header_64_t));

	mach_header_print_summary (header);



	// load commands
	offset += sizeof (mach_header_64_t );

	for (int i = 0; i < header->ncmds; i++) {
		mach_load_command_t *lc = malloc (sizeof(mach_load_command_t ));

		memcpy (lc, &data[offset], sizeof(mach_load_command_t));
		mach_load_command_print (lc, LC_RAW);

		offset += lc->cmdsize;
	}


	return 0;


}
