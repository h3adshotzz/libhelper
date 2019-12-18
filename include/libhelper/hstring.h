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
 *                          HLibc? Maybe...
 *
 *  Implementation of Strings. Currently Libhelper uses GLib "GStrings",
 *  as part of my effort to move away from GLib and use as little external
 *  dependencies as possible - to make running my applications easier - I
 *  will put the effort in to rewrite certain GLib functionality.
 * 
 *  ----------------
 *  Original Author:
 *      Harry Moulton, @h3adsh0tzz  -   me@h3adsh0tzz.com.
 * 
 */

#ifndef _LIBHELPER_H_STRING_H_
#define _LIBHELPER_H_STRING_H_

#include <ctype.h>
#include "strutils.h"
#include "hslist.h"

typedef struct _HString HString;

struct _HString {
    char        *str;
    size_t       len;
    size_t       allocated;
};

#define H_UNLIKELY(expr) (expr)

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#define h_return_val_if_fail(expr, val)  \
    if(!(#expr)) {                       \
        debugf ("bugger\n");             \
        return #val;                     \
    }

#define MY_MAXSIZE  ((size_t) -1)

HString *h_string_new (const char *init);
HString *h_string_insert_len (HString *string, size_t pos, const char *val, size_t len);
HString *h_string_append_len (HString *string, const char *val, size_t len);
HString *h_string_sized_new (size_t size);

HString *h_string_insert_c (HString *string, size_t pos, char c);
HString *h_string_append_c (HString *string, char c);

#endif /* _libhelper_h_string_h_ */