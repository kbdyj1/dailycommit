#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

#include "helper.h"

//=============================================================================

#define SHM_SIZE    4

#define MTX_KEY     "/mtx0"
#define SHM_KEY     "/shm0"

static int mtx_shm_fd = -1;
static int shm_fd = -1;

static int32_t* counter = NULL;

static pthread_mutex_t* mtx = NULL;

static void init_control()
{
    mtx_shm_fd = shm_open(MTX_KEY, O_CREAT | O_RDWR, 0600);
    CHECK_RESULT(mtx_shm_fd, "shm_open(mutex)", 1);

    int result = ftruncate(mtx_shm_fd, sizeof(pthread_mutex_t));
    CHECK_RESULT(result, "ftruncate", 2);

    void* map = mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mtx_shm_fd, 0);
    if (MAP_FAILED == map) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        exit(3);
    }

    mtx = (pthread_mutex_t*)map;

    pthread_mutexattr_t attr;
    result = pthread_mutexattr_init(&attr);
    CHECK_RESULT(result, "pthread_mutexattr_init", 4);

    result = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);   // <----- important !!!
    CHECK_RESULT(result, "pthread_mutexattr_setpshared", 5);

    result = pthread_mutex_init(mtx, &attr);
    CHECK_RESULT(result, "pthread_mutex_init", 6);

    result = pthread_mutexattr_destroy(&attr);
    CHECK_RESULT(result, "pthread_mutexattr_destroy", 7);
}

static void shutdown_control()
{
    int result = -1;
    result = pthread_mutex_destroy(mtx);
    CHECK_RESULT(result, "pthread_mutex_destroy", 8);

    result = munmap(mtx, sizeof(pthread_mutex_t));
    CHECK_RESULT(result, "munmap", 9);

    result = close(mtx_shm_fd);
    CHECK_RESULT(result, "close", 10);

    result = shm_unlink(MTX_KEY);
    CHECK_RESULT(result, "shm_unlink", 11);
}

static void inc_counter()
{
    usleep(1);
    pthread_mutex_lock(mtx);
    {
        int32_t temp = *counter;
        usleep(1);

        temp++;

        usleep(1);
        *counter = temp;
    }
    pthread_mutex_unlock(mtx);
    usleep(1);
}

int init_shared_memory(const char* key);
void shutdown_shared_memory(const char* key);

static void init_shared_resource()
{
    shm_fd = init_shared_memory(SHM_KEY);
}

static void shutdown_shared_resource()
{
    shutdown_shared_memory(SHM_KEY);
}

static void test()
{
    init_control();
    init_shared_resource();

    int result = ftruncate(shm_fd, SHM_SIZE*sizeof(char));
    CHECK_RESULT(result, "ftruncate", 3);

    fprintf(stdout, "the memory region truncated.\n");

    void* map = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (MAP_FAILED == map) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        exit(4);
    }

    counter = (int32_t*)map;
    *counter = 0;

    pid_t pid = fork();
    if (pid) {
        inc_counter();
        fprintf(stdout, "[0] parent inc counter as %d\n", *counter);

        int status = -1;
        wait(&status);

        fprintf(stdout, "[0] child process finished with status %d.\n", status);
    } else {
        inc_counter();
        fprintf(stdout, "[1] child process inc counter as %d\n", *counter);
    }

    result = munmap(counter, SHM_SIZE);
    CHECK_RESULT(result, "munmap", 5);

    result = close(shm_fd);
    CHECK_RESULT(result, "close", 6);

    if (pid) {
        shutdown_shared_resource();
        shutdown_control();
    }
}

//=============================================================================

void test_ch_18_mutex()
{
    test();
}
