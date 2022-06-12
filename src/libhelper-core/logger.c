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

/* libhelper-core: */

#include "libhelper-hlibc.h"
#include "libhelper-logger.h"

void
hlog_print_list_header (char *header)
{
    printf (ANSI_COLOR_RED "%s:\n" ANSI_COLOR_RESET, header);
}

void
hlog_print_list_subheader (char *padding, char *header)
{
    printf ("%s" ANSI_COLOR_DARK_WHITE "%s\n" ANSI_COLOR_RESET, padding, header);
}

void
hlog_print_list_item (char *padding, char *name, char *fmt, ...)
{
    va_list arg;
    int done;

    fmt = mstrappend ("%s%s%s%s%s%s%s%s%s%s", ANSI_COLOR_BOLD, ANSI_COLOR_DARK_WHITE, padding, name, ": ", 
        ANSI_COLOR_RESET, ANSI_COLOR_DARK_GREY, fmt, "\n", ANSI_COLOR_RESET);

    va_start (arg, fmt);
    done = vfprintf (stdout, fmt, arg);
    va_end (arg);
}