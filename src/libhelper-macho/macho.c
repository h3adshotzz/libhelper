//===--------------------------- libhelper ----------------------------===//
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
//
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//	Copyright (C) 2020, Is This On?, @h3adsh0tzz
//
//  me@h3adsh0tzz.com.
//
//
//===------------------------------------------------------------------===//

#include "libhelper/libhelper.h"
#include "libhelper/libhelper-macho.h"

#include "version.h"


//===-----------------------------------------------------------------------===//
/*-- Mach-O                              									 --*/
//===-----------------------------------------------------------------------===//

/**
 *  Create a `macho_t` structure.
 *  
 *  @returns        `macho_t` structure.
 */
macho_t *macho_create ()
{
    macho_t *ret = malloc (sizeof (macho_t));
    memset (ret, '\0', sizeof (macho_t));
    return ret;
}


/**
 *  Free a `macho_t` structure.
 * 
 *  @param          `macho_t` to free().
 */
void macho_free (macho_t *macho)
{
    macho = NULL;
    free (macho);
}


/**
 *  Load a Mach-O from a given filepath.
 * 
 *  @param          filepath to load from.
 * 
 *  @returns        loaded and parsed `macho_t`.
 */
macho_t *macho_load (const char *filename)
{
    file_t      *file = NULL;
    macho_t     *macho = NULL;

    if (filename) {
        debugf ("macho.c: reading Mach-O from filename: %s\n", filename);

        file = file_load (filename);
        if (file->size <= 0) {
            errorf ("File could not be loaded properly: %d", file->size);
            macho_free (macho);
            return NULL;
        } 

        debugf ("macho.c: creating Mach-O struct\n");
        macho = macho_create_from_buffer (file_load_bytes (file, file->size, 0));

        if (macho == NULL) {
            errorf ("Error creating Mach-O: NULL\n");
            return NULL;
        }

        debugf ("macho.c: all is well\n");
    } else {
        errorf ("No filename specified\n");
    }
    return macho;
}


/**
 * macho_dup_bytes:
 * @macho: the #macho_t to load from.
 * @offset: to start copying from.
 * @buffer: (out caller-allocates): where to put the bytes
 * @size: amount of bytes to copy.
 * 
 * Load bytes from a Mach-O for a given size from an offset.
 */
void
macho_dup_bytes (macho_t *macho, uint32_t offset, void *buffer, size_t size)
{
    memcpy (buffer, macho->data + offset, size);
}


/**
 *  Loads a Mach-O from a given buffer into a `macho_t`
 * 
 *  @param          data to load.
 * 
 *  @returns        a `macho_t` from the given data.
 */
macho_t *macho_create_from_buffer (unsigned char *data)
{
    macho_t *macho = macho_create ();

    macho->data = (uint8_t *) data;
    macho->offset = 0;

    // check that valid bytes were given
    if (!data) {
        errorf ("macho_create_from_buffer: Invalid data\n");
        macho_free (macho);
        return NULL;
    }

    // try to detect if we are handling a FAT file.
    if (FAT(macho->data)) {
        errorf ("macho_create_from_buffer: Cannot handle a FAT archive here\n");
        macho_free (macho);
        return NULL;
    }

    // try to load the mach header, and handle any failure
    macho->header = mach_header_load (macho);
    if (macho->header == NULL) {
        errorf ("macho_create_from_buffer: Mach header is NULL\n");
        macho_free (macho);
        return NULL;
    }

    uint32_t offset = sizeof (mach_header_t);
    HSList *scmds = NULL, *lcmds = NULL, *dylibs = NULL;

    // we'll search through every load command and sort them
    for (int i = 0; i < (int) macho->header->ncmds; i++) {
        mach_load_command_info_t *lc = mach_load_command_info_load (macho->data, offset);

        /**
         *  Different types of Load Command are sorted into one of the three 
         *  lists defined above: scmds, lcmds and dylibs.
         */
        if (lc->lc->cmd == LC_SEGMENT || lc->lc->cmd == LC_SEGMENT_64) {
            if (lc->lc->cmd == LC_SEGMENT) {
                warningf ("macho_create_from_buffer(): Found LC_SEGMENT, 32 bit Mach-O's are not supported for parsing\n");
                continue;
            }

            // create a segment info struct, then add to the list
            mach_segment_info_t *seginf = mach_segment_info_load (macho->data, offset);
            if (seginf == NULL) {
                warningf ("macho_create_from_buffer(): failed to load LC_SEGMENT_64 at offset: 0x%08x\n", offset);
                continue;
            }

            // add to segments lists
            scmds = h_slist_append (scmds, seginf);
        } else if (lc->lc->cmd == LC_ID_DYLIB || lc->lc->cmd == LC_LOAD_DYLIB ||
                   lc->lc->cmd == LC_LOAD_WEAK_DYLIB || lc->lc->cmd == LC_REEXPORT_DYLIB) {
            /**
             *  Because a Mach-O  can have multiple dynamically linked libraries which
             *  means there are multiple LC_DYLIB-like commands, so it's easier that
             *  there is a sperate list for DYLIB-related commands.
             */

            // dylib info struct
            mach_dylib_command_info_t *dylibinfo = malloc (sizeof (mach_dylib_command_info_t));
            uint32_t cmdsize = lc->lc->cmdsize;

            // create the raw command
            mach_dylib_command_t *raw = malloc (sizeof (mach_dylib_command_t));
            memset (raw, '\0', sizeof (mach_dylib_command_t));
            memcpy (raw, macho->data + offset, sizeof (mach_dylib_command_t));

            // laod the name of the dylib. This is located after the load command
            //  and is included in the cmdsize.
            uint32_t nsize = cmdsize - sizeof(mach_dylib_command_t);
            uint32_t noff = offset + raw->dylib.offset;

            char *name = malloc (nsize);
            memset (name, '\0', nsize);
            memcpy (name, macho->data + noff, nsize);

            // set the name, raw cmd struct and type
            dylibinfo->name = name;
            dylibinfo->dylib = raw;
            dylibinfo->type = lc->type;

            // add the offset to the load command
            lc->offset = offset;

            // add them to both lists
            dylibs = h_slist_append (dylibs, dylibinfo);
            lcmds = h_slist_append (lcmds, lc);            

        } else {
            // set the offset of the command so we can find it again
            lc->offset = offset;
            lcmds = h_slist_append (lcmds, lc);
        }

        offset += lc->lc->cmdsize;
    }

    // set macho offset
    macho->offset = offset;

    // set LC lists
    macho->lcmds = lcmds;
    macho->scmds = scmds;
    macho->dylibs = dylibs;

    // fix size
    mach_segment_info_t *last_seg = (mach_segment_info_t *) h_slist_last (macho->scmds);
    macho->size = last_seg->segcmd->fileoff + last_seg->segcmd->filesize;

    return macho;
}


//===-----------------------------------------------------------------------===//
/*-- Mach-O Header functions         									 --*/
//===-----------------------------------------------------------------------===//


/**
 *  Verify a magic number.
 * 
 *  @param         magic number to verify
 * 
 *  @returns       header type
 */
mach_header_type_t mach_header_verify (uint32_t magic)
{
    switch (magic) {
        case MACH_MAGIC_64:
        case MACH_CIGAM_64:
            return MH_TYPE_MACHO64;
        case MACH_MAGIC_32:
        case MACH_CIGAM_32:
            return MH_TYPE_MACHO32;
        case MACH_MAGIC_UNIVERSAL:
        case MACH_CIGAM_UNIVERSAL:
            return MH_TYPE_FAT;
        default:
            break;
    }
    return MH_TYPE_UNKNOWN;
}


/**
 *  Create a `mach_header_t` structure.
 * 
 *  @returns        `mach_header_t` structure
 */
mach_header_t *mach_header_create ()
{
    mach_header_t *ret = malloc (sizeof (mach_header_t));
    memset (ret, '\0', sizeof (mach_header_t));
    return ret;
}


/**
 *  Load a `mach_header_t` from a given `macho_t`
 * 
 *  @param          `macho_t` to load a header from.
 * 
 *  @returns        loaded `mach_header_t`.
 */
mach_header_t *mach_header_load (macho_t *macho)
{
    mach_header_t *hdr = NULL;

    // check the given macho was initialised.
    if (macho) {
        unsigned char *data = macho->data;
        hdr = mach_header_create ();

        // copy bytes from data to hdr
        memcpy (hdr, &data[0], sizeof (mach_header_t));

        // verify the magic value was loaded
        if (!hdr->magic) {
            errorf ("mach_header_load(): No magic value, something went wrong\n");
            return NULL;
        }

        // check the magic
        mach_header_type_t type = mach_header_verify (hdr->magic);

        if (type == MH_TYPE_MACHO64) {
            debugf ("macho.c: mach_header_load(): Detected Mach-O 64 bit\n");
        } else if (type == MH_TYPE_MACHO32) {
            debugf ("macho.c: mach_header_load(): Detected Mach-O 32 bit\n");
        } else if (type == MH_TYPE_FAT) {
            debugf ("macho.c: mach_header_load(): Detected Universal Binary. Libhelper cannot load these.\n");
            hdr = NULL;
        } else {
            errorf ("mach_header_load(): Unknown file magic:0x%08x\n", hdr->magic);
            hdr = NULL;
        }
    }
    return hdr;
}


/**
 *  Grab a human-readable cpu type for a given type.
 * 
 *  @param              cputype
 * 
 *  @returns            string representing given type
 */
char *mach_header_read_cpu_type (cpu_type_t type)
{
    debugf ("macho.c: mach_header_read_cpu_type: type: %d\n", type);

    char *cpu_type = "";
    switch (type) {
        case CPU_TYPE_X86:
            cpu_type = "x86";
            break;
        case CPU_TYPE_X86_64:
            cpu_type = "x86_64";
            break;
        case CPU_TYPE_ARM:
            cpu_type = "arm";
            break;
        case CPU_TYPE_ARM64:
            cpu_type = "arm64";
            break;
        case CPU_TYPE_ARM64_32:
            cpu_type = "arm64_32";
            break;
        default:
            cpu_type = "unknown";
            break;
    }
    return cpu_type;
}


/**
 *  Grab a human-readable cpu subtype for a given subtype
 * 
 *  @param              cputype
 *  @param              cpusubtype
 * 
 *  @returns            string representing given subtype
 */
char *mach_header_read_cpu_subtype (cpu_type_t type, cpu_subtype_t subtype)
{
    char *cpu_subtype = "";
    if (type == CPU_TYPE_X86_64) {
        return "x86_64";
    } else if (type == CPU_TYPE_ARM64) {
        switch (subtype) {
        case CPU_SUBTYPE_ARM64_ALL:
            cpu_subtype = "arm64_all";
            break;
        case CPU_SUBTYPE_ARM64_V8:
            cpu_subtype = "arm64_v8";
            break;
        case CPU_SUBTYPE_ARM64E:
        case CPU_SUBTYPE_PTRAUTH_ABI | CPU_SUBTYPE_ARM64E:
            cpu_subtype = "arm64e";
            break;
        default:
            cpu_subtype = "unknown_arm64";
            break;
        }
    } else {
        cpu_subtype = "unknown_x86";
    }
    return cpu_subtype;
}


/**
 *  Grab a human-readable Mach-O header type
 * 
 *  @param              header type
 * 
 *  @returns            header type string.
 */
char *mach_header_read_file_type (uint32_t type)
{
    char *ret = "";
    switch (type) {
        case MACH_TYPE_OBJECT:
            ret = "Mach Object (MH_OBJECT)";
            break;
        case MACH_TYPE_EXECUTE:
            ret = "Mach Executable (MH_EXECUTE)";
            break;
        case MACH_TYPE_DYLIB:
            ret = "Mach Dynamic Library (MH_DYLIB)";
            break;
        case MACH_TYPE_KEXT_BUNDLE:
            ret = "Mach Kernel Extension Bundle (MH_KEXT_BUNDLE)";
            break;
        case MACH_TYPE_FILESET:
            ret = "Mach File Set (MH_FILESET)";
            break;
        default:
            ret = "Unknown";
            warningf ("mach_header_read_file_type(): Unknown mach-o type: %d\n", type);
            break;
    }
    return ret;
}


/**
 *  Grab a human-readable Mach-O header type (in short)
 * 
 *  @param              header type
 * 
 *  @returns            short header type string.
 */
char *mach_header_read_file_type_short (uint32_t type)
{
    char *ret = "";
    switch (type) {
        case MACH_TYPE_OBJECT:
            ret = "Object";
            break;
        case MACH_TYPE_EXECUTE:
            ret = "Executable";
            break;
        case MACH_TYPE_DYLIB:
            ret = "Dynamic Library";
            break;
        case MACH_TYPE_FILESET:
            ret = "File set";
            break;
        default:
            ret = "Unknown";
            break;
    }
    return ret;
}