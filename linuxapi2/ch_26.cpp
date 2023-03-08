#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "utils.h"

namespace { //=================================================================

namespace _1 {

const int CHILD_COUNT = 3;
const int sleepTable[] = { 7, 1, 4 };

void test()
{
    int numDead = 0;
    pid_t child;

    setbuf(stdout, NULL);

    for (int j=0; j<CHILD_COUNT; j++) {
        switch (fork()) {
        case -1:
            errorExit("fork() error.\n");

        case 0:
            printf("[ ] child %d started with PID %ld, sleeping %d\n", j, (long)getpid(), sleepTable[j]);
            sleep(sleepTable[j]);
            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }

    for ( ;; ) {
        child = wait(NULL);
        if (-1 == child) {
            if (errno == ECHILD) {
                printf("\nno more children. bye~\n");
                exit(EXIT_SUCCESS);
            } else {
                errorExit("wait() error.\n");
            }
        }

        ++numDead;
        printf("[ ] wait returned child PID %ld (numDead=%d)\n", (long)child, numDead);
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

void handler(int sig)
{
    signal(sig, SIG_DFL);
    raise(sig);
}

int test_waitpid()
{
    int status;
    pid_t child = waitpid(-1, &status, WUNTRACED | WCONTINUED);
    if (-1 == child)
        errorExit("waitpid() error.\n");


    printf("waitpid() returned: PID: %ld, status:0x%04x (%d, %d)\n", (long)child, status, status>>8, status&0xff);

    return status;
}

int test_waitid()
{
    siginfo_t si;
    if (-1 == waitid(P_ALL, 0, &si, WEXITED)) {
        errorExit("waitid(P_ALL, ...) error.\n");
    }

    printf("si_code: %d\n", si.si_code);
    printf("si_pid: %ld\n", (long)si.si_pid);
    printf("si_signo: %s\n", strsignal(si.si_signo));
    printf("si_status: %d\n", si.si_status);
    printf("si_uid: %d\n", si.si_uid);

    return si.si_status;
}

// not standard
int test_wait4()
{
    int status;
    struct rusage usage;
    if (-1 == wait4(-1, &status, WUNTRACED, &usage)) {
        errorExit("wait4() error.\n");
    }

    double ut = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1000000.0;
    double st = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1000000.0;

    printf("user  : %6.2f secs\n", ut);
    printf("system: %6.2f secs\n", st);

    return status;
}

void test()
{
    pid_t child;
    int status;

    switch (fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        printf("child started with PID: %ld\n", (long)getpid());
        for ( ;; )
            pause();
        exit(EXIT_FAILURE);

    default:
        for ( ;; ) {
#if (0)
            status = test_waitpid();
#else
            //status = test_waitid();
            status = test_wait4();
#endif

            printWaitStatus(NULL, status);

            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                exit(EXIT_SUCCESS);
            }
        }
    }
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_26()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
