#include "ch_18_shared_cond.h"
#include "ch_18_shared_mutex.h"

#include "helper.h"

shared_cond_t* shared_cond_new()
{
    shared_cond_t* obj = (shared_cond_t*)malloc(sizeof(shared_cond_t));
    obj->shm = shared_mem_new();
    return obj;
}
void shared_cond_delete(shared_cond_t* obj)
{
    shared_mem_delete(obj->shm);
    free(obj);
}
void shared_cond_construct(shared_cond_t* obj, const char* name)
{
    shared_mem_construct(obj->shm, name, sizeof(pthread_cond_t));
    if (shared_mem_is_owner(obj->shm)) {
        pthread_condattr_t attr;
        int result = pthread_condattr_init(&attr);
        CHECK_RESULT(result, "pthread_condattr_init", 1);

        result = pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        CHECK_RESULT(result, "pthread_condattr_setpshared", 2);

        result = pthread_cond_init(obj->ptr, &attr);
        CHECK_RESULT(result, "pthread_cond_init", 3);

        result = pthread_condattr_destroy(&attr);
        CHECK_RESULT(result, "", 4);
    }
}
void shared_cond_destruct(shared_cond_t* obj)
{
    if (shared_mem_is_owner(obj->shm)) {
        int result = pthread_cond_destroy(obj->ptr);
        CHECK_RESULT(result, "pthread_cond_destroy", 5);
    }
    shared_mem_destruct(obj->shm);
}

pthread_cond_t* shared_cond_get_ptr(shared_cond_t* obj)
{

}

void shared_cond_wait(shared_cond_t* obj, shared_mutex_t* mtx)
{
    int result = pthread_cond_wait(obj->ptr, shared_mutex_get_ptr(mtx));
    CHECK_RESULT(result, "pthread_cond_wait", 6);
}
void shared_cond_timed_wait(shared_cond_t* obj, shared_mutex_t* mtx, long int t)
{
    //TODO
}
void shared_cond_broadcast(shared_cond_t* obj)
{
    int result = pthread_cond_broadcast(obj->ptr);
    CHECK_RESULT(result, "pthread_cond_broadcast", 7);
}
