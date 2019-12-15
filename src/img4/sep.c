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

static unsigned char *
boyermoore_horspool_memmem(const unsigned char* haystack, size_t hlen,
                           const unsigned char* needle,   size_t nlen)
{
    size_t last, scan = 0;
    size_t bad_char_skip[__UCHAR_MAX + 1]; /* Officially called:
                                          * bad character shift */

    /* Sanity checks on the parameters */
    if (nlen <= 0 || !haystack || !needle)
        return NULL;

    /* ---- Preprocess ---- */
    /* Initialize the table to default value */
    /* When a character is encountered that does not occur
     * in the needle, we can safely skip ahead for the whole
     * length of the needle.
     */
    for (scan = 0; scan <= __UCHAR_MAX; scan = scan + 1)
        bad_char_skip[scan] = nlen;

    /* C arrays have the first byte at [0], therefore:
     * [nlen - 1] is the last byte of the array. */
    last = nlen - 1;

    /* Then populate it with the analysis of the needle */
    for (scan = 0; scan < last; scan = scan + 1)
        bad_char_skip[needle[scan]] = last - scan;

    /* ---- Do the matching ---- */

    /* Search the haystack, while the needle can still be within it. */
    while (hlen >= nlen)
    {
        /* scan from the end of the needle */
        for (scan = last; haystack[scan] == needle[scan]; scan = scan - 1)
            if (scan == 0) /* If the first byte matches, we've found it. */
                return (void *)haystack;

        /* otherwise, we need to skip some bytes and start again.
           Note that here we are getting the skip value based on the last byte
           of needle, no matter where we didn't match. So if needle is: "abcd"
           then we are skipping based on 'd' and that value will be 4, and
           for "abcdd" we again skip on 'd' but the value will be only 1.
           The alternative of pretending that the mismatched character was
           the last character is slower in the normal case (E.g. finding
           "abcd" in "...azcd..." gives 4 by using 'd' but only
           4-2==2 using 'z'. */
        hlen     -= bad_char_skip[haystack[last]];
        haystack += bad_char_skip[haystack[last]];
    }

    return NULL;
}

static size_t
restore_linkedit(uint8_t *p, size_t size)
{
    unsigned i;
    struct mach_header *hdr = (struct mach_header *)p;
    uint64_t min = -1;
    uint64_t delta = 0;
    int is64 = 0;
    uint8_t *q;

    if (size < 1024) {
        return -1;
    }
    if (!MACHO(p)) {
        return -1;
    }
    if (IS64(p)) {
        is64 = 4;
    }

    q = p + sizeof(struct mach_header) + is64;
    for (i = 0; i < hdr->ncmds; i++) {
        const struct load_command *cmd = (struct load_command *)q;
        if (cmd->cmd == LC_SEGMENT) {
            const struct segment_command *seg = (struct segment_command *)q;
            if (strcmp(seg->segname, "__PAGEZERO") && min > seg->vmaddr) {
                min = seg->vmaddr;
            }
        }
        if (cmd->cmd == LC_SEGMENT_64) {
            const struct segment_command_64 *seg = (struct segment_command_64 *)q;
            if (strcmp(seg->segname, "__PAGEZERO") && min > seg->vmaddr) {
                min = seg->vmaddr;
            }
        }
        q = q + cmd->cmdsize;
    }

    q = p + sizeof(struct mach_header) + is64;
    for (i = 0; i < hdr->ncmds; i++) {
        const struct load_command *cmd = (struct load_command *)q;
        if (cmd->cmd == LC_SEGMENT) {
            struct segment_command *seg = (struct segment_command *)q;
            if (!strcmp(seg->segname, "__LINKEDIT")) {
                delta = seg->vmaddr - min - seg->fileoff;
                seg->fileoff += delta;
            }
        }
        if (cmd->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *seg = (struct segment_command_64 *)q;
            if (!strcmp(seg->segname, "__LINKEDIT")) {
                delta = seg->vmaddr - min - seg->fileoff;
                seg->fileoff += delta;
            }
        }
        if (cmd->cmd == LC_SYMTAB) {
            struct symtab_command *sym = (struct symtab_command *)q;
            if (sym->stroff) sym->stroff += delta;
            if (sym->symoff) sym->symoff += delta;
        }
        q = q + cmd->cmdsize;
    }

    return 0;
}

static size_t
calc_size(const uint8_t *p, size_t size)
{
    unsigned i;
    const struct mach_header *hdr = (struct mach_header *)p;
    const uint8_t *q = p + sizeof(struct mach_header);
    size_t end, tsize = 0;

    if (size < 1024) {
        return 0;
    }
    if (!MACHO(p)) {
        return 0;
    }
    if (IS64(p)) {
        q += 4;
    }

    for (i = 0; i < hdr->ncmds; i++) {
        const struct load_command *cmd = (struct load_command *)q;
        if (cmd->cmd == LC_SEGMENT) {
            const struct segment_command *seg = (struct segment_command *)q;
            end = seg->fileoff + seg->filesize;
            if (tsize < end) {
                tsize = end;
            }
        }
        if (cmd->cmd == LC_SEGMENT_64) {
            const struct segment_command_64 *seg = (struct segment_command_64 *)q;
            end = seg->fileoff + seg->filesize;
            if (tsize < end) {
                tsize = end;
            }
        }
        q = q + cmd->cmdsize;
    }

    return tsize;
}

uint8_t *kernel = MAP_FAILED;
size_t kernel_size = 0;
static int kernel_fd = -1;

static int
init_kernel(const char *filename)
{
    kernel_fd = open(filename, O_RDONLY);
    if (kernel_fd < 0) {
        return -1;
    }

    kernel_size = lseek(kernel_fd, 0, SEEK_END);

    kernel = mmap(NULL, kernel_size, PROT_READ, MAP_PRIVATE, kernel_fd, 0);
    if (kernel == MAP_FAILED) {
        close(kernel_fd);
        kernel_fd = -1;
        return -1;
    }

    return 0;
}

static void
term_kernel(void)
{
    munmap(kernel, kernel_size);
    close(kernel_fd);
}

static int
write_file(const char *name, const void *buf, size_t size)
{
    int fd;
    size_t sz;
    fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    sz = write(fd, buf, size);
    close(fd);
    return (sz == size) ? 0 : -1;
}

static int
restore_file(unsigned index, const unsigned char *buf, size_t size, int restore)
{
    int rv;
    void *tmp;
    char name[256];
    char tail[12 + 1];

    if (index == 1 && size > 4096) {
        unsigned char *toc = boyermoore_horspool_memmem(buf + size - 4096, 4096, (unsigned char *)"SEPOS       ", 12);
        if (toc) {
            unsigned char *p = boyermoore_horspool_memmem(toc + 1, 64, (unsigned char *)"SEP", 3);
            if (p) {
                sizeof_sepapp = p - toc;
            }
            apps = (struct sepapp_t *)(toc - offsetof(struct sepapp_t, name));
        }
    }

    if (apps && buf > (unsigned char *)apps) {
        char *p;
        memcpy(tail, apps->name, 12);
        for (p = tail + 12; p > tail && p[-1] == ' '; p--) {
            continue;
        }
        *p = '\0';
        printf("%-12s phys 0x%lx, virt 0x%x, size 0x%x, entry 0x%x\n", tail, apps->phys, apps->virt, apps->size, apps->entry);
        apps = (struct sepapp_t *)((char *)apps + sizeof_sepapp);
    } else {
        if (index == 0) {
            strcpy(tail, "boot");
            printf("%s\n", tail);
        } else if (index == 1) {
            strcpy(tail, "kernel");
            printf("%s\n", tail);
        } else {
            *tail = '\0';
            printf("macho%d\n", index);
        }
    }
    snprintf(name, sizeof(name), "sepdump%02u_%s", index, tail);
    if (!restore) {
        return write_file(name, buf, size);
    }
    tmp = malloc(size);
    if (!tmp) {
        return -1;
    }
    memcpy(tmp, buf, size);
    restore_linkedit(tmp, size);
    rv = write_file(name, tmp, size);
    free(tmp);
    return rv;
}

static int
split(int restore)
{
    size_t i;
    unsigned j = 0;
    size_t last = 0;
    for (i = 0; i < kernel_size; i += 4) {
        size_t sz = calc_size(kernel + i, kernel_size - i);
        if (sz) {
            restore_file(j++, kernel + last, i - last, restore);
            last = i;
            i += sz - 4;
        }
    }
    restore_file(j, kernel + last, i - last, restore);
    return 0;
}

void
sep_split_init (char *filename)
{
    if (USE_LIBHELPER_MACHO) {
        debugf ("Using Libhelper-Macho\n");
    } else {
        debugf ("Using Default AAPL Mach-O Loader\n");
    }

    int rv = init_kernel (filename);
    if (rv) {
        fprintf (stderr, "[e] cannot read kernel\n");
        exit (0);
    }

    rv = split (1);

    term_kernel ();
}