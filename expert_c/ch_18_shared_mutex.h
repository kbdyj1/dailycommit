#ifndef CH_18_SHARED_MUTEX_H
#define CH_18_SHARED_MUTEX_H

#include "ch_18_shared_mem.h"
#include <pthread.h>

typedef struct {
    shared_mem_t* shm;
    pthread_mutex_t* ptr;
} shared_mutex_t;

shared_mutex_t* shared_mutex_new();
void shared_mutex_delete(shared_mutex_t* obj);
void shared_mutex_construct(shared_mutex_t* obj, const char* name);
void shared_mutex_destruct(shared_mutex_t* obj);

pthread_mutex_t* shared_mutex_get_ptr(shared_mutex_t* obj);

void shared_mutex_lock(shared_mutex_t* obj);
void shared_mutex_unlock(shared_mutex_t* obj);

#if !defined(__APPLE__)
void shared_mutex_make_consistent(shared_mutex_t* obj);
#endif

#endif // CH_18_SHARED_MUTEX_H
