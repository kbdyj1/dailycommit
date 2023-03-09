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

namespace _3 {

const int CMD_SIZE = 128;

void test()
{
    pid_t child;
    char cmd[CMD_SIZE];
    int j;

    setbuf(stdout, NULL);

    printf("[P] PID=%ld\n", (long)getpid());

    switch (child = fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        printf("[ ] child PID: %ld exiting\n", (long)getpid());
        _exit(EXIT_SUCCESS);

    default:
        sleep(3);
        j = snprintf(cmd, CMD_SIZE, "ps | grep %s", "linuxapi2");
        cmd[j] = '\0';

        system(cmd);

        if (-1 == kill(child, SIGKILL)) {
            errorExit("kill(child, SIGKILL) failed.\n");
        }

        sleep(3);

        printf("[ ] After sending SIGKILL to zombie (PID=%ld)\n", (long)child);

        system(cmd);

        printf("[ ] before wait().\n");

        int status;
        int pid = wait(&status);

        printf("[ ] after wait(). PID = %ld, status = %d\n", (long)pid, status);

        sleep(3);

        system(cmd);

        exit(EXIT_SUCCESS);
    }
}

} //_3 --------------------------------------------------------------

namespace _4 {

int liveChildren = 0;

void sig_child_handler(int sig)
{
    int saveErrno = errno;
    pid_t child;
    int status;
    
    printf("Caught SIGCHLD.\n");
    
    while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("sig_child_handler: Reaped child: %ld\n", (long)child);
        
        printWaitStatus(NULL, status);
        
        liveChildren--;
    }
    
    if (-1 == child && errno != ECHILD) {
        fprintf(stderr, "waitpid() error: %s\n", strerror(errno));
        errorExit("waitpid() error. \n");
    }
    
    sleep(5);
    
    printf("sig_child_handler returning...\n");
    
    errno = saveErrno;
}

void test()
{
    int j;
    int sigCount = 0;
    const int CHILDNUM = 3;
    const int sleepTable[CHILDNUM] = { 1, 2, 4 };
    
    setbuf(stdout, NULL);
    
    liveChildren = CHILDNUM;
    struct sigaction sa;
    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_child_handler;
    
    if (-1 == sigaction(SIGCHLD, &sa, NULL)) {
        errorExit("sigaction() error.\n");
    }

    sigset_t emptyMask, blockMask;

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if (-1 == sigprocmask(SIG_SETMASK, &blockMask, NULL)) {
        errorExit("sigprocmask(blockMask) error.\n");
    }

    for (j=0; j<CHILDNUM; j++) {
        switch (fork()) {
        case -1:
            errorExit("for() error.\n");

        case 0:
            sleep(sleepTable[j]);
            printf("child %d (PID = %ld) exiting.\n", j, (long)getpid());
            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }

    sigemptyset(&emptyMask);

    while (0 < liveChildren) {
        if (-1 == sigsuspend(&emptyMask) && errno != EINTR) {
            errorExit("sigsuspend() error.\n");
        }
        sigCount++;

        printf("[ ] sigCount: %d\n", sigCount);
    }

    printf("All %d children have terminated. SIGCHLD was caught %d times\n", CHILDNUM, sigCount);

    exit(EXIT_SUCCESS);
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_26()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
