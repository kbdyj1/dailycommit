// PIPE & FIFO

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test()
{
    int fd[2];
    char buf[32];

    if (-1 == pipe(fd)) {
        errorExit("pipe() error.\n");
    }

    switch (fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        // close out
        if (-1 == close(fd[1])) {
            errorExit("CHILD: close(fd[1]) error.\n");
        }
        for ( ;; ) {
            int size = read(fd[0], buf, 32);
            if (-1 == size) {
                errorExit("CHILD: read() -1\n");
            } else if (0 == size) {
                // EOF
                printf("CHILD: EOF\n");
                break;
            }
            buf[size] = '\0';
            printf("CHILD: read(%s)\n", buf);
        }
        if (-1 == close(fd[0])) {
            errorExit("CHILD: close(fd[0]) error.\n");
        }
        _exit(EXIT_SUCCESS);
        break;

    default:
        // close in
        if (-1 == close(fd[0])) {
            errorExit("PARENT: close(fd[0]) error.\n");
        }
        memset(buf, 0, 32);
        strncpy(buf, "Hello, Qt", 9);
        write(fd[1], buf, 32);
        if (-1 == close(fd[1])) {
            errorExit("PARENT: close(fd[1] error.\n");
        }
        // wait for the child to exit
        wait(NULL);

        exit(EXIT_SUCCESS);
        break;
    }
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_07()
{
    _1::test();
}
