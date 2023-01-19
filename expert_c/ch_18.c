#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "helper.h"

int init_semaphore(sem_t* semaphore)
{
    if (SEM_FAILED == semaphore) {
        fprintf(stderr, "sem_open failed: %s\n", strerror(errno));
        return errno;
    } else {
        return 0;
    }
}

int shutdown_semaphore(sem_t* semaphore, const char* key)
{
    int result = sem_close(semaphore);
    CHECK_RESULT(result, "sem_close", 2);

    result = sem_unlink(key);
    CHECK_RESULT(result, "sem_unlink", 3);

    return 0;
}

int init_shared_memory(const char* key)
{
    int fd = shm_open(key, O_CREAT | O_RDWR, 0600);
    CHECK_RESULT(fd, "shm_open", 1);

    fprintf(stdout, "shared memory is created width fd: %d\n", fd);

    return fd;
}

void shutdown_shared_memory(const char* key)
{
    int result = shm_unlink(key);
    CHECK_RESULT(result, "shm_unlink", 2);
}

//=============================================================================

#define SHM_SIZE    4

#define SEM_KEY     "/sem0"
#define SHM_KEY     "/shm0"

static int shfd = -1;
static int32_t* counter = NULL;
static sem_t* sem = NULL;

static void init_control()
{
    sem = sem_open(SEM_KEY, O_CREAT | O_EXCL, 0600, 1);
    int result = init_semaphore(sem);
    if (EEXIST == result) {
        sem_unlink(SEM_KEY);
    }
    if (result) {
        exit(1);
    }
}

static void shutdown_control()
{
    shutdown_semaphore(sem, SEM_KEY);
}

static void init_shared_resource()
{
    shfd = init_shared_memory(SHM_KEY);
}

static void shutdown_shared_resource()
{
    shutdown_shared_memory(SHM_KEY);
}

static void inc_counter()
{
    usleep(1);
    sem_wait(sem);
    {
        int32_t temp = *counter;
        usleep(1);

        temp++;

        usleep(1);
        *counter = temp;
    }
    sem_post(sem);
    usleep(1);
}

static void test()
{
    init_control();
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
        shutdown_control();
    }
}

//=============================================================================

void test_ch_18_mutex();
void test_ch_18_cancel_flag();
void test_ch_18_shared_test();

void test_ch_18(int argc, char** argv)
{
#if (0) //done
    test();
    test_ch_18_mutex();
    test_ch_18_cancel_flag();
#endif

    test_ch_18_shared_test(argc, argv);
}
