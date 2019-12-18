#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#include <libhelper/hslist.h>

void print_list (HSList *list)
{
    for (int i = 0; i < h_slist_length (list); i++) {
        char *test = (char *) h_slist_nth_data (list, i);
        printf ("[%d]: %s\n", i, test);
    }
}

void hlibc_hslist_test ()
{
    printf ("Libhelper: Testing HSList\n");

    HSList *list = NULL;

    char *test_data = "test data 1";
    list = h_slist_append (list, test_data);

    test_data = "test data 2";
    list = h_slist_append (list, test_data);

    print_list (list);

    test_data = "test data 3";
    list = h_slist_append (list, test_data);

    print_list (list);
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#include <libhelper/hstring.h>

void hlibc_hstring_test ()
{
    HString *str = h_string_new ("Hello World!");
    printf ("test 1: %s\n", str->str);

    str = h_string_append_c (str, 'a');
    str = h_string_append_c (str, 'b');
    str = h_string_append_c (str, 'c');

    printf ("test 2: %s\n", str->str);
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

int main ()
{

    hlibc_hstring_test ();

    return 0;
}