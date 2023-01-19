#ifndef CH_18_TEST_C
#define CH_18_TEST_C

#include "ch_18_shared_int32.h"
#include "ch_18_shared_mutex.h"
#include "ch_18_shared_cond.h"

#include <stdio.h>
#include <signal.h>

//=============================================================================

static int received = 0;

static shared_cond_t* cond = NULL;
static shared_mutex_t* mtx = NULL;

#define SHM_INT32_KEY   "/counter0"
#define SHM_MUTEX_KEY   "/mutex0"
#define SHM_COND_KEY    "/cond0"

static void sigint_handler(int signo)
{
    fprintf(stdout, "handling int signal: %d.\n", signo);
    received = 1;
}

//=============================================================================

void test_ch_18_shared_test(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: program number.\n");
        exit(1);
    }
    signal(SIGINT, sigint_handler);

    int my_number = atol(argv[1]);
    printf("my number is %d\n", my_number);

    shared_int32_t* counter = shared_int32_new();
    shared_int32_construct(counter, SHM_INT32_KEY);
    shared_int32_set_value_if_owner(counter, 1);

    mtx = shared_mutex_new();
    shared_mutex_construct(mtx, SHM_MUTEX_KEY);

    cond = shared_cond_new();
    shared_cond_construct(cond, SHM_COND_KEY);

    shared_mutex_lock(mtx);
    while (shared_int32_get_value(counter) < my_number) {
        if (received) {
            break;
        }

        printf("waiting for the signal, just for 5 seconds...\n");

        shared_cond_timed_wait(cond, mtx, 5L * 1000 * 1000 * 1000);
        if (received) {
            break;
        }
        printf(stdout, "checking condition...\n");
    }

    if (received) {
        printf(stdout, "exiting...\n");
        shared_mutex_unlock(mtx);
        goto destroy;
    }

    shared_int32_set_value(counter, my_number + 1);
    printf("my turn! %d ...\n", my_number);
    shared_mutex_unlock(mtx);
    sleep(1);

    shared_cond_broadcast(cond);

destroy:
    shared_cond_destruct(cond);
    shared_cond_delete(cond);

    shared_mutex_destruct(mtx);
    shared_mutex_delete(mtx);

    shared_int32_destruct(counter);
    shared_int32_delete(counter);
}

#endif // CH_18_TEST_C
