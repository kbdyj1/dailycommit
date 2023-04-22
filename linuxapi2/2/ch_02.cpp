#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <memory.h>
#include "utils.h"

namespace { //=================================================================

int global = 0;

namespace _1 {

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;    // static mutex

void* threadFunc(void* param)
{
    int loopCnt = *(int*)param;
    int local;
    int s;

    for (int j=0; j<loopCnt; ) {
#if (1)
        s = pthread_mutex_lock(&mtx);
#else
        s = pthread_mutex_trylock(&mtx);
#endif
        if (s == 0) {
            local = global;
            local++;
            global = local;
            j++;
            s = pthread_mutex_unlock(&mtx);
        }
    }
    return NULL;
}

void test()
{
    pthread_t t0, t1;
    int loops = 100000;

    int s = pthread_create(&t0, NULL, threadFunc, (void*)&loops);
    if (s != 0) {
        fprintf(stderr, "pthread_create(&t0) error: %d\n", s);
        exit(-1);
    }

    s = pthread_create(&t1, NULL, threadFunc, (void*)&loops);
    if (s != 0) {
        fprintf(stderr, "pthread_create(&t1) error: %d\n", s);
        exit(-1);
    }

    s = pthread_join(t0, NULL);
    if (s != 0) {
        fprintf(stderr, "pthread_join(t0) error: %d\n", s);
        exit(-1);
    }

    s = pthread_join(t1, NULL);
    if (s != 0) {
        fprintf(stderr, "pthread_join(t1) error: %d\n", s);
        exit(-1);
    }

    printf("global: %d\n", global);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test_result(const char* message, int s)
{
    if (s != 0) {
        errnoExit(message, s);
    } else {
        printf("%s: ok\n", message);
    }
}

void test()
{
    pthread_mutex_t mtx;
    pthread_mutexattr_t attr;

    int s = pthread_mutexattr_init(&attr);
    test_result("pthread_mutexattr_init", s);

    s = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    test_result("pthread_mutexattr_settype", s);

    s = pthread_mutex_init(&mtx, &attr);
    test_result("pthread_mutex_init", s);

    s = pthread_mutex_destroy(&mtx);
    test_result("pthread_mutex_destroy", s);
}

} //_2 --------------------------------------------------------------

namespace _3 {

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int sharedCnt = 0;

void* consumerThread(void* arg)
{
    int count = *(int*)arg;
    int s;

    while (count) {
        s = pthread_mutex_lock(&mtx);

        while (sharedCnt == 0) {
            s = pthread_cond_wait(&cond, &mtx);
        }

        --count;
        --sharedCnt;

        printf("consumer(count: %d, shared: %d\n", count, sharedCnt);

        s = pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

void* producerThread(void* arg)
{
    int count = *(int*)arg;
    int s;
    for (int i=0; i<count; i++) {
        s = pthread_mutex_lock(&mtx);
        if (s != 0) {
            errnoExit("pthread_mutex_lock(): ", s);
        }

        sharedCnt++;

        s = pthread_mutex_unlock(&mtx);
        if (s != 0) {
            errnoExit("pthread_mutex_unlock: ", s);
        }

        s = pthread_cond_signal(&cond);
    }
    return NULL;
}

void test()
{
    int totalCnt = 100;
    int localCnt = 25;
    constexpr int threadCnt = 4;

    pthread_t producer[threadCnt];
    pthread_t consumer;

    int s = pthread_create(&consumer, NULL, consumerThread, &totalCnt);
    if (s != 0) {
        errnoExit("pthread_create(consumer): ", s);
    }
    for (int i=0; i<threadCnt; i++) {
        s = pthread_create(&producer[i], NULL, producerThread, &localCnt);
        if (s != 0) {
            errnoExit("pthread_create(producer): ", s);
        }
    }

    pthread_join(consumer, NULL);
    pthread_join(producer[0], NULL);
    pthread_join(producer[1], NULL);
    pthread_join(producer[2], NULL);
    pthread_join(producer[3], NULL);
}

} //_3 --------------------------------------------------------------

namespace _4 {

pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int totalThreads = 0;
int totalLives = 0;
int totalUnjoined = 0;

enum state_t {
    STATE_ALIVE,
    STATE_TERMINATE,
    STATE_JOINED
};

struct threadinfo {
    pthread_t tid;
    enum state_t state;
    int sleepTime;
    int index;
}* pthreadinfo;

void* threadFunc(void* arg)
{
    threadinfo* pinfo = (threadinfo*)arg;
    int s;

    sleep(pinfo->sleepTime);
    printf("thread[%ld] terminated.\n", (long)pinfo->index);

    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
        errnoExit("pthread_mutex_lock(): ", s);
    }

    totalUnjoined++;
    pinfo->state = STATE_TERMINATE;

    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
        errnoExit("pthread_mutex_unlock(): ", s);
    }

    s = pthread_cond_signal(&threadDied);
    if (s != 0) {
        errnoExit("pthread_cond_signal(): ", s);
    }

    return NULL;
}

void test()
{
    int s;
    constexpr int numThreads = 5;
    int sleeps[] = { 1, 1, 2, 3, 3 };

    pthreadinfo = (threadinfo*)malloc(numThreads * sizeof(*pthreadinfo));
    for (int i=0; i<numThreads; i++) {
        pthreadinfo[i].sleepTime = sleeps[i];
        pthreadinfo[i].state = STATE_ALIVE;
        pthreadinfo[i].index = i;
        s = pthread_create(&pthreadinfo[i].tid, NULL, threadFunc, (void*)&pthreadinfo[i]);
        if (s != 0) {
            errnoExit("pthread_create(): ", s);
        }
    }

    totalThreads = numThreads;
    totalLives = numThreads;

    while (0 < totalLives) {
        s = pthread_mutex_lock(&mtx);
        if (s != 0) {
            errnoExit("pthread_mutex_lock(): ", s);
        }

        while (0 == totalUnjoined) {
            s = pthread_cond_wait(&threadDied, &mtx);
            if (s != 0) {
                errnoExit("pthread_cond_wait(): ", s);
            }
        }

        for (int i=0; i<totalThreads; i++) {
            if (pthreadinfo[i].state == STATE_TERMINATE) {
                s = pthread_join(pthreadinfo[i].tid, NULL);
                if (s != 0) {
                    errnoExit("pthread_join(): ", s);
                }

                pthreadinfo[i].state = STATE_JOINED;
                --totalLives;
                --totalUnjoined;

                printf("Reaped thread %d (totalLive: %d)\n", i, totalLives);
            }
        }

        s = pthread_mutex_unlock(&mtx);
        if (s != 0) {
            errnoExit("pthread_mutex_unlock(): ", s);
        }
    }

    exit(EXIT_SUCCESS);
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_02()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
