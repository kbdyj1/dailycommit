#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
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

namespace _2 {

void* threadFunc(void* param)
{
    int threadNum = *(int*)param;

    printf("thread %d executing\n", threadNum);

    sigset_t set;
    sigemptyset(&set);
    if (-1 == sigaddset(&set, SIGUSR1)) {
        fprintf(stderr, "sigaddset() error.\n");
        pthread_exit((void*)1);
    }

    int sig;
    int s = sigwait(&set, &sig);
    if (s != 0) {
        errnoExit("sigwait() : ", s);
    } else {
        printf("sigwait() done\n");
    }

    if (sig != SIGUSR1) {
        fprintf(stderr, "sigwait() error.\n");
        pthread_exit((void*)2);
    } else {
        printf("sigwait() received: SIGUSR1\n");
    }

    return NULL;
}

void test()
{
    pthread_t tid;
    int threadParam = 1;

    int s = pthread_create(&tid, NULL, threadFunc, &threadParam);
    if (s != 0) {
        errnoExit("pthread_create: ", s);
    }

    sleep(2);

    s = pthread_kill(tid, SIGUSR1);
    if (s != 0) {
        errnoExit("pthread_kill: ", s);
    }

    s = pthread_join(tid, NULL);
    if (s != 0) {
        errnoExit("pthread_join: ", s);
    } else {
        printf("pthread_join() done.\n");
    }

    sleep(2);

    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_05()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
