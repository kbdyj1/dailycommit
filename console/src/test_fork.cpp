#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void test_fork()
{
    pid_t child;

    fprintf(stdout, "start fork()\n");

    switch (child = fork()) {
    case -1:
        fprintf(stderr, "[E] %s\n", strerror(errno));
        break;

    case 0:
        fprintf(stdout, "[C]  pid: %ld\n", (long)getpid());
        fprintf(stdout, "[C] ppid: %ld\n", (long)getppid());
        break;

    default:
        fprintf(stdout, "[P] child pid: %ld\n", (long)child);
        break;
    }
}
