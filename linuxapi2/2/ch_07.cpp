// PIPE & FIFO

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test()
{
    int fd[2];
    char buf[32];
    int size;

    if (-1 == pipe(fd)) {
        errorExit("pipe() error.\n");
    }

    switch (fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        // close out
        if (-1 == close(fd[1])) {
            errorExit("close(fd[1]) error.\n");
        }
        size = read(fd[0], buf, 32);
        buf[size] = '\0';
        printf("read(%s)\n", buf);
        break;

    default:
        // close in
        if (-1 == close(fd[0])) {
            errorExit("close(fd[0]) error.\n");
        }
        memset(buf, 0, 32);
        strncpy(buf, "Hello, Qt", 9);
        write(fd[1], buf, 32);
        break;
    }
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_07()
{
    _1::test();
}
