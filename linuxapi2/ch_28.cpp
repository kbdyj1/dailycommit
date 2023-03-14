#define _BSD_SOURCE

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/acct.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

#if (1)
#   define PACCT   "/var/log/pacct"    //-> $? 1
#else
#   define PACCT   "usr/account/pacct" //-> $? 0
#endif

void turnOnAcct(bool on)
{
    int ret = on ? acct(PACCT) : acct(NULL);

    if (-1 == ret)
        errorExit("acct() error.\n");
    else
        exit(EXIT_SUCCESS);
}

void test_sc_clk_tck()
{
    long tick = sysconf(_SC_CLK_TCK);

    printf("_SC_CLK_TCK: %ld.\n", tick);
}

void test()
{
    turnOnAcct(true);
    //test_sc_clk_tck();
}

} //_1

namespace _2 {

#ifndef CHILD_SIG
#define CHILD_SIG   SIGUSR1
#endif

int threadFunc(void* arg)
{
    if (-1 == close(*((int*)arg))) {
        errorExit("close() error.\n");
    }

    return 0;
}

void test_clone(bool clone_files)
{
    const int STACK_SIZE = 65536;
    int flags = clone_files ? CLONE_FILES : 0;

    int fd = open("/dev/null", O_RDWR);
    if (-fd == fd)
        errorExit("open() error.\n");

    char* stack = (char*)malloc(STACK_SIZE);
    char* stackTop = stack + STACK_SIZE;

    if (CHILD_SIG != 0 && CHILD_SIG != SIGCHLD) {
        if (signal(CHILD_SIG, SIG_IGN) == SIG_ERR)
            errorExit("signal(CHILD_SIG) error.\n");
    }

    if (-1 == clone(threadFunc, stackTop, flags | CHILD_SIG, (void*)&fd)) {
        errorExit("clone() error.\n");
    }

    if (-1 == waitpid(-1, NULL, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0)) {
        errorExit("waitpid() error.\n");
    }

    printf("child has terminated.\n");

    int s = write(fd, "x", 1);
    if (-1 == s && errno == EBADF) {
        printf("file descriptor %d has been closed.\n", fd);
    } else if (-1 == s) {
        printf("write() on file descriptor %d failed unexpectedly (%s)\n", fd, strerror(errno));
    } else {
        printf("write() on file descriptor %d succeeded.\n", fd);
    }

    exit(EXIT_SUCCESS);
}

void test()
{
    test_clone(false);
}

} //_2

} //namespace =================================================================

void test_ch_28()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
