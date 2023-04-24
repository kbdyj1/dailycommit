#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void threadCleanup(void*)
{
    printf("threadCleanup\n");
}

void* threadFunc(void*)
{
//    pthread_cleanup_push(threadCleanup, NULL);

    printf("threadFunc()\n");
    for (int j=1; ; j++) {
#if (0)
        printf("Loop %d\n", j);
        sleep(1);
#else
        pthread_testcancel();
#endif
    }
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

} //namespace =================================================================

void exec_ch_04()
{
    _1::test();
}
