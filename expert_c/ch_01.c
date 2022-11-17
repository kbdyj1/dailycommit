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

void test_macro()
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

void test_va_args()
{
    LOG_ERROR("%s version %s\n", "expert C", VERSION);
}

void test_ptr_arithmetic()
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

void print_bytes(void* data, size_t len)
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

void test_void_ptr()
{
    int i = 99;
    double d = 6.4;

    print_bytes(&i, sizeof(i));
    print_bytes(&d, sizeof(d));
}

//=============================================================================

void test_ch_01()
{
#if (0) // done
    test_macro();
    test_va_args();
    test_ptr_arithmetic();
#endif

    test_void_ptr();
}
