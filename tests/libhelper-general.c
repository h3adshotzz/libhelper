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


//
//	THIS IS A TEST PROGRAM FOR LIBHELPER
//
//		Purpose is to test functions of general libhelper functionality, such
//		as:
//			- hslist
//			- file
//			- strutils
//

#include <libhelper/libhelper.h>


//////////////////////////////////////////////////////////////////////////////////////////

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
	test = h_slist_append (test, str_a);
	
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

//////////////////////////////////////////////////////////////////////////////////////////

void _libhelper_file_tests (unsigned char *path)
{
	file_t *test = file_load (path);
	unsigned char *test_str = file_load_bytes (test, test->size, 0);
	printf ("file: %s\n", test_str);
}

//////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
	
	// hslist testing
	_libhelper_hslist_tests ();
	
	// file testing
	if (argc > 1)
		_libhelper_file_tests (argv[1]);
	
	return 0;
}