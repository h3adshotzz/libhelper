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

#ifndef LIBHELPER_MACHO_THREADSTATE_H
#define LIBHELPER_MACHO_THREADSTATE_H

/**
 *                  === The Libhelper Project ===
 *                          Mach-O Parser
 * 
 *  Documentation relating to the macho-segment.h header file:          |
 *                                                                      |
 *                                                                      |
 * 
 *  ----------------
 *  Original Author:
 *      Harry Moulton, @h3adsh0tzz  -   me@h3adsh0tzz.com.
 * 
 */

#include <ctype.h>

typedef struct arm_thread_state64_t {
    uint64_t    __x[29];    /* General purpose registers x0-28 */
    uint64_t    __fp;       /* Frame pointer x29 */
    uint64_t    __lr;       /* Link register x30 */
    uint64_t    __sp;       /* Stack pointer x31 */
    uint64_t    __pc;       /* Program counter */
    uint64_t    __cpsr;     /* Current program status register */
} arm_thread_state64_t;


#endif /* libhelper_macho_threadstate_h */