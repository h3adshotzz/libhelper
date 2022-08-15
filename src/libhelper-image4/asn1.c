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

#include "libhelper-asn1.h"
#include "libhelper-image4.h"


asn1_elem_len_t asn1_len (const char *buf)
{
	asn1_length_t *tmp = (asn1_length_t *)buf;
	size_t outsize = 0;
	int bytes = 0;

	unsigned char *sbuf = (unsigned char *)buf;

	if (!tmp->is_long) {
		outsize = tmp->len;
	} else {
		bytes = tmp->len;
		for (int i = 0; i < bytes; i++) {
			outsize *= 0x100;
			outsize += sbuf[1+i];
		}
	}

	asn1_elem_len_t ret;
	ret.data_len = outsize;
	ret.size_bytes = bytes + 1;

	return ret;
}


uint64_t asn1_get_number_from_tag(asn1_tag_t *tag)
{
		if (!tag) {
				printf ("[Error] data is not an asn1 type\n");
				return 0;
		}

    if (tag->tag_number != kASN1TagINTEGER) {
        printf ("[Error] Not an integer\n");
        exit(1);
    }

    uint64_t ret = 0;
    asn1_elem_len_t len = asn1_len((char *) ++tag);
    
    unsigned char *data = (unsigned char *) tag + len.size_bytes;
    
    while (len.data_len--) {
        ret *= 0x100;
        ret+= *data++;
    }
    
    return ret;
}

char *
asn1_get_string_from_tag (asn1_tag_t *tag)
{
    char *str = 0;
    size_t len;

    asn1_get_string ((char *) tag, &str, &len);
    if (len > 32) len = len + (32 - len);

    char *ret = calloc (1, len);
    memcpy (ret, str, len);

    return (ret) ? ret : NULL;
}

size_t asn1_get_private_tagnum (asn1_tag_t *tag, size_t *size_bytes)
{
	if (*(unsigned char *) tag != 0xff) {
		printf ("[Error] Not a private TAG 0x%02x\n", *(unsigned int *) tag);
		exit(1);
	}

	size_t sb = 1;
	asn1_elem_len_t taglen = asn1_len((char *)++tag);
	taglen.size_bytes -= 1;

	if (taglen.size_bytes != 4) {
		/* 
         WARNING: seems like apple's private tag is always 4 bytes long
         i first assumed 0x84 can be parsed as long size with 4 bytes, 
         but 0x86 also seems to be 4 bytes size even if one would assume it means 6 bytes size.
         This opens the question what the 4 or 6 nibble means.
        */
		taglen.size_bytes = 4;
	}

	size_t tagname = 0;
	do {
		tagname *= 0x100;
		tagname >>= 1;
		tagname += ((asn1_private_tag_t *)tag)->num;
		sb++;
	} while (((asn1_private_tag_t *) tag++)->more);

	if (size_bytes) *size_bytes = sb;
	return tagname;
}


char *asn1_get_string (char *buf, char **outstring, size_t *strlen)
{
	asn1_tag_t *tag = (asn1_tag_t *)buf;

	if (!(tag->tag_number | kASN1TagIA5String)) {
		printf("[Error] not a string\n");
		return 0;
	}

	asn1_elem_len_t len = asn1_len(++buf);
	*strlen = len.data_len;
	buf += len.size_bytes;
	if (outstring) *outstring = buf;

	return buf +* strlen;
}


int asn1_element_at_index_with_counter(const char *buf, int index, asn1_tag_t **tagret)
{
    int ret = 0;
    
    if (!((asn1_tag_t *)buf)->is_constructed) {
		return 0;
	}
    asn1_elem_len_t len = asn1_len(++buf);
    
    buf += len.size_bytes;
    
	/* TODO: add lenght and range checks */
    while (len.data_len) {
        if (ret == index && tagret){
            *tagret = (asn1_tag_t *) buf;
            return ret;
        }
        
        if (*buf == kASN1TagPrivate) {
            
			size_t sb;
            asn1_get_private_tagnum((asn1_tag_t *) buf, &sb);
            
			buf += sb;
            len.data_len -= sb;

        } else if (*buf == (char)0x9F){
            
			//buf is element in set and it's value is encoded in the next byte
            asn1_elem_len_t l = asn1_len(++buf);
            if (l.size_bytes > 1) l.data_len += 0x80;
            buf += l.size_bytes;
            len.data_len -= 1 + l.size_bytes;
        
		} else
            buf++,len.data_len--;
        
        asn1_elem_len_t sublen = asn1_len(buf);
        size_t toadd = sublen.data_len + sublen.size_bytes;
        len.data_len -= toadd;
        buf += toadd;
        ret ++;
    }
    
    return ret;
}


char* asn1_element_at_index (const char *buf, int index)
{
	asn1_tag_t *ret;
	asn1_element_at_index_with_counter(buf, index, &ret);
	return (char*) ret;
}


int asn1_elements_in_object (const char *buf)
{
	return asn1_element_at_index_with_counter (buf, -1, NULL);
}


int asn1_get_sequence_name (const char *buf, char **name, size_t *namelen)
{
	int err = 0;

	if (((asn1_tag_t *) buf)->tag_number != kASN1TagSEQUENCE) {
		printf("not a sequence\n");
		return err;
	}

	int elems = asn1_elements_in_object (buf);
	if (!elems) {
		printf("no elements in sequence\n");
		return err;
	}

	size_t len;
	asn1_get_string (asn1_element_at_index (buf, 0), name, &len);
	if (namelen) *namelen = len;

	return err;
}
