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

/* libhelper-core: hlibc */

#ifndef __LIBHELPER_CORE_HLIBC_H__
#define __LIBHELPER_CORE_HLIBC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/mman.h>

/**
 *  HString macro's
 */
#define H_UNLIKELY(expr)            (expr)
#define MY_MAXSIZE                  ((size_t) -1)
#define MAX(a, b)                   (((a) > (b)) ? (a) : (b))
#define MIN(a, b)                   (((a) < (b)) ? (a) : (b))

//
//	@TODO: This generates a warning:
//		warning: incompatible poiunter types returning 'char *' from a
//			function with result type 'HString *' (aka 'struct _HString *')
//
/*#define h_return_val_if_fail(expr, val)  \
    if(!(#expr)) {                       \
        debugf ("bugger\n");             \
        return #val;                     \
    }*/
#define h_return_val_if_fail(expr, val)     0

/**
 *  \brief      Allocate a block of memory and init to 0.
 * 
 *  \param size     Amount of memory to allocate.
 *  
 *  \returns    Pointer to allocated memory.
 */
void *
h_slice_alloc0 (size_t size);


/******************************************************************************
* Libhelper HLibc HSList (Singly-linked lists)
* 
* API for Singly-Linked Lists. Implementation "borrowed" from Glib.
* 
*******************************************************************************/

/**
 *  \brief      HSList item.
 * 
 *  HSList structure. Contains the data pointer, and the pointer to the next item
 *  in the list, hence "singly-linked list".
 */
typedef struct _libhelper_hlibc_hslist       HSList;
struct _libhelper_hlibc_hslist {
    void        *data;      /* pointer to data */
    HSList      *next;      /* pointer to next list item */
};

/**
 *  \brief      HSList API Get Last Element.
 * 
 *  Get the last element in a given list and return it.
 * 
 *  \param  list    A HSList.
 * 
 *  \returns    The last element in the given list.
 */
extern HSList *
h_slist_last (HSList    *list);

/**
 *  \brief      HSList API Append list.
 * 
 *  Append a given element to a given list, and return the list with the appended
 *  element.
 * 
 *  \param  list    A HSList.
 *  \param  data    Data to append to the list.
 * 
 *  \returns    The list with the appended item.
 */
extern HSList *
h_slist_append (HSList  *list,
                void    *data);

/**
 *  \brief      HSList API Remove Item.
 * 
 *  Remove data from a given list at a given index, and return the modified list
 *  back.
 * 
 *  \param  list    A HSList.
 *  \param  data    Date to remove from the list.
 * 
 *  \returns    The list with the given data removed.
 */
extern HSList *
h_slist_remove (HSList  *list,
                void    *data);

/**
 *  \brief      HSList API Get List Length.
 * 
 *  Return the length of a given List.
 * 
 *  \param  list    A HSList.
 * 
 *  \returns    The length of the given list.
 */
extern int
h_slist_length (HSList  *list);

/**
 *  \brief      HSList API Get Data at Index.
 * 
 *  Return the data pointer from the given list at a given index.
 * 
 *  \param  list    A HSList.
 *  \param  n       Index.
 * 
 *  \returns    Pointer to the data at position `n`.
 */
extern void *
h_slist_nth_data (HSList    *list,
                  int        n);


/******************************************************************************
* Libhelper HLibc HString
* 
* API for Strings. Implementation "borrowed" from Glib.
* 
*******************************************************************************/

/**
 *  \brief      HString
 * 
 *  String implementation in C. Contains the char pointer, length and allocated
 *  length.
 */
struct _libhelper_hstring {
    char        *str;           /* char pointer for string */
    size_t       len;           /* string length */
    size_t       allocated;     /* allocated string length */
};
typedef struct _libhelper_hstring       HString;

/**
 *  \brief      HString API New.
 * 
 *  Create a new HString, allocate with initial value, and return the pointer.
 * 
 *  \param  init    Initial string value.
 * 
 *  \returns    The pointer to the new HString.
 */
extern HString *
h_string_new (const char            *init);

/**
 *  \brief      HString API New.
 * 
 *  Create an empty HString with a given intial size and return the pointer.
 * 
 *  \param  size    Initial String size.
 * 
 *  \returns    The pointer to the new HString.
 */
extern HString *
h_string_sized_new (size_t          size);

/**
 *  \brief      HString API Insert.
 * 
 *  Insert another string into a HString at a given position.
 * 
 *  \param  string  HString to modify.
 *  \param  pos     Position to insert the new string at.
 *  \param  val     Char pointer to insert.
 *  \param  len     Length of `val`.
 * 
 *  \returns    The modified HString.
 */
extern HString *
h_string_insert_len (HString        *string,
                     size_t          pos,
                     const char     *val,
                     size_t          len);

/**
 *  \brief      HString API Append.
 * 
 *  Append a given String with a char pointer and return the modified string.
 * 
 *  \param  string  HString to modify.
 *  \param  val     Char pointer to insert.
 *  \param  len     Length of `val`.
 * 
 *  \returns    The append HString.
 */
extern HString *
h_string_append_len (HString        *string,
                     const char     *val,
                     size_t          len);

/**
 *  \brief      HString API Insert Character.
 * 
 *  Insert a single character at a given index of a given HString and return
 *  the pointer to the modified string.
 *  
 *  \param  string  HString to modify.
 *  \param  pos     Position to insert the character.
 *  \param  c       Character to insert.
 * 
 *  \returns    Pointer to modified HString.
 */
extern HString *
h_string_insert_c (HString          *string,
                   size_t            pos,
                   char              c);

/**
 *  \brief      HString API Append Character.
 * 
 *  Append a single character to a given HString and return the pointer to the 
 *  modified string.
 *  
 *  \param  string  HString to modify.
 *  \param  c       Character to append.
 * 
 *  \returns    Pointer to modified HString.
 */
extern HString *
h_string_append_c (HString          *string,
                   char              c);

/**
 *  \brief      String List.
 */
struct __libhelper_hlibc_stringlist {
	char	    **ptrs;
	int			  count;
};
typedef struct __libhelper_hlibc_stringlist     StringList;

/**
 *  \brief       Split a string by a substring.
 * 
 *  Splits a string at a given delimiter, and returns a StringList.
 * 
 *  \param  s       Char pointer to split.
 *  \param  delim   Delimiter.
 * 
 *  \returns    A StringList.
 */
extern StringList *
strsplit (const char                *s,
          const char                *delim);

/**
 *  \brief      Append a to b.
 * 
 *  Take two strings (char ptr's), and append b to a and return the resulting
 *  string.
 *  
 *  \param  a   String to be appended.
 *  \param  b   String to append to `a`.
 *  
 *  \returns    Param b appended to a.
 */
extern char *
strappend (char     *a,
           char     *b);

/**
 *  \brief      Append multiple strings.
 * 
 *  Take multiple strings (char ptr's) to eachother and return the resulting
 *  string.
 *  
 *  \param  fmt   String to be append.
 *  
 *  \returns    Appended strings.
 */
extern char *
mstrappend (char *fmt, ...);


#ifdef __cplusplus
}
#endif

#endif /* __libhelper_core_hlibc_h__ */