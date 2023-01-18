#ifndef CH_18_SHARED_COND_H
#define CH_18_SHARED_COND_H

#include "ch_18_shared_mem.h"
#include "ch_18_shared_mutex.h"
#include <pthread.h>

typedef struct {
    struct shared_mem_t* shm;
    pthread_cond_t* ptr;
} shared_cond_t;

shared_cond_t* shared_cond_new();
void shared_cond_delete(shared_cond_t* obj);
void shared_cond_construct(shared_cond_t* obj, const char* name);
void shared_cond_destruct(shared_cond_t* obj);

pthread_cond_t* shared_cond_get_ptr(shared_cond_t* obj);

void shared_cond_wait(shared_cond_t* obj, shared_mutex_t* mtx);
void shared_cond_timed_wait(shared_cond_t* obj, shared_mutex_t* mtx, long int t);
void shared_cond_broadcast(shared_cond_t* obj);

#endif // CH_18_SHARED_COND_H
