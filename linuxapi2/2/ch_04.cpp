#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void threadCleanup(void*)
{
    printf("\tthreadCleanup\n");
}

void* threadFunc(void*)
{
    pthread_cleanup_push(threadCleanup, NULL);
    {
        printf("threadFunc()\n");
        for (int j=1; ; j++) {
#if (1)
            printf("Loop %d\n", j);
            sleep(1);
#else
            pthread_testcancel();
#endif
        }
    }
    pthread_cleanup_pop(1);

    return NULL;
}

void test()
{
    pthread_t tid;
    int s = pthread_create(&tid, NULL, threadFunc, NULL);
    if (s != 0) {
        errnoExit("pthread_create()", s);
    }

    sleep(3);

    s = pthread_cancel(tid);
    if (s != 0) {
        errnoExit("pthread_cancel()", s);
    }

    void* res;
    s = pthread_join(tid, &res);
    if (s != 0) {
        errnoExit("pthread_join()", s);
    }

    auto canceled = res == PTHREAD_CANCELED;
    printf("Was thread canceled? %d\n", canceled);

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

namespace _2 {

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int global = 0;

void cleanup(void* arg)
{
    printf("cleanup: freeing block at %p\n", arg);
    free(arg);

    printf("cleanup: unlocking mutex\n");
    int s = pthread_mutex_unlock(&mutex);
    if (0 != s) {
        errnoExit("pthread_mutex_unlock", s);
    }
}

void* threadFunc(void* arg)
{
    void* buf = malloc(0x10000);
    printf("thread: allocate memory at %p\n", buf);

    int s = pthread_mutex_lock(&mutex);
    if (0 != s) {
        errnoExit("pthread_mutex_lock", s);
    }

    pthread_cleanup_push(cleanup, buf);
    {
        while (0 == global) {
            s = pthread_cond_wait(&cond, &mutex);
            if (0 != s) {
                errnoExit("pthread_cond_wait", s);
            }
        }

        printf("thread: condition wait loop completed\n");

    }
    pthread_cleanup_pop(1);

    return NULL;
}

void test(int argc, const char**)
{
    pthread_t tid;
    int s = pthread_create(&tid, NULL, threadFunc, NULL);
    if (0 != s) {
        errnoExit("pthread_create", s);
    }

    sleep(2);

    if (1 == argc) {
        printf("main: about to cancel thread\n");
        s = pthread_cancel(tid);
        if (0 != s) {
            errnoExit("pthread_cancel", s);
        }
    } else {
        printf("main: about to signal condition variable\n");
        global = 1;
        s = pthread_cond_signal(&cond);
        if (0 != s) {
            errnoExit("pthread_cond_signal", s);
        }
    }

    void* res;
    s = pthread_join(tid, &res);
    if (0 != s) {
        errnoExit("pthread_join", s);
    }

    auto canceled = res == PTHREAD_CANCELED;
    printf("main: thread was canceled? %d\n", canceled);

    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_04(int argc, const char** argv)
{
#if (0) //done
    _1::test();
#endif

    _2::test(argc, argv);
}
