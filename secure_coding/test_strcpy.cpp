#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

static void test_memmove()
{
    char buf[16] = "123456789";
    char* dest = buf;
    char* src = &buf[4];

#if (0)
    memmove(dest, src, 6);
#else
    memmove(src, dest, 6);  // memory overlap
#endif

    printf("memmove: %s\n", buf);
}

static void test_strnlen()
{
#if (0)
    char str[] = "Hello, Qt6";
#else
#   define str "Hello, Qt6"
#   define str2 "Secure coding."
#endif

    size_t len = strnlen(str, 64);

    printf("strnlen(): %ld\n", len);
}

static void test_builtin_object_size()
{
    struct V {
        char buf[10];
        int i;
        char message[10];
    };
    V v;

    void* p = &v.i;

#ifdef __GNUC__
    size_t s0 = __builtin_object_size(p, 0);
    printf("__builtin_object_size(, 0): %ld\n", s0);

    size_t s1 = __builtin_object_size(p, 1);
    printf("__builtin_object_size(, 1): %ld\n", s1);
#endif
}

void test_strcpy()
{
#if (0) //done
    test_null_terminated();
    test_strcat();
    test_memmove();
    test_strnlen();
#endif
    test_builtin_object_size();
}
