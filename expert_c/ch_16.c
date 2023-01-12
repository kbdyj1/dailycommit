#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

static pthread_mutex_t mtx;

static void common_join(pthread_t t0, pthread_t t1)
{
    int result0 = pthread_join(t0, NULL);
    int result1 = pthread_join(t1, NULL);

    if (result0 || result1) {
        printf("pthread_join failed. %d, %d\n", result0, result1);
        exit(2);
    }

    printf("join OK\n");
}

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

    common_join(t0, t1);

    pthread_mutex_destroy(&mtx);

    printf("[%d] value: %d\n", tid, value);
}

//------------------------------------------------------------------- condition

typedef unsigned int bool_t;

#define FALSE   0
#define TRUE    1

typedef struct {
    bool_t done;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} shared_state_t;

static void _2_shared_state_init(shared_state_t* state)
{
    state->done = FALSE;
    pthread_mutex_init(&state->mtx, NULL);
    pthread_cond_init(&state->cond, NULL);
}

static void _2_shared_state_destroy(shared_state_t* state)
{
    pthread_mutex_destroy(&state->mtx);
    pthread_cond_destroy(&state->cond);
}

static void* _2_thread_0(void* param)
{
    shared_state_t* state = (shared_state_t*)param;
    pthread_mutex_lock(&state->mtx);
    printf("A\n");
    state->done = TRUE;
    pthread_cond_signal(&state->cond);
    pthread_mutex_unlock(&state->mtx);

    return NULL;
}

static void* _2_thread_1(void* param)
{
    shared_state_t* state = (shared_state_t*)param;
    pthread_mutex_lock(&state->mtx);
    while (!state->done) {
        pthread_cond_wait(&state->cond, &state->mtx);   //->sleep -> unlock mutex
        printf("loop!!!\n");
    }
    printf("B\n");
    pthread_mutex_unlock(&state->mtx);

    return NULL;
}

static void _2_test()
{
    shared_state_t state;

    _2_shared_state_init(&state);

    pthread_t t0;
    pthread_t t1;

    int result1 = pthread_create(&t1, NULL, _2_thread_1, &state);
    int result0 = pthread_create(&t0, NULL, _2_thread_0, &state);

    if (result0 || result1) {
        printf("pthread_created failed. %d, %d\n", result0, result1);
        exit(1);
    }

    common_join(t0, t1);

    _2_shared_state_destroy(&state);
}

//------------------------------------------------------------------- barrier

#if !defined(__APPLE__) //OSX does not support barrier

static pthread_barrier_t barrier;

static void* _3_thread_0(void* param)
{
    printf("A\n");
    pthread_barrier_wait(&barrier);

    return NULL;
}

static void* _3_thread_1(void* param)
{
    pthread_barrier_wait(&barrier);
    printf("B\n");

    return NULL;
}

static void _3_test()
{
    pthread_barrier_init(&barrier, NULL, 2);

    pthread_t t0;
    pthread_t t1;

    int result0 = pthread_create(&t0, NULL, _3_thread_0, NULL);
    int result1 = pthread_create(&t1, NULL, _3_thread_1, NULL);

    if (result0 || result1) {
        printf("pthread_create failed: %d, %d\n", result0, result1);
        exit(1);
    }

    common_join(t0, t1);

    pthread_barrier_destroy(&barrier);
}

#endif //!__APPLE__

//------------------------------------------------------------------- semaphore

static sem_t* semaphore = NULL;

static void* _4_thread_0(void* param)
{
    int* p = (int*)param;

    sem_wait(semaphore);

    (*p)++;

    printf("%d\n", *p);

    sem_post(semaphore);

    return NULL;
}

static void* _4_thread_1(void* param)
{
    int* p = (int*)param;

    sem_wait(semaphore);

    (*p) += 2;

    printf("%d\n", *p);

    sem_post(semaphore);

    return NULL;
}

static void _4_test()
{
    int value = 0;

    pthread_t t0;
    pthread_t t1;

#if defined(__APPLE__)
    semaphore = sem_open("sem0", O_CREATE|O_EXCL, 0644, 1);
#else
    sem_t sem;
    semaphore = &sem;
    sem_init(semaphore, 0, 1);
#endif

    int result0 = pthread_create(&t0, NULL, _4_thread_0, &value);
    int result1 = pthread_create(&t1, NULL, _4_thread_1, &value);

    if (result0 || result1) {
        printf("pthread_create failed. %d, %d\n", result0, result1);
        exit(1);
    }

    common_join(t0, t1);

    printf("value: %d\n", value);

#if defined(__APPLE__)
    sem_close(semaphore);
#else
    sem_destroy(semaphore);
#endif
}

//=============================================================================
void test_ch_16()
{
#if (0)
    _1_test();
    _2_test();
    _3_test();
#endif

    _4_test();

    sleep(1);
}
