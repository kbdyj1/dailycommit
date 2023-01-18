#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

#include "common_def.h"
#include "ch_18_shared_mem.h"
#include "helper.h"

//=============================================================================

static bool_t owner_process_set = FALSE;
static bool_t owner_process = FALSE;

shared_mem_t* shared_mem_new()
{
    return (shared_mem_t*)malloc(sizeof(shared_mem_t));
}

void shared_mem_delete(shared_mem_t* shm)
{
    free(shm->name);
    free(shm);
}

void shared_mem_construct(shared_mem_t* shm, const char* name, size_t size)
{
    shm->size = size;
    shm->name = (char *)malloc(strlen(name) + 1);
    strcpy(shm->name, name);
    shm->shm_fd = shm_open(shm->name, O_RDWR, 0600);
    if (0 <= shm->shm_fd) {
        if (!owner_process_set) {
            owner_process = FALSE;
            owner_process_set = TRUE;
        }
        printf("the shared memory %s is opened.\n", shm->name);
    } else if (ENOENT == errno) {
        printf("the shared memory %s does not exist.\n");
        shm->shm_fd = shm_open(shm->name, O_CREAT | O_EXCL | O_RDWR, 0600);
        if (0 <= shm->shm_fd) {
            if (!owner_process_set) {
                owner_process = TRUE;
                owner_process_set = TRUE;
            }
            printf("the shared memory %s is created.\n", shm->name);
            int result = ftruncate(shm->shm_fd, shm->size);
            CHECK_RESULT(result, "ftruncate", 1);
        } else {
            fprintf(stderr, "failed to create failed. %s\n", strerror(errno));
            exit(2);
        }
    } else {
        fprintf(stderr, "failed to create shared memory: %s\n", strerror(errno));
        exit(3);
    }

    shm->map_ptr = mmap(0, shm->size, PROT_READ | PROT_WRITE, MAP_SHARED, shm->shm_fd, 0);
    if (MAP_FAILED == shm->map_ptr) {
        fprintf(stdout, "mmap failed: %s\n", strerror(errno));
        exit(4);
    }
    shm->ptr = (char*)shm->map_ptr;
}

void shared_mem_destruct(shared_mem_t* shm)
{
    int result = munmap(shm->shm_fd, shm->size);
    CHECK_RESULT(result, "munmap", 5);

    printf("the shared memory %s is unmapped.\n", shm->name);

    result = close(shm->shm_fd);
    CHECK_RESULT(result, "close", 6);

    printf("the shared memory %s is closed.\n", shm->name);

    if (owner_process) {
        result = shm_unlink(shm->name);
        CHECK_RESULT(result, "shm_unlink", 7);

        printf("the shared memory %s is deleted.\n", shm->name);
    }
}

char* shared_mem_get_ptr(shared_mem_t* shm)
{
    return shm->ptr;
}

bool_t shared_mem_is_owner(shared_mem_t* shm)
{
    return owner_process;
}

void shared_mem_set_owner(shared_mem_t* shm, bool_t is_owner)
{
    owner_process = is_owner;
}

//=============================================================================

