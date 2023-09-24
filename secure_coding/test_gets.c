#define __STDC_WANT_LIB_EXT1__  1
#include <stdio.h>
#include <stdlib.h>

//#define USE_FGETS
#define USE_GETLINE

#if defined(USE_FGETS)
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
#endif

#if defined(USE_GETLINE)
static void get_y_or_n()
{
    char* res = NULL;
    size_t len;

    puts("continue? [y] n: ");
    int exit_code = 0;
    if ((0 > getline(&res, &len, stdin)) || (len && res[0] == 'n')) {
        printf("len: %ld\n", len);
    } else {
        exit_code = -1;
    }

    free(res);
    exit(exit_code);
}
#endif

static void test()
{
    get_y_or_n();
}

void test_gets()
{
    test();
}
