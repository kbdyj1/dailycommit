#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

namespace { //=================================================================

int global = 0;

namespace _1 {

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

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

} //namespace =================================================================

void exec_ch_02()
{
    _1::test();
}
