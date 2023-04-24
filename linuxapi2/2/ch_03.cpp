#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"

namespace { //=================================================================

#define MAX_ERROR_LEN   256

namespace _1 {

pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_key_t threadKey = -1;
void* threadData = NULL;

void onDestroy(void* p)
{
    printf("onDestroy(void*)\n");
    free(p);
}

void initOnce()
{
    printf("initOnce() called.\n");
    threadKey = pthread_key_create(&threadKey, onDestroy);
    threadData = malloc(1);
    pthread_setspecific(threadKey, threadData);
}

void test()
{
    pthread_once(&once, initOnce);
    pthread_once(&once, initOnce);
}

} //_1 --------------------------------------------------------------

namespace _2 {

char buf[MAX_ERROR_LEN];

char* strError(int err)
{
    if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    } else {
        strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN-1);
        buf[MAX_ERROR_LEN-1] = '\0';
    }
    return buf;
}

} //_2 --------------------------------------------------------------

namespace _3 {

pthread_once_t once;
pthread_key_t strErrorKey;

void onDestroy(void* p)
{
    free(p);
}

void initKey()
{
    int s = pthread_key_create(&strErrorKey, onDestroy);
    if (s != 0) {
        //errnoExit("pthread_key_create(): ", s);
    }
}

char* strError(int err)
{
    int s = pthread_once(&once, initKey);
    if (s != 0) {
        //errnoExit("pthread_once(): ", s);
    }

    char* buf = (char*) pthread_getspecific(strErrorKey);
    if (buf == NULL) {
        buf = (char*)malloc(MAX_ERROR_LEN);

        s = pthread_setspecific(strErrorKey, buf);
        if (s != 0) {
            //errnoExit("pthread_setspecific(): ", s);
        }
    }

    if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    } else {
        strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN-1);
        buf[MAX_ERROR_LEN-1] = '\0';
    }
    return buf;
}

void test()
{
    printf("sysconf(_SC_THREAD_KEY_MAX): %d\n", sysconf(_SC_THREAD_KEYS_MAX));
}

} //_3 --------------------------------------------------------------

namespace _4 { // TLS

__thread char buf[MAX_ERROR_LEN];

char* strError(int err)
{
    if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    } else {
        strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN-1);
        buf[MAX_ERROR_LEN-1] = '\0';
    }
    return buf;
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_03()
{
#if (0) //done
    _1::test();
#endif
}
