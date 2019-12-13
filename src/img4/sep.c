/**
 *     libhelper
 *     Copyright (C) 2019, @h3adsh0tzz
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
/**
 *  Adapated from original sepsplit.c by xerub. 
 *  -------------------------------------------
 * 
 *  SEP firmware split tool
 *
 *  Copyright (c) 2017 xerub
 */

/**
 *  The current issues:
 * 
 *  I'm not sure what's going on here, there is a BAD_ACCESS crash in calc_size
 *  on the `if (cmd->cmd == LC_SEGMENT_64)` line. It seems that although the 
 *  macho header is loaded correctly, the load commands situated just after the
 *  header are being corrupted.
 * 
 *  lldb output:
 * 
 *  (lldb) r /Users/h3adsh0tzz/Research/SEP/sep.decrypted
 *  Process 80696 launched: '/Users/h3adsh0tzz/Sources/htool/build/tests/sep_split_test' (x86_64)
 *  DEBUG: file loaded okay. attempting to identify macho regions
 *  DEBUG: ==================
 *  Mach-O Header Dump
 *  ==================
 *  
 *  DEBUG: Magic: 		0xfeedfacf
 *  DEBUG: CPU Type: 	arm64
 *  DEBUG: CPU Sub-Type: 	0x2
 *  DEBUG: File Type: 	Mach Executable (MH_EXECUTE)
 *  DEBUG: Load Commands: 	9
 *  DEBUG: LC Size: 	1352
 *  DEBUG: ------------------
 *  
 *  DEBUG: [0]: 0x1004024
 *  DEBUG: [1]: 0x46549f83
 *  Process 80696 stopped
 * thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=1, address=0x146549f83)
 *    frame #0: 0x00000001000029ae sep_split_test`calc_size(ptr="����
 *                                                                     ", size=143511552) at sep.c:81:22
 *     78  	    //  We need to work out the full size of the file, so we work out how many
 *     79  	    //  load commands there are, how big each segment is, then add that to the
 *     80  	    //  size of the header and we have our file size.
 *  -> 81  	    //
 *     82  	    for (i = 0; i < header->ncmds; i++) {
 *     83  	        debugf("[%d]: 0x%x\n", i, lc_ptr);
 *     84  	        const mach_load_command_t *cmd = (const mach_load_command_t *) lc_ptr;
 *  Target 0: (sep_split_test) stopped.
 *  (lldb) fr v
 *  (const uint8_t *) ptr = 0x0000000101004000 "����
 *                                                  "
 *  (size_t) size = 143511552
 *  (unsigned int) i = 1
 *  (const mach_header_t *) header = 0x0000000101004000
 *  (const uint8_t *) lc_ptr = 0x0000000146549f83 ""
 *  (size_t) end = 326417514496
 *  (size_t) tsize = 0
 *  (const mach_load_command_t *) cmd = 0x0000000146549f83
 *  (lldb) fr v cmd->cmd
 *  (uint32_t) cmd->cmd = <read memory from 0x146549f83 failed (0 of 4 bytes read)>
 *  
 *  (lldb) fr v cmd->cmdsize
 *  (uint32_t) cmd->cmdsize = <read memory from 0x146549f87 failed (0 of 4 bytes read)>
 *  
 *  (lldb)
 * 
 *  --------------------------------------------
 * 
 *  Running that same exact sep.decrypted file through the original sepsplit by xerub
 *  gives the following, expected output:
 * 
 *  23:52 h3adsh0tzz@h3adsh0tzzs-iMac.local ~/Sources/htool/tests% ./sepsplit.o ~/Research/SEP/sep.decrypted                  (git)-[master]
 *  boot
 *  kernel
 *  ...
 *  macho8
 * 
 */

#include "libhelper-img4/sep.h"

uint8_t     *kernel         = MAP_FAILED;
size_t       kernel_size    = 0;
static int   kernel_fd      = -1;

#define IS64(image) (*(uint8_t *)(image) & 1)
#define MACHO(p) ((*(unsigned int *)(p) & ~1) == 0xfeedface)

static
size_t calc_size (const uint8_t *ptr, size_t size)
{
    //  Create a struct for the mach header, and assume that the mem region
    //  at `ptr` is a Mach-O header - until we can check it. Also create a 
    //  pointer to, what should be, the start of the load commands relative
    //  to the header pointer.
    //
    unsigned i;
    const mach_header_t *header = (const mach_header_t *) ptr;
    const uint8_t *lc_ptr = ptr + sizeof(mach_header_t);
    size_t end, tsize = 0;


    //  Check three things: If the size of the data at `ptr` is less thab
    //  1024 bytes, it's most likely not a Mach-O, so ignore it.
    //
    //  Then check if it is actually a Mach-O, and then if its 64bit.
    //
    if (size < 1024) {
        return 0;
    }
    if (!MACHO(ptr)) {
        return 0;
    }
    if (IS64(ptr)) {
        lc_ptr += 4;
    }

    mach_header_print_summary (header);

    //  We need to work out the full size of the file, so we work out how many
    //  load commands there are, how big each segment is, then add that to the
    //  size of the header and we have our file size.
    //
    for (i = 0; i < header->ncmds; i++) {
        debugf("[%d]: 0x%x\n", i, lc_ptr);
        const mach_load_command_t *cmd = (const mach_load_command_t *) lc_ptr;
        
        //  Because SEPOS is 64bit, not 32bit, I'm not including checking for
        //  32bit segment commands - at least not until I've implemented it in
        //  libhelper-macho.
        //

        if (cmd->cmd == LC_SEGMENT_64) {
            const mach_segment_command_64_t *seg = (const mach_segment_command_64_t *) lc_ptr;
            end = seg->fileoff + seg->filesize;
            if (tsize < end) {
                tsize = end;
            }
        }


        // Advance the lc_ptr
        //
        lc_ptr += cmd->cmdsize;
    }
    
    return tsize;
}

int sep_split_init (const char *filename)
{
    // Try to open and load the file into 'kernel_fd'
    kernel_fd = open (filename, O_RDONLY);
    if (kernel_fd < 0) {
        errorf ("There was a problem opening the file: %s\n", filename);
        exit (0);
    }

    // get the size of the file
    kernel_size = lseek (kernel_fd, 0, SEEK_END);

    // mmap the file into the kernel pointer
    kernel = mmap (NULL, kernel_size, PROT_READ, MAP_PRIVATE, kernel_fd, 0);
    if (kernel == MAP_FAILED) {
        close (kernel_fd);
        errorf ("Could not map the file.\n");
        exit (0);
    }

    debugf ("file loaded okay. attempting to identify macho regions\n");
    
    //  Now we start trying to split the sepos firmware. There are 9 areas
    //  we need to extract, the first being the bootloader, the second being
    //  the L4 Kernel Mach-O and the other 7 being application Mach-O's for
    //  SEPOS.
    //
    size_t i, last = 0;
    unsigned j = 0;
    for (i = 0; i < kernel_size; i += 4) {

        //  Each loop jumps 4 bytes, the length of a Mach-O magic. We are searching
        //  for the start of a Mach-O header, which looks like 0xCDFAEDFE. The 'kernel'
        //  points to the current part of the mapped file that we want to check for
        //  a Mach-O.
        //
        //  Basically, if `kernel + i` points to a valid Mach-O header, then we have
        //  found one of the SEPOS componenents. If the pointer is valid, calc_size
        //  will return a valid size of the Mach-O so we can load it from our calculated
        //  offset.
        //
        size_t sz = calc_size (kernel + i, kernel_size - i);
        if (sz) {
            debugf ("restore_file ()\n");
            //restore_file(j++, kernel + last, i - last, restore);
            j++;
            last = i;
            i += sz - 4;
        }
    }
    debugf ("out of loop restore_file ()\n");
    //restore_file(j, kernel + last, i - last, restore);


    return 0;
}