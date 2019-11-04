/**
 * 
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

#include "macho.h"


uint32_t macho_read_magic (unsigned char *buf, int offset)
{
    uint32_t magic = 0;
    memcpy (&magic, buf, sizeof(uint32_t));
    if (!magic) {
        g_print ("[*] Error: Could not read magic from Mach-O.\n");
        exit (0);
    }
    return magic;
}

void mach_header_dump_test (unsigned char *buf, int offset)
{
    //struct mach_header_64_t *header = malloc (sizeof(struct mach_header_64_t));
    //memcpy (&header, buf, sizeof(struct mach_header_64_t));
    struct mach_header_64_t *header = (struct mach_header_64_t *) buf;

    g_print ("magic: 0x%x\n", header->magic);
    g_print ("cputype: 0x%x\n", header->cputype);
    g_print ("lc: 0x%x\n", header->ncmds);
}