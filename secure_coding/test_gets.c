#define __STDC_WANT_LIB_EXT1__  1
#include <stdio.h>
#include <stdlib.h>

static void get_y_or_n()
{
    char res[8];
    size_t len = sizeof(res);
    puts("continue? [y] n: ");
    char* ret = fgets(res, len+1, stdin);
    puts("\ninput is: ");
    puts(ret);

    if ('n' == res[0]) {
        exit(0);
    } else {
        exit(-1);
    }
}

static void test()
{
    //constraint_handler_t constraint = set_constraint_handler_s(ignore_handler_s);

    get_y_or_n();
}

void test_gets()
{
    test();
}
