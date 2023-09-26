#define __STDC_WANT_LIB_EXT1__  1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define USE_FGETS
#define USE_GETLINE

static void get_y_or_n_with_fgets()
{
    char res[8];
    size_t len = sizeof(res);
    puts("continue? [y] n: ");
    char* ret = fgets(res, len, stdin);
    puts("\ninput is: ");
    puts(ret);

    if ('n' == res[0]) {
        exit(0);
    } else {
        exit(-1);
    }
}

static void get_y_or_n_with_getline()
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

static void test_fgets()
{
    char buf[10];
    int c;
    char* p;

    if (fgets(buf, sizeof(buf), stdin)) {
        p = strchr(buf, '\n');
        if (p) {
            *p = '\0';
        } else {
            while (((c = getchar()) != '\n') && !feof(stdin) && !ferror(stdin))
                ;
        }
        printf("\nentered: %s\n", buf);
    } else {
        fprintf(stderr, "fgets() return NULL.\n");
    }
}

static void test_getline()
{
    const size_t BufferSize = 10;
    char* buf = malloc(BufferSize);
    ssize_t size;

    if (-1 == (size = getline(&buf, &BufferSize, stdin))) {
        fprintf(stderr, "getline() error.\n");
    } else {
        char* p = strchr(buf, '\n');
        if (p) {
            *p = '\0';
        } else {
            char c;
            while ((c = getchar()) != '\n' && !feof(stdin) && !ferror(stdin))
                ;
        }
    }

    fprintf(stdout, "\ninput: %s\n", buf);

    free(buf);
}

static void test()
{
    //get_y_or_n();
    //test_fgets();
    test_getline();
}

void test_gets()
{
    test();
}
