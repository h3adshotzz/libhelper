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
//  Copyright (C) 2022, Is This On? Holdings Limited
//
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-image4: */

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "libhelper-asn1.h"
#include "libhelper-image4.h"

#define IMAGE4_DEBUG        1

void hexdump (char *title, char *mem, uint32_t size)
{
    debugf ("hexdump: %s\n", title);
    uint8_t offset = 0x0;
    int lines = size / 16;
    int pos = 0;  //pos

    for (int i = 0; i < lines; i++) {

        printf ("%08x  ", offset);

        uint8_t ln[16];

        for (int j = 0; j < 16; j++) {
            uint8_t byte = (uint8_t) mem[pos];
            printf ("%02x ", byte);

            if (j == 7) printf (" ");

            pos++;
            ln[j] = byte;
        }

        printf ("  |");

        for (int k = 0; k < 16; k++) {

            if (ln[k] < 0x20 || ln[k] > 0x7e) {
                printf (".");
            } else {
                printf ("%c", (char) ln[k]);
            }

        }

        printf ("|\n");

        offset += 0x10;
    }

    printf ("\n");
}

//===----------------------------------------------------------------------===//
/*                      Image4 Struct Loaders                                 */
//===----------------------------------------------------------------------===//

static image4_t *
image4_load_file (const char *path)
{
    /* create an image4 structure */
    image4_t *image4 = calloc (1, sizeof (image4_t));

    /* verify file path */
    if (!path) {
        errorf ("htool_image4ary_load: file path is invalid\n");
        return NULL;
    }
    image4->path = (char *) strdup (path);

    /* create the file descriptor */
    int fd = open (image4->path, O_RDONLY);

    /* calculate the file size */
    struct stat st;
    fstat (fd, &st);
    image4->size = st.st_size;

    /* mmap the file */
    image4->data = mmap (NULL, image4->size, PROT_READ, MAP_PRIVATE, fd, 0);
    close (fd);

    /* verify the map was successful */
    if (image4->data == MAP_FAILED) {
        errorf ("htool_image4ary_load: failed to map file at path: %s\n", image4->path);
        return NULL;
    }

    return (image4) ? image4 : NULL;
}


image4_t *
image4_load (const char *path)
{
    /* load the file at the given path into a new image4_t struct */
    image4_t *image4 = image4_load_file (path);

    /* verify the load */
    if (!image4) goto FAIL;

    /* check and set the image4 file type */
    image4->type = image4_get_file_type (image4);
    debugf ("image4->type: %d\n", image4->type);

    /**
     *  Depending on the type set above, parse the data within the
     *  image4_t struct.
     */
    if (image4->type == IMAGE4_COMP_TYPE_IMG4) {

        /**
         *  A full Image4 file can contain an IM4P, IM4M and an IM4R. We'll
         *  check if each exists, and parse them.
         */
        char *magic;
        size_t l;

        /* Verify that the image4->data is an IMG4 tag */
        asn1_get_sequence_name (image4->data, &magic, &l);
        if (strncmp (magic, "IMG4", l)) {
            errorf ("Expected \"IMG4\", got \"%s\"\n", magic);
            goto FAIL_RETURN_NULL;
        }

        /**
         *  Check for an IM4P.
         *
         *  If there is an IM4P in this .img4, it should be the first element
         *  at index 1.
         */
        char *im4p_raw = (char *) asn1_element_at_index (image4->data, 1);
        asn1_get_sequence_name (im4p_raw, &magic, &l);
        if (!strncmp (magic, "IM4P", 4)) {
            /* set the flag and im4p struct */
            image4->flags |= IMAGE4_FLAG_INCLUDES_IM4P;
            image4->im4p = image4_parse_im4p (im4p_raw);

            if (image4->im4p == NULL) goto FAIL_RETURN_NULL;
            debugf ("IM4P FOUND\n");
        }

        /**
         *  Check for an IM4M
         */
        char *im4m_raw = (char *) asn1_element_at_index (image4->data, 2);
        if (((asn1_tag_t *) im4m_raw)->tag_class != kASN1TagClassContextSpecific) {
            errorf ("Unexpected Tag 0x%02x, expected SET\n", *(unsigned char*) im4m_raw);
            goto FAIL_RETURN_NULL;
        }

        im4m_raw += asn1_len (im4m_raw + 1).size_bytes + 1;
        asn1_get_sequence_name (im4m_raw, &magic, &l);
        if (!strncmp ("IM4M", magic, 4)) {
            /* set the flag and im4m struct */
            image4->flags |= IMAGE4_FLAG_INCLUDES_IM4M;
            image4->im4m = image4_parse_im4m (im4m_raw);

            debugf ("IM4M FOUND\n");
        }

        /**
         *  Check for an IM4R
         */
        char *im4r_raw = (char *) asn1_element_at_index (image4->data, 2);
        if (((asn1_tag_t *) im4r_raw)->tag_class != kASN1TagClassContextSpecific) {
            errorf ("Unexpected Tag 0x%02x, expected SET\n", *(unsigned char*) im4r_raw);
            goto FAIL_RETURN_NULL;
        }

        im4r_raw += asn1_len (im4r_raw + 1).size_bytes + 1;
        asn1_get_sequence_name (im4r_raw, &magic, &l);
        if (!strncmp ("IM4R", magic, 4)) {
            image4->flags |= IMAGE4_FLAG_INCLUDES_IM4R;
            debugf ("IM4M FOUND\n");
        }

    } else if (image4->type == IMAGE4_COMP_TYPE_IM4P) {

        /* check that the file type is an IM4P */
        char *magic;
        size_t l;

        asn1_get_sequence_name (image4->data, &magic, &l);
        if (!strncmp (magic, "IM4P", 4)) {
            debugf ("magic: %s\n", magic);

            /* set the flag and im4p struct */
            image4->flags |= IMAGE4_FLAG_SINGLE_COMPONENT;
            image4->im4p = image4_parse_im4p (image4->data);

            if (image4->im4p == NULL) goto FAIL_RETURN_NULL;
        }

    } else if (image4->type == IMAGE4_COMP_TYPE_IM4M) {

        /* check that the file type is an IM4M */
        char *magic;
        size_t l;

        asn1_get_sequence_name (image4->data, &magic, &l);
        if (!strncmp (magic, "IM4M", 4)) {
            debugf ("magic: %s\n", magic);

            /* set the flag and im4m struct */
            image4->flags |= IMAGE4_FLAG_SINGLE_COMPONENT;
            image4->im4m = image4_parse_im4m (image4->data);

            if (image4->im4m == NULL) goto FAIL_RETURN_NULL;
        }
    }

FAIL:
    return (image4) ? image4 : NULL;

FAIL_RETURN_NULL:
    errorf ("An error occured while parsing the Image4: %s\n", path);
    return NULL;
}


im4p_t *
image4_parse_im4p (unsigned char *buf)
{
    im4p_t *im4p = calloc (1, sizeof (im4p_t));

    /* verify the magic is an im4p */
    char *magic;
    size_t l;

    asn1_get_sequence_name (buf, &magic, &l);
    if (strncmp (magic, "IM4P", 4)) {
        warningf ("image4_parse_im4p: provided buffer is not an IM4P.\n");
        return NULL;
    }

    int elems = asn1_elements_in_object (buf);

    /* get and set the component type */
    if (--elems > 0)
        im4p->comp = asn1_get_string_from_tag ((asn1_tag_t *) asn1_element_at_index (buf, 1));

    /* get and set the descriptor */
    if (--elems > 0)
        im4p->desc = asn1_get_string_from_tag ((asn1_tag_t *) asn1_element_at_index (buf, 2));

    /* get and set the payload size */
    asn1_tag_t *data = (asn1_tag_t *) asn1_element_at_index (buf, 3);
    if (data->tag_number != kASN1TagOCTET) {
        warningf ("image4_parse_im4p: skipped an unexpected tag where an OCTETSTRING was expected\n");
    } else {
	    im4p->size = asn1_len((char *) data + 1).data_len;
    }

    //NOTE: Should also set im4p->offset here.

    /* check if the payload is compressed (if it's encrypted, the flag can be set during decryption */
    im4p->flags |= image4_get_compression_type (buf);

    /* check, get and set the KBAG value */
    char *kbag_octet = (char *) asn1_element_at_index (buf, 4);
    if (kbag_octet != NULL && ((asn1_tag_t *) kbag_octet)->tag_number == kASN1TagOCTET) {

        /* set the im4p flag that the payload is encrypted */
        im4p->flags |= IM4P_FLAG_FILE_ENCRYPTED;
        im4p->flags |= IM4P_FLAG_INCLUDES_KBAG;

        /* get the length of the kbag tag octet */
        asn1_elem_len_t octet_len = asn1_len (++kbag_octet);
        kbag_octet += octet_len.size_bytes;

        /* parse the kbag */
        for (int i = 0; i < asn1_elements_in_object (kbag_octet); i++) {

            /* create the kbag struct */
            kbag_t *kbag = calloc (1, sizeof (kbag_t));

            /* pick the value */
            char *seq = (char *) asn1_element_at_index (kbag_octet, i);
            int elems = asn1_elements_in_object (seq);

            /**
             *  With KBAGs, we often get multiple types: PRODUCTION and DEVELOPMENT. Typically the
             *  KBAG marked PRODUCTION is the first one. This code will parse however many KBAGs
             *  are contained here, and mark the first two accordingly. Any extras are marked as
             *  UNKNOWN.
             */
            if (elems--) {
                asn1_tag_t *num = (asn1_tag_t *) asn1_element_at_index (seq, 0);

                if (num->tag_number == kASN1TagINTEGER) {
                    int j;

                    /* set the kbag type, depending on i */
                    if (i == 0) kbag->type = IMAGE4_KBAG_PRODUCTION;
                    else if (i == 1) kbag->type = IMAGE4_KBAG_DEVELOPMENT;
                    else kbag->type = IMAGE4_KBAG_UNKNOWN;

                    /**
                     *  Parse the KBAG IV. Load the tag from the KBAG sequence, get the length
                     *  and string for the IV, and write byte-by-byte into the iv buffer in the
                     *  kbag struct.
                     */
                    asn1_tag_t *tag_iv = asn1_element_at_index (seq, 1);
                    asn1_elem_len_t tag_iv_len = asn1_len ((char *) tag_iv + 1);

                    j = 0;
                    unsigned char *tag_iv_str = (unsigned char *) tag_iv + tag_iv_len.size_bytes + 1;
                    while (tag_iv_len.data_len--) kbag->iv[j] = (uint8_t) *tag_iv_str++, j++;


                    /**
                     *  Parse the KBAG Key. This is the same process as with the IV. The only difference
                     *  between the two is that the key is 32 bytes and the IV is only 16 bytes.
                     */
                    asn1_tag_t *tag_key = asn1_element_at_index (seq, 2);
                    asn1_elem_len_t tag_key_len = asn1_len ((char *) tag_key + 1);

                    j = 0;
                    unsigned char *tag_key_str = (unsigned char *) tag_key + tag_key_len.size_bytes + 1;
                    while (tag_key_len.data_len--) kbag->key[j] = (uint8_t) *tag_key_str++, j++;

                } else {
                    /* we didn't expect this tag here */
                    warningf ("image4_parse_im4p: skipping unexpected tag.\n");
                }
            }

            /* Add the KBAG to the im4p */
            im4p->kbags = h_slist_append (im4p->kbags, kbag);
        }

    } else {
        // no kbag
        im4p->flags |= IM4P_FLAG_FILE_NOT_ENCRYPTED;
        im4p->flags |= IM4P_FLAG_NO_KBAG;
    }

    return im4p;
}


im4m_t *
image4_parse_im4m (unsigned char *buf)
{
    im4m_t *im4m = calloc (1, sizeof (im4m_t));
    char *magic;
    size_t l;


    /**
     *  The IM4M is an ASN1 Sequence. There are 5 elements in this sequence, the first
     *  being the Image4 tag, which should be "IM4M" here.
     */
    asn1_get_sequence_name (buf, &magic, &l);
    if (strncmp (magic, "IM4M", l)) {
        warningf ("image4_parse_im4m: provided buffer is not an IM4M\n");
        return NULL;
    }

    /**
     *  The second element in `buf` should be the IM4M version.
     */
    im4m->version = asn1_get_number_from_tag ((asn1_tag_t *) asn1_element_at_index (buf, 1));
    debugf ("im4m->version: %d\n", im4m->version);

    /**
     *  The third element is an ASN1 SET containing the manifest. 
     * 
     *  This SET contains a private tag. The private tag then contains a string, identifying
     *  it as a "MANB", or manifest body. And another SET containing however many manifest
     *  entries there are.
     * 
     *  First we want to verify that this third element is a SET, that the SET contains a private
     *  tag, and that the private tag is a "MANB".
     */
    asn1_tag_t *manifest_set = (asn1_tag_t *) asn1_element_at_index (buf, 2);
    if (manifest_set->tag_number != kASN1TagSET) {
        errorf ("image4_parse_im4m: expected a kASN1TagSET as the third element in the IM4M.\n");
        return NULL;
    }

    /* skip over the private tag */
    size_t sb;
    asn1_tag_t *priv_tag = manifest_set + asn1_len ((char *) manifest_set + 1).size_bytes + 1;

    debugf ("priv_tag tagnum: %d, sizebytes: %d\n", asn1_get_private_tagnum (priv_tag, &sb), sb);

    /**
     *  This locates the start of the manifest SEQUENCE, skipping over the private tag.
     *  Contained in this tag should be two elements - the "MANB" string, and a SET
     *  containing all the manifest entries.
     */
    asn1_tag_t *manifest_sequence = (asn1_tag_t *) priv_tag + sb;
    manifest_sequence += asn1_len (manifest_sequence).size_bytes;
   
    debugf ("manifest_sequence->tag_number: %d\n", manifest_sequence->tag_number);
    debugf ("manifest_sequence: sequence_count: %d\n", asn1_elements_in_object (manifest_sequence));

    /* verify that manifest_sequence contains the "MANB" string */
    asn1_get_sequence_name (manifest_sequence, &magic, &l);
    if (strncmp (magic, "MANB", l)) {
        errorf ("image4_parse_im4m: expected \"MANB\", got \"%s\"\n", magic);
    }

    /**
     *  After the "MANB" string, another SET contains a collection of SETs that each
     *  have a list of manifest entries.
     * 
     * 
     */
    asn1_tag_t *manifest_body = asn1_element_at_index (manifest_sequence, 1);
    int manifest_body_elems = asn1_elements_in_object (manifest_body);

    HSList *manifest_list = NULL;

    int skip = 9;
    for (int i = 0; i < manifest_body_elems; i++) {

        size_t sb;
        manifest_t *manifest = calloc (1, sizeof (manifest));

        /**
         *  This loop goes through each of the SEQUENCE's in the IM4M Manifest Body
         *  SET.
         * 
         *  In this set we have a collection of private tags, these tags contain manifests,
         *  and manifest entries. The first thing to do here is to work out the name for the
         *  manifest, and then we can parse all the entries.
         * 
         *  Start by getting the private tag at `i`, and then working out the pointer for
         *  the SEQUENCE tag directly after it.
         */
        asn1_tag_t *priv_tag = (asn1_tag_t *) asn1_element_at_index (manifest_body, i);
        //int skip = asn1_len ((char *) priv_tag + 1).size_bytes + 4;
        asn1_tag_t *sequence = priv_tag + skip;
        if (i == 0) skip--;
        
        //hexdump ("priv_tag", priv_tag, 64);
        //hexdump ("sequence", sequence, 32);

        /**
         *  This SEQUENCE should contain two elements: an IA5String, and another SET containg
         *  all the manifest entries.
         */
        int sequence_elems = asn1_elements_in_object (sequence);
        if (sequence_elems < 2) {
            errorf ("image4_parse_im4m: expected more than two elements in sequence: %d\n", sequence_elems);
            continue;
        }

        /**
         *  We can now set the manifest name
         */
        asn1_tag_t *manifest_name_tag = (asn1_tag_t *) asn1_element_at_index (sequence, 0);
        manifest->name = asn1_get_string_from_tag (manifest_name_tag);

        /**
         *  And start parsing the manifest entries
         */
        asn1_tag_t *manifest_entries = (asn1_tag_t *) asn1_element_at_index (sequence, 1);
        int entries = asn1_elements_in_object (manifest_entries);
        for (int i = 0; i < entries; i++) {

            /**
             *  Manifest Entries are encoded as a SEQUENCE, with a name and a data field. The
             *  data field is not always an IA5String, so we need to check its tag_number and
             *  work from there.
             * 
             *  The SEQUENCE starts 7 bytes after the priv tag.
             */
            asn1_tag_t *priv = (asn1_tag_t *) asn1_element_at_index (manifest_entries, i);
            //asn1_len ((char *) priv + 1).size_bytes + 2;
            asn1_tag_t *entry_sequence = priv + 7;

            /* Check that at least two elements exist */
            int entry_sequence_elems = asn1_elements_in_object (entry_sequence);
            if (entry_sequence_elems < 2)
                errorf ("entry_sequence_elems < 2: %d\n", entry_sequence_elems);

            /**
             *  Create tags for the name and data fields.
             */
            asn1_tag_t *entry_name_tag = (asn1_tag_t *) asn1_element_at_index (entry_sequence, 0);
            asn1_tag_t *entry_data_tag = (asn1_tag_t *) asn1_element_at_index (entry_sequence, 1);

            /**
             *  Work out the data tag type, and parse based on that.
             */
            char *entry_data = "DATA";
            if (entry_data_tag->tag_number == kASN1TagIA5String) {
                entry_data = asn1_get_string_from_tag (entry_data_tag);
            } else if (entry_data_tag->tag_number == kASN1TagOCTET) {
                
                /**
                 *  An OCTET string is encoded in the file like:
                 * 
                 *      04 20 XX XX XX
                 * 
                 *  0x04 being the identifier for an OCTET string, and 0x20 being the size.
                 */

                asn1_elem_len_t str_len = asn1_len (entry_data_tag + 1);
                unsigned char *string = (unsigned char *) entry_data_tag + str_len.size_bytes + 1;
                
                char tmp[4];
                char *octet = calloc (1, str_len.data_len + 1);
                while (str_len.data_len--) {
                    snprintf (tmp, 4, "%02x", *string++);
                    octet = mstrappend ("%s%s", octet, tmp);
                }
                octet = strappend (octet, "\0");
                entry_data = octet;

            } else if (entry_data_tag->tag_number == kASN1TagINTEGER) {

                entry_data = calloc (1, 32);
                sprintf (entry_data, "%d", asn1_get_number_from_tag (entry_data_tag));

            } else if (entry_data_tag->tag_number == kASN1TagBOOLEAN) {
                if (*(char *) entry_data_tag + 2 == 0) entry_data = "FALSE";
                else entry_data = "TRUE";
            } else {
                entry_data = "UNKNOWN";
            }

            /* Create the manifest entry struct and add it to the manifest */
            manifest_entry_t *m_entry = calloc (1, sizeof (manifest_entry_t));
            m_entry->name = asn1_get_string_from_tag (entry_name_tag);
            m_entry->data = entry_data;

            manifest->entries = h_slist_append (manifest->entries, m_entry);
        }

        manifest_list = h_slist_append (manifest_list, manifest);
    }

    /* Set */
    im4m->manifests = manifest_list;

    return im4m;
}

//===----------------------------------------------------------------------===//
/*                      Image4 Field Getters                                  */
//===----------------------------------------------------------------------===//

img4type_t
image4_get_file_type (image4_t *image4)
{
    char 	*magic;
    size_t 	 l;

    unsigned char *buffer = malloc (image4->size);
    memcpy (buffer, image4->data, image4->size);

    /**
     *	Get the Image4 magic sequence, and then compare that to the known
     *  types. Return the correct img4type_t.
     */
    asn1_get_sequence_name(image4->data, &magic, &l);
    free (buffer);

    if (!strncmp("IMG4", magic, l)) return IMAGE4_COMP_TYPE_IMG4;
    else if (!strncmp("IM4P", magic, l)) return IMAGE4_COMP_TYPE_IM4P;
    else if (!strncmp("IM4M", magic, l)) return IMAGE4_COMP_TYPE_IM4M;
    else if (!strncmp("IM4R", magic, l)) return IMAGE4_COMP_TYPE_IM4R;
    else goto FAIL;

FAIL:
    warningf ("image4_get_file_type: unexpected tag, got: \"%s\"\n", magic);
    return IMAGE4_COMP_TYPE_UNKNOWN;
}


char *
image4_get_file_type_name (img4type_t type)
{
    if (type == IMAGE4_COMP_TYPE_IMG4) return "IMG4";
    else if (type == IMAGE4_COMP_TYPE_IM4P) return "IM4P";
    else if (type == IMAGE4_COMP_TYPE_IM4M) return "IM4M";
    else if (type == IMAGE4_COMP_TYPE_IM4R) return "IM4R";
    else return "UNKNOWN";
}

char *
image4_get_file_type_description (image4_t *image4)
{
    if (image4->type != IMAGE4_COMP_TYPE_IMG4) {
        return image4_get_file_type_name (image4->type);
    } else {

        HSList *comps = NULL;

        if (image4->flags & IMAGE4_FLAG_INCLUDES_IM4P)
            comps = h_slist_append (comps, "IM4P");
        if (image4->flags & IMAGE4_FLAG_INCLUDES_IM4M)
            comps = h_slist_append (comps, "IM4M");
        if (image4->flags & IMAGE4_FLAG_INCLUDES_IM4R)
            comps = h_slist_append (comps, "IM4R");

        char *ret = "IMG4 (";
        int max = h_slist_length (comps);
        for (int i = 0; i < max; i++) {
            ret = strappend (ret, (char *) h_slist_nth_data (comps, i));
            if ((i+1) != max) ret = strappend (ret, ", ");
        }

        ret = strappend (ret, ")");
        return ret;
    }
}

char *
image4_get_component_type_name (image4_t *image4)
{
    char *magic = image4->im4p->comp;
	if (!strncmp (magic, IMAGE_TYPE_IBOOT, 4))                    return IMAGE_TYPE_IBOOT;
	else if (!strncmp (magic, IMAGE_TYPE_IBEC, 4))                return IMAGE_TYPE_IBEC;
	else if (!strncmp (magic, IMAGE_TYPE_IBSS, 4))                return IMAGE_TYPE_IBSS;
	else if (!strncmp (magic, IMAGE_TYPE_LLB, 4))                 return IMAGE_TYPE_LLB;
	else if (!strncmp (magic, IMAGE_TYPE_SEP_OS, 4))              return IMAGE_TYPE_SEP_OS;
	else if (!strncmp (magic, IMAGE_TYPE_SEP_OS_RESTORE, 4))      return IMAGE_TYPE_SEP_OS_RESTORE;
	else if (!strncmp (magic, IMAGE_TYPE_DEVTREE, 4))             return IMAGE_TYPE_DEVTREE;
	else if (!strncmp (magic, IMAGE_TYPE_RAMDISK, 4))             return IMAGE_TYPE_RAMDISK;
	else if (!strncmp (magic, IMAGE_TYPE_KERNELCACHE, 4))         return IMAGE_TYPE_KERNELCACHE;
	else if (!strncmp (magic, IMAGE_TYPE_LOGO, 4))                return IMAGE_TYPE_LOGO;
	else if (!strncmp (magic, IMAGE_TYPE_RECMODE, 4))             return IMAGE_TYPE_RECMODE;
	else if (!strncmp (magic, IMAGE_TYPE_NEEDSERVICE, 4))         return IMAGE_TYPE_NEEDSERVICE;
    else if (!strncmp (magic, IMAGE_TYPE_GLYPHCHRG, 4))           return IMAGE_TYPE_GLYPHCHRG;
    else if (!strncmp (magic, IMAGE_TYPE_GLYPHPLUGIN, 4))         return IMAGE_TYPE_GLYPHPLUGIN;
	else if (!strncmp (magic, IMAGE_TYPE_BATTERYCHARGING0, 4))    return IMAGE_TYPE_BATTERYCHARGING0;
	else if (!strncmp (magic, IMAGE_TYPE_BATTERYCHARGING1, 4))    return IMAGE_TYPE_BATTERYCHARGING1;
	else if (!strncmp (magic, IMAGE_TYPE_BATTERYLOW0, 4))         return IMAGE_TYPE_BATTERYLOW0;
	else if (!strncmp (magic, IMAGE_TYPE_BATTERYLOW1, 4))         return IMAGE_TYPE_BATTERYLOW1;
	else if (!strncmp (magic, IMAGE_TYPE_BATTERYFULL, 4))         return IMAGE_TYPE_BATTERYFULL;
	else if (!strncmp (magic, IMAGE_TYPE_OS_RESTORE, 4))          return IMAGE_TYPE_OS_RESTORE;
	else if (!strncmp (magic, IMAGE_TYPE_HAMMER, 4))              return IMAGE_TYPE_HAMMER;
    else return "ERROR";
}


char *
image4_get_component_type_description (image4_t *image4)
{
    char *magic;
	size_t l;

	char *raw = asn1_element_at_index (image4->data, 1) + 2;
    debugf ("image4_get_component_type_description: %d\n", image4->type);

	if (!strncmp (raw, "ibot", 4)) {
		return "iBoot";
	} else if (!strncmp (raw, IMAGE_TYPE_IBEC, 4)) {
		return "iBoot Epoch Change (iBEC)";
	} else if (!strncmp (raw, IMAGE_TYPE_IBSS, 4)) {
		return "iBoot Single Stage (iBSS)";
	} else if (!strncmp (raw, IMAGE_TYPE_LLB, 4)) {
		return "Low Level Bootloader (LLB)";
	} else if (!strncmp (raw, IMAGE_TYPE_SEP_OS, 4)) {
		return "Secure Enclave OS (SEP OS)";
	} else if (!strncmp (raw, IMAGE_TYPE_SEP_OS_RESTORE, 4)) {
		return "Secure Enclave OS Restore (SEP OS RESTORE)";
	} else if (!strncmp (raw, IMAGE_TYPE_DEVTREE, 4)) {
		return "Device Tree";
	} else if (!strncmp (raw, IMAGE_TYPE_RAMDISK, 4)) {
		return "Update/Restore Ramdisk";
	} else if (!strncmp (raw, IMAGE_TYPE_KERNELCACHE, 4)) {
		return "Darwin Kernel Cache";
	} else if (!strncmp (raw, IMAGE_TYPE_LOGO, 4)) {
		return "Boot Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_RECMODE, 4)) {
		return "Recovery Mode Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_NEEDSERVICE, 4)) {
		return "Need Service Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_GLYPHCHRG, 4)) {
		return "Glyph Charge Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_GLYPHPLUGIN, 4)) {
		return "Glyph Plugin Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_BATTERYCHARGING0, 4)) {
		return "Battery Charging 0 Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_BATTERYCHARGING1, 4)) {
		return "Battery Charging 1 Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_BATTERYLOW0, 4)) {
		return "Battery Charging Low 0 Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_BATTERYLOW1, 4)) {
		return "Battery Charging Low 1 Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_BATTERYFULL, 4)) {
		return "Battery Full Logo";
	} else if (!strncmp (raw, IMAGE_TYPE_OS_RESTORE, 4)) {
		return "iOS Restore Image";
	} else if (!strncmp (raw, IMAGE_TYPE_HAMMER, 4)) {
		return "Hammer";
	} else {
		return "Unknown Component";
	}
}


int
image4_get_compression_type (char *buf)
{
    /* Get the element count and ensure buf is an im4p */
    int c = asn1_elements_in_object (buf);
    if (c < 4) {
        errorf ("Not enough elements in given payload.\n");
        return -1;
    }

    /* Try to select the payload tag from the buffer */
    char *tag = asn1_element_at_index (buf, 3) + 1;
    asn1_elem_len_t len = asn1_len (tag);
    char *data = tag + len.size_bytes;

    /* Check for either lzss or bvx2/lzfse */
    if (!strncmp (data, "complzss", 8)) {
            return IM4P_FLAG_FILE_COMPRESSED_LZSS;
    } else if (!strncmp (data, "bvx2", 4)) {
            return IM4P_FLAG_FILE_COMPRESSED_BVX2;
    } else {
            return IM4P_FLAG_FILE_COMPRESSED_NONE;
    }
}

char *
image4_get_compression_description (image4_t *image4)
{
    if (image4->im4p->flags & IM4P_FLAG_FILE_ENCRYPTED) return "Encrypted";
    else if (image4->im4p->flags & IM4P_FLAG_FILE_COMPRESSED_LZSS) return "Compressed (LZSS)";
    else if (image4->im4p->flags & IM4P_FLAG_FILE_COMPRESSED_BVX2) return "Compressed (BVX2)";
    else return "Uncompressed, Not Encrypted";
}