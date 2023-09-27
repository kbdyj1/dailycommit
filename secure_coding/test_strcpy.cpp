#include <string.h>
#include <stdio.h>

static void test_null_terminated()
{
    char buf[8];
    const char* src = "Hello, Qt6";
    strncpy(buf, src, 8);

    printf("strncpy -> buf[7] : %c\n", buf[7]);
}

static void test_strcat()
{
    char buf[16] = "Hello";
    strncat(buf, " Qt6.123456789", sizeof(buf)-strlen(buf)-1);

    printf("strncat -> %s\nbuf[15] : %d\n", buf, (int)buf[15]);
    printf("strncat -> %s\nbuf[16] : %d\n", buf, (int)buf[16]);
}

void test_strcpy()
{
#if (0) //done
    test_null_terminated();
#endif

    test_strcat();
}
