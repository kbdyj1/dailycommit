#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

char *b;
char buf1[21];

char *__attribute__ ((noinline)) do_set (bool cond)
{

    char *buf = buf1;
    if (cond)
        buf = (char*)malloc (42);

    memset (buf, 0, 22);
    return buf;
}

static void test_memset_chk()
{
    b = do_set(false);
}

static void test_memcpy_chk()
{
    char buf[4];
    strcpy(buf, "Hello, Qt6");
}

void test_fotify_source()
{
    test_memcpy_chk();
}
