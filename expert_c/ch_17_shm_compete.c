#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "helper.h"

//=============================================================================

#define USE_FILE

#define SHM_SIZE    4

#if defined(USE_FILE)
#   define SHM_KEY     "shm1.bin"
#else
#   define SHM_KEY     "/shm1"
#endif

static int shfd = -1;

static int32_t* counter = NULL;

static void init_shared_resource()
{
#if defined(USE_FILE)
    shfd = open(SHM_KEY, O_CREAT | O_RDWR, 0600);
    CHECK_RESULT(shfd, "open", 1);
#else
    shfd = shm_open(SHM_KEY, O_CREAT | O_RDWR, 0600);
    CHECK_RESULT(shfd, "shm_open", 1);
#endif

    fprintf(stdout, "shared memory is created width fd: %d\n", shfd);
}

static void shutdown_shared_resource()
{
#if defined(USE_FILE)
    int result = remove(SHM_KEY);
    CHECK_RESULT(result, "remove", 2);
#else
    int result = shm_unlink(SHM_KEY);
    CHECK_RESULT(result, "shm_unlink", 2);
#endif
}

static void inc_counter()
{
    usleep(1);

    int32_t temp = *counter;

    usleep(1);

    temp++;

    usleep(1);

    *counter = temp;

    usleep(1);
}

static void _1_test()
{
    init_shared_resource();

    int result = ftruncate(shfd, SHM_SIZE*sizeof(char));
    CHECK_RESULT(result, "ftruncate", 3);

    fprintf(stdout, "the memory region truncated.\n");

    void* map = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shfd, 0);
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

    result = close(shfd);
    CHECK_RESULT(result, "close", 6);

    if (pid) {
        shutdown_shared_resource();
    }
}

//=============================================================================

void test_ch_17_shm_compete()
{
    _1_test();
}
