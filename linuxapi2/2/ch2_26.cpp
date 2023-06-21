#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>

#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test()
{
    printf("FD_SETSIZE: %d\n", FD_SETSIZE); //1024
}

} //_1 --------------------------------------------------------------

namespace _2 {

void usageError(const char* program)
{
    fprintf(stderr, "Usage: %s timeout fd[rw] ...\n", program);
    exit(EXIT_FAILURE);
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        usageError(argv[0]);
    }
    timeval* pto = NULL;
    timeval timeout;

    if (0 != strcmp(argv[1], "-")) {
        pto = &timeout;
        timeout.tv_sec = atoi(argv[1]);
        timeout.tv_usec = 0;
    }

    int fds = 0;
    fd_set rfds;
    fd_set wfds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    int numRead;
    char buf[10];
    int fd;

    for (int i=2; i<argc; i++) {
        numRead = sscanf(argv[i], "%d%2[rw]", &fd, buf);
        if (2 != numRead) {
            usageError(argv[0]);
        }
        if (FD_SETSIZE <= fd) {
            errorExit("file descriptor exceeds limit (FD_SETSIZE <= %d)\n", fd);
        }
        if (fds <= fd)
            fds = fd + 1;
        if (NULL != strchr(buf, 'r')) {
            FD_SET(fd, &rfds);
        }
        if (NULL != strchr(buf, 'w')) {
            FD_SET(fd, &wfds);
        }
    }

    int ready = select(fds, &rfds, &wfds, NULL, pto);
    if (-1 == ready) {
        errnoExit("select", errno);
    }
    printf("ready: %d\n", ready);

    for (int i=0; i<fds; i++) {
        printf("%d: %s%s\n", i, FD_ISSET(i, &rfds) ? "r" : " ", FD_ISSET(i, &wfds) ? "w" : " ");
    }
    if (NULL != pto) {
        printf("timeout after select(): %lds\n", (long)timeout.tv_sec);
    }
    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_26(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
#endif

    _2::test(argc, argv);
}
