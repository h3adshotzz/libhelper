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

/**
 *  HSList tests
 */
void _libhelper_hslist_test_print_list (HSList *list)
{
	printf ("sizeof: %x\n", h_slist_length (list));
	for (int i = 0; i < (int) h_slist_length (list); i++) {
		char *test = (char *) h_slist_nth_data (list, i);
		printf ("[%x]: %s\n", i, test);
	}
}
int _libhelper_hslist_tests ()
{
	char *str_a = "this is a test a";
	char *str_b = "this is a test b";
	char *str_c = "this is a test c";
	
	// create the list
	HSList *test = NULL;
	
	// add two element
	test = h_slist_append (test, str_a);
	test = h_slist_append (test, str_b);
	
	// print the current standing
	_libhelper_hslist_test_print_list (test);
	
	// add another element
	test = h_slist_append (test, str_c);
	
	// print the current standing
	_libhelper_hslist_test_print_list (test);
	
	// grab the last element in the list
	printf ("list last elem: %s\n", (char *) h_slist_last (test));
	
	// remove two elements from the list
	test = h_slist_remove (test, str_a);
	test = h_slist_remove (test, str_c);
	
	// print the current standing
	_libhelper_hslist_test_print_list (test);
	
	return 1;
}

/**
 *  HString tests
 */
void _libhelper_hstring_tests ()
{
	HString *test = h_string_new ("test");
	printf ("test: %s\n", test->str);
	
	char *test2 = mstrappend ("%s%s", "a", "b");
	printf ("test2: %s\n", test2);
}

/**
 *  File API tests
 */
void _libhelper_file_tests (char *path)
{
	file_t *test = file_load ((const char *) path);
	if (test)
		printf ("success: %d\n", test->size);
}

///////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
    /* libhelper version */
    printf ("version: %s\n", libhelper_get_version_string());

    /* HSList testing */
    _libhelper_hslist_tests ();

    /* HString testing */
    _libhelper_hstring_tests ();

    /* File API testing */
    if (argc > 1)
        _libhelper_file_tests (argv[1]);

    return 0;
}