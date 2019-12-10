#include <stdio.h>
#include <hslist.h>

void print_list (HSList *list)
{
    for (int i = 0; i < h_slist_length (list); i++) {
        char *test = (char *) h_slist_nth_data (list, i);
        printf ("[%d]: %s\n", i, test);
    }
}

int main ()
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

    return 0;
}