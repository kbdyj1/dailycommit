#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define _GNU_SOURCE
#include <unistd.h>

static int thread_main(void* param)
{
    printf("Hello, POSIX\n");

    return 0;
}

static void test_thread_id()
{
    pthread_t thread = 0;

    thread = pthread_self();

    printf("pthread: %lu\n", (unsigned long)thread);
}

static void test_thread_create_detached(bool detach)
{
    pthread_t thread;

    int result = pthread_create(&thread, NULL, thread_main, NULL);
    if (result != 0) {
        printf("pthread_create failed: %d\n", result);
        exit(1);
    }

    printf("pthread: %lu\n", (unsigned long)thread);

    if (detach) {
        result = pthread_detach(thread);
        if (result != 0) {
            printf("pthread_detach failed: %lu\n", (unsigned long)thread);
            exit(2);
        } else {
            printf("pthread_detach succeeded\n");
            pthread_exit(NULL);
        }
    } else {
        result = pthread_join(thread, NULL);
        if (result != 0) {
            printf("pthread_join failed: %d\n", result);
            exit(2);
        }
    }
}


static void test_thread_create()
{
    test_thread_create_detached(false);
}

static void test_detached_thread()
{
    test_thread_create_detached(true);
}

//-------------------------------------------------------------------

static void* thread_main2(void* param)
{
    const char* str = (const char*)param;

    printf("%s\n", str);

    return NULL;
}

static void test_thread_compete()
{
    pthread_t t0;
    pthread_t t1;
    pthread_t t2;

    int result0 = pthread_create(&t0, NULL, thread_main2, "Minji");
    int result1 = pthread_create(&t1, NULL, thread_main2, "Hani");
    int result2 = pthread_create(&t2, NULL, thread_main2, "Daniel");

    if (result0 || result1 || result2) {
        printf("pthread_create failed. 1:%d, 2:%d, 3:%d\n", result0, result1, result2);
        exit(1);
    }

    result0 = pthread_join(t0, NULL);
    result1 = pthread_join(t1, NULL);
    result2 = pthread_join(t2, NULL);

    if (result0 || result1 || result2) {
        printf("pthread_join failed. 1:%d, 2:%d, 3:%d\n", result0, result1, result2);
        exit(2);
    }

    sleep(1);

    fflush(stdout);
}

//-------------------------------------------------------------------

static void* thread_main3(void* param)
{
    int* sharedValue = (int*)param;

    int tid = gettid();

    (*sharedValue)++;

    printf("[%d] shared value: %d\n", tid, *sharedValue);

    return NULL;
}

static void* thread_main4(void *param)
{
    int* sharedValue = (int*)param;

    int tid = gettid();

    *sharedValue += 2;

    printf("[%d] shared value: %d\n", tid, *sharedValue);

    return NULL;
}

void test_thread_compete_with_shared()
{
    int sharedValue = 0;

    pthread_t t0;
    pthread_t t1;

    int result0 = pthread_create(&t0, NULL, thread_main3, &sharedValue);
    int result1 = pthread_create(&t1, NULL, thread_main4, &sharedValue);

    if (result0 || result1) {
        printf("pthread_create() error: %d, %d", result0, result1);
        exit(1);
    }

    result0 = pthread_join(t0, NULL);
    result1 = pthread_join(t1, NULL);

    if (result0 || result1) {
        printf("pthread_join() error: %d, %d", result0, result1);
        exit(2);
    }

    printf("sharedValue: %d\n", sharedValue);
}

//=============================================================================

void test_ch_15()
{
#if (0) //done
    test_thread_create();
    test_thread_id();
    test_detached_thread();
    test_thread_compete();
#endif

    test_thread_compete_with_shared();
}
