#define _BSD_SOURCE
#include <sys/mman.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

const int LEN = 1024 * 1024;
#define SHELL_CMD "cat /proc/%ld/maps | grep zero"
const int CMD_LEN = sizeof(SHELL_CMD) + 20;

//#define DEBUG_CMD

void test()
{
    void* addr = mmap(NULL, LEN, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (MAP_FAILED == addr)
        errorExit("mmap() failed.\n");

    printf("#1 mprotect()\n");
    char cmd[CMD_LEN];
    snprintf(cmd, CMD_LEN, SHELL_CMD, (long)getpid());

#ifdef DEBUG_CMD
    printf("%s\n", cmd);
#else
    system(cmd);
#endif

    if (-1 == mprotect(addr, LEN, PROT_READ | PROT_WRITE))
        errorExit("mprotect() error.\n");

    printf("#2 mprotect()\n");
#ifdef DEBUG_CMD
    printf("%s\n", cmd);
#else
    system(cmd);
#endif

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void printMinCore(char* addr, size_t len)
{
    long pageSize = sysconf(_SC_PAGESIZE);
    printf("Page size: %ld\n", pageSize);

    long pages = (len + pageSize - 1) / pageSize;
    unsigned char* vec = (unsigned char*)malloc(pages);
    if (NULL == vec)
        errorExit("malloc(pages) error.\n");

    if (-1 == mincore(addr, len, vec)) {
        errorExit("mincore() error.\n");
    }

    for (int i=0; i<pages; i++) {
        if (i % 64 == 0) {
            printf("%s%10p: ", (i == 0) ? "" : "\n", addr + (i * pageSize));
        }
        printf("%c", (vec[i] & 1) ? '*' : '.');
    }
    printf("\n");

    free(vec);
}

void test(int argc, const char* argv[])
{
    printf("RLIMIT_MEMLOCK: %d pages.\n", RLIMIT_MEMLOCK);
    if (4 != argc) {
        fprintf(stderr, "usage: %s num-pages lock-page-step lock-page-len\n", argv[0]);
    }

    long pageSize = sysconf(_SC_PAGESIZE);
    if (-1 == pageSize)
        errorExit("sysconf(_SC_PAGE_SIZE): -1\n");

    long len = atoi(argv[1]) * pageSize;
    long step = atoi(argv[2]) * pageSize;
    long lockLen = atoi(argv[3]) * pageSize;

    char* addr = (char*)mmap(NULL, len, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (MAP_FAILED == addr)
        errorExit("mmap() error.\n");

    printf("Allocated %ld (%#lx) bytes starting at %p\n", len, len, addr);

    printf("Before mlock:\n");
    printMinCore(addr, len);

    for (int i=0; i+lockLen <= len; i+=step) {
        if (-1 == mlock(addr + i, lockLen))
            errorExit("mlock() error.\n");
    }

    printf("After mlock:\n");
    printMinCore(addr, len);

    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_13(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
#endif

    _2::test(argc, argv);
}
