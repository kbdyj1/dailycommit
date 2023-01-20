#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

void test_ch_19_pipe()
{
    int fds[2];

    pipe(fds);

    int child = fork();

    if (-1 == child) {
        fprintf(stderr, "fork error. %s\n", strerror(errno));
        exit(1);
    }
    if (0 == child) {
        close(fds[0]);  //read
        char str[] = "Hello, Qt!";
        fprintf(stdout, "CHILD: waiting for 2 seconds...\n");
        sleep(2);
        fprintf(stdout, "CHILD: writing to parent...\n");
        write(fds[1], str, strlen(str)+1);
    } else {
        close(fds[1]);  //write
        char buf[32];
        fprintf(stdout, "PARENT: reading from child...\n");
        int read_bytes = read(fds[0], buf, 32);
        fprintf(stdout, "PARENT: received. [%s]\n", buf);
    }
}
