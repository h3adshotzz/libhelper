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

file_t *file_create ()
{
	file_t *file = g_new0 (file_t, 1);
	return file;
}


file_t *file_load (const char *path)
{
	g_autoptr (GError) error = NULL;
	file_t *file = file_create ();

	/* set the file path */
	if (!path) {
		//errorf ("File path is not valid\n");
		return NULL;
	}
	file->path = g_strdup (path);

	/* load the file */
	file->data = g_mapped_file_new (file->path, FALSE, &error);
	if (!file->data) {
		/* maybe do something with error here? */
		//errorf ("File could not be loaded\n");
		return NULL;
	}
	
	if (g_mapped_file_get_length (file->data)) {
		return file;
	} else {
		g_clear_pointer (&file, file_free);
		return NULL;
	}
}


void file_free (file_t *file)
{
	g_clear_pointer (&file->path, g_free);
	g_clear_pointer (&file->data, g_mapped_file_unref);
	g_free (file);
}


int file_write_new (char *filename, unsigned char *buf, size_t size)
{
	FILE *f = fopen (filename, "wb");
	if (!f)
		return LH_FILE_FAILURE;
	
	size_t res = fwrite (buf, sizeof (char), size, f);
	fclose (f);

	return res;
}


/**
 * file_get_data:
 * @f: the #file_t
 * 
 * Returns: the length of @f's data
 */
size_t
file_get_length (file_t *f)
{
	g_return_val_if_fail (f != NULL && f->data != NULL, 0);

	return g_mapped_file_get_length (f->data);
}



/**
 * file_get_data:
 * @f: the #file_t to read from
 * @offset: where in the file to start
 * 
 * Returns: (transfer none): the contents starting at @offset
 */
const void *
file_get_data (file_t *f, uint32_t offset)
{
	g_return_val_if_fail (f != NULL && f->data != NULL, NULL);

	return g_mapped_file_get_contents (f->data) + offset;
}


/**
 * file_read_data:
 * @f: the #file_t to read from
 * @offset: where in the file to start
 * @buf: (out caller-allocates): where to memcpy() to
 * @size: how much to read
 */
void
file_read_data (file_t *f, uint32_t offset, void *buf, size_t size)
{
	g_return_if_fail (f != NULL && f->data != NULL);
	g_return_if_fail (g_mapped_file_get_length (f->data) <= offset + size);

	memcpy (buf, file_get_data (f, offset), size);
}


/**
 * file_dup_data:
 * @f: the #file_t to read from
 * @offset: where in the file to start
 * @size: how much to read
 * 
 * Returns: (transfer full): the contents
 */
void *
file_dup_data (file_t *f, uint32_t offset, size_t size)
{
	void *buf = malloc (size);

	file_read_data (f, offset, buf, size);

	return buf;
}
