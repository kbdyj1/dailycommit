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

} //namespace =================================================================

void test_ch_24(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
