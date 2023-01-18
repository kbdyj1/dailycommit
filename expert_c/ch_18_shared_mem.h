#ifndef CH_18_SHARED_MEM_H
#define CH_18_SHARED_MEM_H

#include "common_def.h"
#include <stdlib.h>

typedef struct  {
    char* name;
    int shm_fd;
    void* map_ptr;
    char* ptr;
    size_t size;
} shared_mem_t;

shared_mem_t* shared_mem_new();
void shared_mem_delete(shared_mem_t* shm);
void shared_mem_construct(shared_mem_t* shm, const char* name, size_t size);
void shared_mem_destruct(shared_mem_t* shm);

char* shared_mem_get_ptr(shared_mem_t* shm);
bool_t shared_mem_is_owner(shared_mem_t* shm);
void shared_mem_set_owner(shared_mem_t* shm, bool_t is_owner);

#endif // CH_18_SHARED_MEM_H
