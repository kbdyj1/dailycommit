#define _BSD_SOURCE
#include <sys/mman.h>
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

} //namespace =================================================================

void exec_ch_13()
{
    _1::test();
}
