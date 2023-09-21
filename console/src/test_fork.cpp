#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

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

} //===========================================================================

void test_fork()
{
    _1::test();
}
