#include "ch_18_shared_mutex.h"

#include <stdio.h>
#include <errno.h>

shared_mutex_t* shared_mutex_new()
{
    return (shared_mutex_t*)malloc(sizeof(shared_mutex_t));
}

void shared_mutex_delete(shared_mutex_t* obj)
{
    shared_mem_delete(obj->shm);
    free(obj);
}

void shared_mutex_construct(shared_mutex_t* obj, const char* name)
{
    shared_mem_construct(obj->shm, name, sizeof(pthread_mutex_t));
    obj->ptr = shared_mem_get_ptr(obj->shm);
    if (shared_mem_is_owner(obj->shm)) {
        pthread_mutexattr_t attr;
        int result = pthread_mutexattr_init(&attr);
        if (result != 0) {
            fprintf(stderr, "pthread_mutexattr_init() failed: %s.\n", strerror(errno));
            exit(1);
        }

#if !defined(__APPLE__)
        result = pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
        if (0 != result) {
            fprintf(stderr, "pthread_mutexattr_setrubust() failed: %s\n", strerror(errno));
            exit(2);
        }
#endif

        result = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        if (0 != result) {
            fprintf(stderr, "pthread_mutexattr_setpshared() failed: %s.\n", strerror(errno));
            exit(3);
        }

        result = pthread_mutexattr_destroy(&attr);
        if (0 != result) {
            fprintf(stderr, "pthread_mutexattr_destroy() failed: %s.\n", strerror(errno));
            exit(4);
        }
    }
}

void shared_mutex_destruct(shared_mutex_t* obj)
{
    if (shared_mem_is_owner(obj->shm)) {
        int result = pthread_mutex_destroy(obj->ptr);
        if (0 != result) {
            fprintf(stderr, "pthread_mutex_destroy() failed: %s.\n", strerror(errno));
            exit(5);
        }
    }
    shared_mem_destruct(obj->shm);
}

pthread_mutex_t* shared_mutex_get_ptr(shared_mutex_t* obj)
{
    return obj->ptr;
}

void shared_mutex_lock(shared_mutex_t* obj)
{
    int result = pthread_mutex_lock(obj->ptr);

#if !defined(__APPLE__)
    if (result == EOWNERDEAD) {
        fprintf(stderr, "the owner of the mutex is dead...\n");
        shared_mutex_make_consistent(obj);
        fprintf(stdout, "new owner.\n");
        shared_mem_set_owner(obj->shm, TRUE);
        return;
    }
#endif

    if (0 != result) {
        fprintf(stderr, "pthread_mutex_lock() failed: %s.\n", strerror(errno));
        exit(7);
    }
}

void shared_mutex_unlock(shared_mutex_t* obj)
{
    int result = pthread_mutex_unlock(obj->ptr);
    if (0 != result) {
        fprintf(stderr, "shared_mutex_unlock() failed: %s.\n", strerror(errno));
        exit(8);
    }
}

#if !defined(__APPLE__)
void shared_mutex_make_consistent(shared_mutex_t* obj)
{
    int result = pthread_mutex_consistent(obj->ptr);
    if (0 != result) {
        fprintf(stderr, "pthread_mutex_consistent() failed: %s.\n", strerror(errno));
        exit(6);
    }
}
#endif
