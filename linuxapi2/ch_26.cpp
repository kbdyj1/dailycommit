#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
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

void test()
{

}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_26()
{
#if (0) //done
    _1::test();
#endif


}
