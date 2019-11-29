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

#ifndef LIBHELPER_MACHO_SYMBOL_H
#define LIBHELPER_MACHO_SYMBOL_H

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

#include "macho.h"
#include <glib-2.0/glib.h>
#include "file.h"
#include <ctype.h>

/**
 * 	
 */
typedef struct mach_symtab_command_t {
	uint32_t	cmd;			/* LC_SYMTAB */
	uint32_t	cmdsize;		/* sizeof(mach_symtab_command_t) */
	uint32_t	symoff;			/* offset of the symbol table */
	uint32_t	nsyms;			/* number of symbols */
	uint32_t	stroff;			/* offset of the string table */
	uint32_t	strsize;		/* size of the string table in bytes */
} mach_symtab_command_t;

/**
 * 
 */
typedef struct nlist {
	uint32_t	n_strx;			/* index into the string table */

	uint8_t		n_type;			/* type flag */
	uint8_t 	n_sect;			/* section number, or NO_SECT */
	uint16_t	n_desc;			/* see stab.h */
	uint64_t	n_value;		/* value of this symbol (or stab offset) */
} nlist;

/**
 * 
 */
typedef struct mach_symbol_table_t {
    mach_symtab_command_t   cmd;        /* LC_SYMTAB */
    GSList                  *symbols;   /* List of symbols */
} mach_symbol_table_t;


/*
 * The n_type field really contains four fields:
 *	unsigned char N_STAB:3,
 *		      N_PEXT:1,
 *		      N_TYPE:3,
 *		      N_EXT:1;
 * which are used via the following masks.
 */
#define	N_STAB	0xe0  /* if any of these bits set, a symbolic debugging entry */
#define	N_PEXT	0x10  /* private external symbol bit */
#define	N_TYPE	0x0e  /* mask for the type bits */
#define	N_EXT	0x01  /* external symbol bit, set for external symbols */

/*
 * Values for N_TYPE bits of the n_type field.
 */
#define	N_UNDF	0x0		/* undefined, n_sect == NO_SECT */
#define	N_ABS	0x2		/* absolute, n_sect == NO_SECT */
#define	N_SECT	0xe		/* defined in section number n_sect */
#define	N_PBUD	0xc		/* prebound undefined (defined in a dylib) */
#define N_INDR	0xa		/* indirect */


/**
 *  Function definitions
 */
mach_symtab_command_t *mach_symtab_command_create ();
mach_symtab_command_t *mach_symtab_command_load (file_t *file, off_t offset);
char *mach_symtab_find_symbol_name (file_t *file, nlist *sym, mach_symtab_command_t *cmd);
mach_symbol_table_t *mach_symtab_load_symbols (macho_t *macho, mach_symtab_command_t *symbol_table);


#endif /* libhelper_macho_symbol_h */