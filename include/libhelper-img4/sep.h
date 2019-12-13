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
 *                  === The Libhelper Project ===
 *                             Image4
 *
 *  Part of the Image4 sub-lib. Handles the splitting and analysis of
 *  Secure Enclave OS (SEPOS) firmware files. The SEP splitting code is
 *  adapted from @xerub's sepsplit.c to make it usable in a library, and
 *  I've added my own spin to it.
 *                                                                      |
 *                                                                      |
 * 
 *  ----------------
 *  Original Author:
 *      Harry Moulton, @h3adsh0tzz  -   me@h3adsh0tzz.com.
 * 
 */

#ifndef _LIBHELPER_IMG4_SEP_H_
#define _LIBHELPER_IMG4_SEP_H_

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "libhelper-macho/macho-header.h"
#include "libhelper-macho/macho-command.h"
#include "libhelper-macho/macho-segment.h"

#include "libhelper/strutils.h"

int sep_split_init (const char *filename);

#endif /* _libhelper_img4_sep_h_ */