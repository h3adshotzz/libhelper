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

/* libhelper-core: tests of version, file and hlibc */

#include <stdio.h>

#include <libhelper.h>
#include <libhelper-file.h>
#include <libhelper-hlibc.h>

#include <libhelper-asn1.h>


char *asn1_convert_octet_string (asn1_tag_t *tag)
{
    asn1_elem_len_t len = asn1_len ((char *) tag + 1);
    unsigned char *string = (unsigned char *) tag + len.size_bytes + 1;

    if (len.data_len < 32) {
        while (len.data_len--) printf ("%02x", *string++);
    } else {
        for (int i = 0; i < 32; i++) printf ("%02x", *string++);
    }

    return NULL;
}


void asn1_tag_debug (asn1_tag_t *tag)
{
    debugf ("tag->is_constructed: %d\n", tag->is_constructed);
    debugf ("tag->tag->tag_class: %d\n", tag->tag_class);
    debugf ("tag->tag->tag_number: %d\n", tag->tag_number);
}

void asn1_cycle_object (asn1_tag_t *object, char *padding)
{
    /* print the type of object we're handling */
    if (object->tag_number == kASN1TagSEQUENCE) printf ("%sSEQUENCE: ", padding);
    else if (object->tag_number == kASN1TagSET) printf ("%sSET: ", padding);
    else  printf ("%sUNKNOWN TYPE: [%d]", padding, object->tag_number);
        
    /* check and print the amount of elements in the object */
    int elems = asn1_elements_in_object ((const char *) object);
    printf ("(%d elems)\n", elems);

    /* adjust padding string by 4 characters */
    padding = strappend (padding, "    ");

    /* cycle through elements in the object */
    for (int i = 0; i < elems; i++) {
        asn1_tag_t *tag = asn1_element_at_index ((const char *) object, i);
        
        if (tag->tag_number == kASN1TagEnd_of_Content) {

            /* the kASN1TagEnd_of_Content tag can still contain an object,
             so if there is one we want to call asn1_cycle_object again */
            int eoc_elems = asn1_elements_in_object (tag);
            for (int i = 0; i < eoc_elems; i++) {
                printf ("%s[%d] (%d elems)\n", padding, i, eoc_elems);

                /* adjust padding string by 4 characters */
                padding = strappend (padding, "    ");
                asn1_cycle_object (tag + 4, padding);
            }

        } else if (tag->tag_number == kASN1TagSEQUENCE) {

            /* easy, just call asn1_cycle_object on tag */
            asn1_cycle_object (tag, padding);

        } else if (tag->tag_number == kASN1TagSET) {

            /* sets are similar to sequences, so they can be handled by the
                same function. */
            asn1_cycle_object (tag, padding);

        } else if (tag->tag_number == kASN1TagIA5String) {

            /* calculate tag length and print IA5String */
            asn1_elem_len_t len = asn1_len ((char *) ++tag);
            printf ("%sIA5String: %.*s\n", padding, (int) len.data_len, ((char *) tag) + len.size_bytes);

        } else if (tag->tag_number == kASN1TagOCTET) {

            /* calc length and get string pointer */
            asn1_elem_len_t len = asn1_len ((char *) tag + 1);
            unsigned char *string = (unsigned char *) tag + len.size_bytes + 1;

            /* print octet string. If the string is more than 32 bytes, just
                print the first 32. Otherwise print the whole thing */
            printf ("%sOCTET STRING (%d bytes) ", padding, len.data_len);
            if (len.data_len < 32) while (len.data_len--) printf ("%02x", *string++);
            else for (int i = 0; i < 32; i++) printf ("%02x", *string++);
            printf ("\n");

            /* octet strings may have additional objects within it, so check
                if there's a sequence embedded */
            asn1_tag_t *octet_tag = tag + 2;
            if (octet_tag->tag_number == kASN1TagSEQUENCE) {
                padding = strappend(padding, "    ");

                /* check number of elements, adjust padding and call cycle object */
                int octet_elems = asn1_elements_in_object (octet_tag);
                printf ("%sSEQUENCE: (%d elems)\n", padding, octet_elems);

                for (int i = 0; i < octet_elems; i++) {
                    asn1_tag_t *tmp_tag = asn1_element_at_index (octet_tag, i);
                    asn1_cycle_object (tmp_tag, strappend(padding, "    "));
                }


            }

        } else if (tag->tag_number == kASN1TagINTEGER) {
            
            /* get and print integer */
            printf ("%sINTEGER: %d\n", padding, asn1_get_number_from_tag (tag));

        } else if (tag->tag_number == 31) {     /* PRIVATE */

            printf ("%sPRIVATE N/A\n", padding);

            asn1_tag_t *child = (asn1_tag_t *) tag + 10;
            int e = asn1_elements_in_object ((const char *) child);
            int te = asn1_elements_in_object ((const char *) tag);
            
            printf ("private (%d), elems: %d\n", tag->tag_number, te);
            printf ("\tchild (%d), elems: %d\n", child->tag_number, e);
            //asn1_cycle_object (child, strappend (padding, "    "));

        } else {
            debugf ("%stag->tag->tag_number: %d\n", padding, tag->tag_number);
        }
    }
}

int main (int argc, char **argv)
{
    char *file = argv[1];
    printf ("Loaded file: %s\n", file);
    printf ("--------------------------------\n");

    file_t *f = file_load (file);

    asn1_cycle_object ((asn1_tag_t *) f->data, "");
    return 0;
}