#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static int sValue = 2;

namespace { //=================================================================

namespace _1 {

void test()
{
    pid_t child;

    int lValue = 3;

    fprintf(stdout, "start fork()\n");

    switch (child = fork()) {
    case -1:
        fprintf(stderr, "[E] %s\n", strerror(errno));
        break;

    case 0:
        sValue *= 2;
        lValue *= 3;
        fprintf(stdout, "[C]  pid: %ld\n", (long)getpid());
        fprintf(stdout, "[C] ppid: %ld\n", (long)getppid());
        fprintf(stdout, "[C] static: %d, local: %d\n", sValue, lValue);
        break;

    default:
        sleep(1);
        fprintf(stdout, "[P] pid  : %ld\n", (long)getpid());
        fprintf(stdout, "[P] child: %ld\n", (long)child);
        fprintf(stdout, "[P] static: %d, local: %d\n", sValue, lValue);
        break;
    }
}

} //-----------------------------------------------------------------

namespace _2 {

void test()
{
    char path[] = "/tmp/test_123456";

    setbuf(stdout, NULL);

    int fd = open(path, O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "can't open %s\n", path);
        return;
    }

    printf("before fork() offset: %lld\n", lseek(fd, 0, SEEK_CUR));

    int flags = fcntl(fd, F_GETFL);
    if (-1 == flags) {
        fprintf(stderr, "error: F_GETFL\n");
        return;
    }

    switch (fork()) {
    case -1:
        fprintf(stderr, "fork error\n");
        return;

    case 0:
        if (-1 == lseek(fd, 1000, SEEK_SET)) {
            fprintf(stderr, "lseek(fd, 1000, SEEK_CUR) failed.\n");
            return;
        }
        flags = fcntl(fd, F_GETFL);
        flags |= O_APPEND;

        if (-1 == fcntl(fd, F_SETFL, flags)) {
            fprintf(stderr, "fcntl F_SETFL failed.\n");
            return;
        }
        _exit(EXIT_SUCCESS);

    default:
        sleep(1);
        exit(EXIT_SUCCESS);
    }
}

} //-----------------------------------------------------------------

namespace _3 {

void test()
{
    int lValue = 1;

    switch (vfork()) {
    case -1:
        fprintf(stderr, "vfork return -1.\n");
        return;

    case 0:
        sleep(1);
        write(STDOUT_FILENO, "[C] exec.\n", 10);
        lValue *= 2;
        _exit(EXIT_SUCCESS);

    default:
        write(STDOUT_FILENO, "[P] exec.\n", 10);
        printf("lValue: %d\n", lValue);
        exit(EXIT_SUCCESS);
    }
}

} //-----------------------------------------------------------------

namespace _4 {

#define SYNC_SIGNAL SIGUSR1

void sigHandler(int sig)
{
    fprintf(stdout, "[ ] pid: %ld\n", (long)getpid());
}

void test()
{
    setbuf(stdout, NULL);

    sigset_t bmask;
    sigset_t omask;
    sigset_t emask;

    sigemptyset(&bmask);
    sigaddset(&bmask, SYNC_SIGNAL);
    if (-1 == sigprocmask(SIG_BLOCK, &bmask, &omask)) {
        fprintf(stderr, "sigprocmask() return -1.\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sigHandler;
    if (-1 == sigaction(SYNC_SIGNAL, &sa, NULL)) {
        fprintf(stderr, "sigaction() return -1.\n");
        exit(EXIT_FAILURE);
    }

    pid_t child;

    switch (child = fork()) {
    case -1:
        fprintf(stderr, "fork() return -1");
        exit(EXIT_FAILURE);

    case 0:
        printf("[C] pid: %ld\n", (long)getpid());
        sleep(1);
        printf("[C] pid: %ld about to signal to parent.\n", (long)getpid());
        if (-1 == kill(getppid(), SYNC_SIGNAL)) {
            fprintf(stderr, "kill(SYNC_SIGNAL) return -1.\n");
            exit(EXIT_FAILURE);
        }

        _exit(EXIT_SUCCESS);

    default:
        printf("[P] pid: %ld\n", (long)getpid());
        sigemptyset(&emask);
        if (-1 == sigsuspend(&emask) && errno != EINTR) {
            fprintf(stderr, "sigsuspend() return -1\n");
            exit(EXIT_FAILURE);
        }

        printf("[P] pid: %ld parent got signal\n", (long)getpid());

        if (-1 == sigprocmask(SIG_SETMASK, &omask, NULL)) {
            fprintf(stderr, "[P] sigprocmask return -1.\n");
            exit(EXIT_FAILURE);
        }

        printf("[P] pid: %ld work done.\n", (long)getpid());
        exit(EXIT_SUCCESS);
    }
}

} //-----------------------------------------------------------------

} //===========================================================================

void test_fork()
{
#if (0) //done
    _1::test();
    _3::test();
#endif

    _4::test();
}
