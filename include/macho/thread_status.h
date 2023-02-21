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
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//  Copyright (C) 2020, Is This On?, @h3adsh0tzz
//  Copyright (C) 2021, Is This On? Holdings Limited
//  Copyright (C) 2023, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#ifndef LIBHELPER_MACHO_THREAD_STATUS_H
#define LIBHELPER_MACHO_THREAD_STATUS_H

#define ARM_THREAD_STATE64		6
#define ARM_THREAD_STATE32		9

struct _arm_thread_state_t
{
    uint32_t r[13];   /* General purpose register r0-r12 */
	uint32_t sp;      /* Stack pointer r13 */
	uint32_t lr;      /* Link register r14 */
	uint32_t pc;      /* Program counter r15 */
	uint32_t cpsr;    /* Current program status register */
};

struct _arm_thread_state_64_t
{
    uint64_t    __x[29];    /* General purpose registers x0-x28 */
	uint64_t    __fp;               /* Frame pointer x29 */
	uint64_t    __lr;               /* Link register x30 */
	uint64_t    __sp;               /* Stack pointer x31 */
	uint64_t    __pc;               /* Program counter */
	uint32_t    __cpsr;             /* Current program status register */
	uint32_t    __flags;    /* Flags describing structure format */
};

typedef struct _arm_thread_state_t      arm_thread_state_t;
typedef struct _arm_thread_state_t      arm_thread_state32_t;
typedef struct _arm_thread_state_64_t   arm_thread_state64_t;

typedef unsigned int                    mach_msg_type_number_t;

#define ARM_THREAD_STATE_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_thread_state_t)/sizeof(uint32_t)))
#define ARM_THREAD_STATE32_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_thread_state32_t)/sizeof(uint32_t)))
#define ARM_THREAD_STATE64_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_thread_state64_t)/sizeof(uint32_t)))

#endif /* libhelper_macho_thread_status_h */