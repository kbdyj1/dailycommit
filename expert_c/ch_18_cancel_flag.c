#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>

#include "helper.h"

//=============================================================================

typedef uint16_t bool_t;

#define TRUE    1
#define FALSE   0

#define MTX_KEY "/mtx0"
#define SHM_KEY "/shm0"

static int cancel_flag_shm_fd = -1;

static bool_t cancel_flag_shm_owner = FALSE;

static int mtx_shm_fd = -1;

static pthread_mutex_t* mtx = NULL;

static bool_t mtx_owner = FALSE;

static bool_t* cancel_flag = NULL;

static void init_control()
{
    mtx_shm_fd = shm_open(MTX_KEY, O_RDWR, 0600);
    if (0 <= mtx_shm_fd) {
        mtx_owner = FALSE;
        fprintf(stdout, "the mutex's shared memory object is opened.\n");
    } else if (ENOENT == errno) {
        fprintf(stderr, "mutex's shared memory doesn't exist\n");
        fprintf(stdout, "creating the mutex's shared memory object...\n");
        mtx_shm_fd = shm_open(MTX_KEY, O_CREAT | O_EXCL | O_RDWR, 0600);
        CHECK_RESULT(mtx_shm_fd, "shm_open", 7);

        mtx_owner = TRUE;
        fprintf(stdout, "mutex's shared memory is created.\n");
    } else {
        fprintf(stderr, "failed to create mutex's shared memory");
        exit(8);
    }

    if (mtx_owner) {
        int result = ftruncate(mtx_shm_fd, sizeof(pthread_mutex_t));
        CHECK_RESULT(result, "ftruncate", 9);
    }

    void* map = mmap(0, sizeof(pthread_mutex_t), PROT_WRITE | PROT_READ, MAP_SHARED, mtx_shm_fd, 0);
    if (MAP_FAILED == map) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        exit(10);
    }

    mtx = (pthread_mutex_t*)map;

    if (mtx_owner) {
        pthread_mutexattr_t attr;
        int result = pthread_mutexattr_init(&attr);
        CHECK_RESULT(result, "pthread_mutexattr_init", 11);

        result = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        CHECK_RESULT(result, "pthread_mutexattr_setpshared", 12);

        result = pthread_mutex_init(mtx, &attr);
        CHECK_RESULT(result, "pthread_mutex_init", 13);

        result = pthread_mutexattr_destroy(&attr);
        CHECK_RESULT(result, "pthread_mutexattr_destroy", 14);
    }
}

static void shutdown_control()
{
    sleep(1);

    int result = -1;
    if (mtx_owner) {
        result = pthread_mutex_destroy(mtx);
        CHECK_RESULT(result, "pthread_mutex_destroy", 15);
    }

    result = munmap(mtx, sizeof(pthread_mutex_t));
    CHECK_RESULT(result, "munmap", 16);

    result = close(mtx_shm_fd);
    CHECK_RESULT(result, "close", 17);

    if (mtx_owner) {
        result = shm_unlink(MTX_KEY);
        CHECK_RESULT(result, "mutex shm_unlink", 18);
    }
}

static void init_shared_resource()
{
    cancel_flag_shm_fd = shm_open(SHM_KEY, O_RDWR, 0600);
    if (0 <= cancel_flag_shm_fd) {
        cancel_flag_shm_owner = FALSE;
        fprintf(stdout, "the shared memory object(%s) is opened\n", SHM_KEY);
    } else if (ENOENT == errno) {
        fprintf(stderr, "the shared memory doesn't exist.\n");
        fprintf(stdout, "creating the shared memory.\n");
        cancel_flag_shm_fd = shm_open(SHM_KEY, O_CREAT | O_EXCL | O_RDWR, 0600);
        CHECK_RESULT(cancel_flag_shm_fd, "shm_open", 1);

        cancel_flag_shm_owner = TRUE;
    }

    if (cancel_flag_shm_owner) {
        int result = ftruncate(cancel_flag_shm_fd, sizeof(bool_t));
        CHECK_RESULT(result, "ftruncate", 2);

        fprintf(stdout, "the memory region is truncated.\n");
    }

    void* map = mmap(0, sizeof(bool_t), PROT_WRITE, MAP_SHARED, cancel_flag_shm_fd, 0);
    if (MAP_FAILED == map) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        exit(3);
    }

    cancel_flag = (bool_t*)map;

    if (cancel_flag_shm_owner) {
        *cancel_flag = FALSE;
    }
}

static void shutdown_shared_resource()
{
    int result = munmap(cancel_flag, sizeof(bool_t));
    CHECK_RESULT(result, "munmap", 4);

    result = close(cancel_flag_shm_fd);
    CHECK_RESULT(result, "close", 5);

    if (cancel_flag_shm_owner) {
        sleep(1);

        result = shm_unlink(SHM_KEY);
        CHECK_RESULT(result, "shm_unlink", 6);

        fprintf(stdout, "/shm0 is unlinked.\n");
    }
}

static bool_t is_canceled()
{
    pthread_mutex_lock(mtx);
    bool_t temp = *cancel_flag;
    pthread_mutex_unlock(mtx);

    return temp;
}

static void cancel()
{
    pthread_mutex_lock(mtx);
    *cancel_flag = TRUE;
    pthread_mutex_unlock(mtx);
}

static void sigint_handler(int sig)
{
    fprintf(stdout, "handling int signal: %d\n", sig);
    cancel();
}

static void test()
{
    signal(SIGINT, sigint_handler);

    init_shared_resource();
    init_control();

    while (!is_canceled()) {
        fprintf(stdout, "working...\n");
        sleep(1);
    }

    fprintf(stdout, "cancel signal is received.\n");

    shutdown_control();
    shutdown_shared_resource();
}

//=============================================================================

void test_ch_18_cancel_flag()
{
    test();
}
