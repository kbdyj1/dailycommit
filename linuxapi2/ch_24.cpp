#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

namespace { //=================================================================

namespace _1 {

int iData = 111;

void test()
{
    int iStack = 222;
    pid_t child;

    switch (child = fork()) {
    case -1:
        fprintf(stderr, "fork() error.\n");
        exit(-1);

    case 0: //child
        iData *= 2;
        iStack *= 2;
        break;

    default: //parent
        sleep(3);
        break;
    }

    printf("pid=%ld %s iData=%d iStack=%d\n",
           (long)getpid(),
           (child == 0) ? "(child)" : "(parent)",
           iData,
           iStack);

    exit(0);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    int fd;
    int flags;
    char temp[] = "/tmp/testXXXXXX";

    setbuf(stdout, NULL);   //prevent buffering of stdout

    fd = mkstemp(temp);
    if (-1 == fd) {
        fprintf(stderr, "mkstemp(%s) error.\n", temp);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(-1);
    } else {
        printf("mkstemp(%s) succeeded.\n", temp);
    }

    printf("file offset before fork(): %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (-1 == flags) {
        fprintf(stderr, "fcntl(F_GETFL) error.\n");
        exit(-1);
    }

    printf("O_APPEND before fork() is: %s\n", (flags & O_APPEND) ? "ON" : "OFF");

    switch (fork()) {
    case -1:
        fprintf(stderr, "fork() error.\n");
        exit(-1);

    case 0: //child
        if (-1 == lseek(fd, 1000, SEEK_SET)) {
            fprintf(stderr, "lseek(1000, SEEK_SET) error.\n");
            exit(-1);
        }
        flags = fcntl(fd, SEEK_SET);

        flags |= O_APPEND;

        if (-1 == fcntl(fd, F_SETFL, flags)) {
            fprintf(stderr, "fcntl(F_SETFL) error.\n");
            exit(-1);
        }
        exit(EXIT_SUCCESS);

    default:
        if (-1 == wait(NULL)) {
            fprintf(stderr, "wait(NULL) error.\n");
            exit(-1);
        }

        printf("child exited.\n");

        printf("file offset in parent: %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

        flags = fcntl(fd, F_GETFL);

        printf("O_APPEND in parent is: %s\n", (flags & O_APPEND) ? "ON" : "OFF");
        exit(EXIT_SUCCESS);
    }
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    int iStack = 1;

    switch (vfork()) {
    case -1:
        fprintf(stderr, "vfork() error.\n");
        exit(-1);

    case 0:
        sleep(2);
        write(STDOUT_FILENO, "child executing\n", 16);
        iStack *= 3;
        _exit(EXIT_SUCCESS);

    default:
        write(STDOUT_FILENO, "parent executing\n", 17);
        printf("iStack: %d\n", iStack);
        exit(EXIT_SUCCESS);
    }
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test()
{
    int numChildren = 5;
    int j;
    pid_t child;

    setbuf(stdout, NULL);

    for (j=0; j<numChildren; j++) {
        switch (child = fork()) {
        case -1:
            fprintf(stderr, "fork() error.\n");
            exit(-1);

        case 0:
            printf("[ ] %d\n", j);
            _exit(EXIT_SUCCESS);
            break;

        default:
            printf("[P] %d\n", j);
            wait(NULL);
            break;
        }
    }
}

} //_4 --------------------------------------------------------------

namespace _5 {

#ifdef SYNC_SIG
#undef SYNC_SIG
#endif

#define SYNC_SIG    SIGUSR1

void handler(int sig)
{}

void test()
{
    pid_t child;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);
    if (-1 == sigprocmask(SIG_BLOCK, &blockMask, &origMask)) {
        fprintf(stderr, "sigprocmask() error.\n");
        exit(-1);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;

    if (-1 == sigaction(SYNC_SIG, &sa, NULL)) {
        fprintf(stderr, "sigaction() error.\n");
        exit(-1);
    }

    switch (child = fork()) {
    case -1:
        fprintf(stderr, "fork() error.\n");
        exit(-1);

    case 0:
        printf("[ ] %ld child started.\n", (long)getpid());
        sleep(2);

        printf("[ ] %ld child about to signal to parent.\n", (long)getpid());
        if (-1 == kill(getppid(), SYNC_SIG)) {
            fprintf(stderr, "kill() error.\n");
            exit(-1);
        }

        _exit(EXIT_SUCCESS);

    default:
        printf("[P] %ld parent about to wait for signal.\n", (long)getpid());

        sigemptyset(&emptyMask);

        if (-1 == sigsuspend(&emptyMask) && errno != EINTR) {
            fprintf(stderr, "sigsuspend() error.\n");
            exit(-1);
        }

        printf("[P] %ld parent got signal.\n", (long)getpid());

        if (-1 == sigprocmask(SIG_SETMASK, &origMask, NULL)) {
            fprintf(stderr, "sigprocmask(origMask) error.\n");
            exit(-1);
        }

        exit(EXIT_SUCCESS);
    }
}

#undef  SYNC_SIG

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_24(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
#endif

    _5::test();
}
