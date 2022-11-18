//=============================================================================
//  ch.01 Fundations
//
//  2022/11/17 ~
//=============================================================================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD(NAME)                   \
    char NAME ## _cmd[256] = "";    \
    strcpy(NAME ## _cmd, #NAME);

static void test_macro()
{
    CMD(copy)
    CMD(paste)
    CMD(cut)

    char cmd[256];
    scanf("%s", cmd);

    if (0 == strcmp(cmd, copy_cmd)) {
        printf("copy!\n");
    } else if (0 == strcmp(cmd, paste_cmd)) {
        printf("paste!\n");
    } else if (0 == strcmp(cmd, cut_cmd)) {
        printf("cut!\n");
    }
}

#define VERSION "1.0.0"

#define LOG_ERROR(format, ...)  \
    fprintf(stderr, format, __VA_ARGS__)

static void test_va_args()
{
    LOG_ERROR("%s version %s\n", "expert C", VERSION);
}

static void test_ptr_arithmetic()
{
    int i = 0;
    int* pi = &i;
    char c = '0';
    char* pc = &c;

    printf("#0 int*: %u, char*: %u\n", (unsigned int)pi, (unsigned int)pc);

    pi++;
    pc++;

    printf("#1 int*: %u, char*: %u\n", (unsigned int)pi, (unsigned int)pc);
}

static void print_bytes(void* data, size_t len)
{
    char delim = ' ';
    unsigned char* p = data;

    printf("[%d] ", len);

    for (size_t i = 0; i < len; i++) {
        printf("%c 0x%x", delim, *p);
        delim = ',';
        p++;
    }
    printf("\n");
}

static void test_void_ptr()
{
    int i = 99;
    double d = 6.4;

    print_bytes(&i, sizeof(i));
    print_bytes(&d, sizeof(d));
}

static void test_pointer_size()
{
    printf("pointer size: %lu", sizeof(char*));
}

static int* bad_ptr()
{
    int local = 0;

    return &local;
}

static void test_segmentation_fault()
{
    int* ptr = bad_ptr();

    printf("bad int: %d", *ptr);
}

//------------------------------------------------------------------- memory layout

#if (1)
typedef struct {
    char c0;
    char c1;
    char c2;
    short s;
} sample_t;
#else
struct __attribue__ ((__packed__)) sample_t {
    char c0;
    char c1;
    char c2;
    short s;
};
typedef struct sample_t sample_t;
#endif

static void print_sample_size(sample_t* p)
{
    printf("size: %lu bytes.\n", sizeof(*p));
}

static void test_struct_size()
{
    sample_t s;
    s.c0 = 'A';
    s.c1 = 'B';
    s.c2 = 'C';
    s.s = 765;

    print_sample_size(&s);
    print_bytes(&s, sizeof(sample_t));
}

//=============================================================================

void test_ch_01()
{
#if (0) // done
    test_macro();
    test_va_args();
    test_ptr_arithmetic();
    test_void_ptr();
    test_pointer_size();
    test_segmentation_fault();
#endif

    test_struct_size();
}
