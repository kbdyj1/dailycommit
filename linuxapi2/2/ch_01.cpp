#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace { //=================================================================

namespace _1 {

void* func(void* arg)
{
    char* s = (char*)arg;

    printf("func(%s) : %ld\n", s, (long)pthread_self());

//    sleep(2);

    return (void*)strlen(s);
}

void test()
{
    pthread_t t0;
    const char* message = "Hello, Qt";
    int s = pthread_create(&t0, NULL, func, (void*)message);
    if (s != 0) {
        char * err = strerror(s);
        fprintf(stderr, "pthread_create() error. (%s)\n", err);
        exit(-1);
    }

    printf("message from main\n");

    int res;
    s = pthread_join(t0, (void**)&res);
    if (s != 0) {
        char * err = strerror(s);
        fprintf(stderr, "pthread_join() error. (%s)\n", err);
        exit(-1);
    }

    printf("thread returned: %d\n", res);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_01()
{
    _1::test();
}
