#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t mtx;

static void* _1_thread_main0(void* param)
{
    int* value = (int*)param;
    int tid = gettid();

    pthread_mutex_lock(&mtx);
    {
        (*value)++;
        printf("[%d] shared value: %d\n", tid, *value);
    }
    pthread_mutex_unlock(&mtx);

    return NULL;
}

static void* _1_thread_main1(void* param)
{
    int* value = (int*)param;
    int tid = gettid();

    pthread_mutex_lock(&mtx);
    {
        *value += 2;
        printf("[%d] shared value: %d\n", tid, *value);
    }
    pthread_mutex_unlock(&mtx);

    return NULL;
}

static void _1_test()
{
    int value = 0;

    int tid = gettid();

    pthread_t t0;
    pthread_t t1;

    pthread_mutex_init(&mtx, NULL);

    int ret0 = pthread_create(&t0, NULL, _1_thread_main0, &value);
    int ret1 = pthread_create(&t1, NULL, _1_thread_main1, &value);

    if (ret0 || ret1) {
        exit(1);
    }

    ret0 = pthread_join(t0, NULL);
    ret1 = pthread_join(t1, NULL);

    if (ret0 || ret1) {
        exit(2);
    }

    pthread_mutex_destroy(&mtx);

    printf("[%d] value: %d\n", tid, value);
}

//-------------------------------------------------------------------

//=============================================================================
void test_ch_16()
{
    _1_test();
}
