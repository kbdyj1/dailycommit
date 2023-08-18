#include <sys/syscall.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define IOPRIO_CLASS_SHIFT  (13)
#define IOPRIO_PRIO_VALUE(Class,Data)   (((Class) << IOPRIO_CLASS_SHIFT) | Data)

enum {
    IOPRIO_WHO_PROCESS = 1,
    IOPRIO_WHO_PGRP,
    IOPRIO_WHO_USER
};

enum {
    IOPRIO_CLASS_NONE,
    IOPRIO_CLASS_RT,
    IOPRIO_CLASS_BE,
    IOPRIO_CLASS_IDLE,
};

void test_ioprio()
{
    // if not set, ioprio uses the value of priority
    syscall(SYS_ioprio_set, IOPRIO_WHO_PROCESS, 0, IOPRIO_PRIO_VALUE(IOPRIO_CLASS_IDLE, 0));

    int ret = setpriority(PRIO_PROCESS, 0, 10);   //-20(HIGH) ~ 20(LOW)
    if (!ret) {
        printf("setpriority(): %s", strerror(errno));
    }
}
