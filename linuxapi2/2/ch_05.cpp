#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

const int StackSizePerThread = 2*1024*1024; // linux

void* threadFunc(void*)
{
    int tid = gettid();
    pthread_attr_t attr;
    size_t size;

    sleep(1);

    int s = pthread_attr_getstacksize(&attr, &size);
    if (s != 0) {
        errnoExit("pthread_attr_getstacksize: ", s);
    }

    printf("thread [%ld] stack size: %ld\n", (long)tid, size);
    printf("thread Min stack size: %ld\n", sysconf(_SC_THREAD_STACK_MIN));  //16K

    return NULL;
}

void test()
{
    pthread_t tid;
    pthread_attr_t attr;

    int StackSize = StackSizePerThread * 2;

    int s = pthread_attr_init(&attr);
    if (s != 0) {
        errnoExit("pthread_attr_init: ", s);
    }

    s = pthread_attr_setstacksize(&attr, StackSize);
    if (s != 0) {
        errnoExit("pthread_attr_setstacksize: ", s);
    }

    s = pthread_create(&tid, &attr, threadFunc, NULL);
    if (s != 0) {
        errnoExit("pthread_create: ", s);
    } else {
        printf("main: thread was created.\n");
    }

    printf("main: about to join\n");

    s = pthread_join(tid, NULL);
    if (s != 0) {
        errnoExit("pthread_join: ", s);
    }

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_05()
{
    _1::test();
}
