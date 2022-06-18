//===----------------------------------------------------------------------===//
//
//                         The Libhelper Project
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-macho: tests of macho api */

#include <stdio.h>

#include <libhelper.h>
#include <libhelper-macho.h>
#include <libhelper-logger.h>
#include <libhelper-file.h>

///////////////////////////////////////////////////////////////////////////////

void print_header (mach_header_t *mh)
{
    printf ("==HEADER==\n");
    printf ("      magic: 0x%08x\n", mh->magic);
    printf ("    cputype: 0x%08x\n", mh->cputype);
    printf (" cpusubtype: 0x%08x\n", mh->cpusubtype);
    printf ("   filetype: 0x%04x\n", mh->filetype);
    printf ("      ncmds: %d\n", mh->ncmds);
    printf (" sizeofcmds: 0x%08x\n", mh->sizeofcmds);
    printf ("      flags: 0x%08x\n", mh->flags);
    printf ("==HEADER==\n");
}

typedef struct test_thread {
        uint32_t cmd;
        uint32_t cmdsize;
        uint32_t flavour;
        uint32_t count;
        uint64_t regs[29];
        uint64_t fp;
        uint64_t lr;
        uint64_t sp;
        uint64_t pc;
        uint32_t cpsr;
} test_thread;

struct arm_saved_state64 {
	uint64_t    x[29];		/* General purpose registers x0-x28 */
	uint64_t    fp;			/* Frame pointer x29 */
	uint64_t    lr;			/* Link register x30 */
	uint64_t    sp;			/* Stack pointer x31 */
	uint64_t    pc;			/* Program counter */
	uint32_t    cpsr;		/* Current program status register */
	uint32_t	reserved;	/* Reserved padding */
	uint64_t	far;		/* Virtual fault address */
	uint32_t	esr;		/* Exception syndrome register */
	uint32_t	exception;	/* Exception number */
};
typedef struct arm_saved_state64 arm_saved_state64_t;

int main (int argc, char *argv[])
{
    const char *filename = argv[1];
    printf ("\n**Running Libhelper-MachO Test Suite**\n");

    /* macho_check_arch */
    mach_header_type_t type = macho_check_arch (filename);
    if (type != MH_TYPE_MACHO64) {
        debugf ("macho is not 64-bit: type: %d, filename: %s\n", type, filename);
        test_failuref ("macho_check_arch\n");
        return 0;
    } else {
        test_successf ("macho_check_arch\n");
    }


    /* macho_64_load */
    macho_t *macho = macho_64_load (filename);
    if (!macho) {
        test_failuref ("macho_64_load\n");
    } else {
        test_successf ("macho_64_load\n");
    }


    /////////////////////////////////////////////////////////////

    mach_load_command_info_t *inf = NULL;
    for (int i = 0; i < h_slist_length (macho->lcmds); i++) {
        inf = (mach_load_command_info_t *) h_slist_nth_data (macho->lcmds, i);
        if (inf->lc->cmd == LC_THREAD || inf->lc->cmd == LC_UNIXTHREAD)
            break;
    }

    mach_thread_command_t *thread_cmd = (mach_thread_command_t *) inf->lc;
    printf ("cmdsize: 0x%08llx\n", thread_cmd->cmdsize);

    printf ("flavour: %d, count: %d\n", thread_cmd->flavour, thread_cmd->count);

    for (int i = 0; i < 29; i++) {
        printf ("x%d: 0x%016llx\n", i, thread_cmd->regs[i]);
    }

    printf ("fp: 0x%016llx\n", thread_cmd->fp);
    printf ("lr: 0x%016llx\n", thread_cmd->lr);
    printf ("sp: 0x%016llx\n", thread_cmd->sp);
    printf ("pc: 0x%016llx\n", thread_cmd->pc);
    printf ("cpsr: 0x%08llx\n", thread_cmd->cpsr);

    //test_thread *test = (test_thread *) thread_cmd;
    //printf ("flavour: 0x%08llx, count: 0x%08llx\n", test->flavour, test->count);
    //
    //for (int i = 0; i < 34; i++) {
    //    uint64_t reg = test->regs[i];
    //    printf("x%d: 0x%08llx\n", i, reg);
    //}

    //printf("cpsr: 0x%08llx\n", test->cpsr);

    return 0;
}
