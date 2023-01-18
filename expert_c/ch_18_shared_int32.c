#include "ch_18_shared_int32.h"

shared_int32_t* shared_int32_new(const char* name)
{
    shared_int32_t* obj = (shared_int32_t*)malloc(sizeof(shared_int32_t));
    obj->shm = shared_mem_new();
    return obj;
}

void shared_int32_delete(shared_int32_t* obj)
{
    shared_mem_delete(obj->shm);
    free(obj);
}

void shared_int32_construct(shared_int32_t* obj, const char* name)
{
    shared_mem_construct(obj->shm, name, sizeof(int32_t));
    obj->ptr = (int32_t*)shared_mem_get_ptr(obj->shm);
}

void shared_int32_destruct(shared_int32_t* obj)
{
    shared_mem_destruct(obj->shm);
}

void shared_int32_set_value(shared_int32_t* obj, int32_t value)
{
    *(obj->ptr) = value;
}

void shared_int32_set_value_if_owner(shared_int32_t* obj, int32_t value)
{
    if (shared_mem_is_owner(obj->shm)) {
        *(obj->ptr) = value;
    }
}

int32_t shared_int32_get_value(shared_int32_t* obj)
{
    return *(obj->ptr);
}
