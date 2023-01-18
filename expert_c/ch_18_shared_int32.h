#ifndef CH_18_SHARED_INT32_H
#define CH_18_SHARED_INT32_H

#include "ch_18_shared_mem.h"

typedef struct {
    struct shared_mem_t* shm;
    int32_t* ptr;
} shared_int32_t;

shared_int32_t* shared_int32_new(const char* name);
void shared_int32_delete(shared_int32_t* obj);
void shared_int32_construct(shared_int32_t* obj, const char* name);
void shared_int32_destruct(shared_int32_t* obj);

void shared_int32_set_value(shared_int32_t* obj, int32_t value);
void shared_int32_set_value_if_owner(shared_int32_t* obj, int32_t value);
int32_t shared_int32_get_value(shared_int32_t* obj);

#endif // CH_18_SHARED_INT32_H
