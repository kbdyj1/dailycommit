#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#include "helper.h"

//=============================================================================

#define SHM_SIZE    4

#define MTX_KEY     "/mtx0"

static int mtx_fd = -1;
static int shm_fd = -1;

static int32_t* counter = NULL;

static pthread_mutex_t* mtx = NULL;

static void init_control()
{
    mtx_fd = shm_open(MTX_KEY, O_CREAT | O_RDWR, 0600);
    CHECK_RESULT(mtx_fd, "shm_open(mutex)", 1);
}

//=============================================================================

void test_ch_18_mutex()
{

}
