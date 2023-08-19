#include <sys/syscall.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <QtGlobal>

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

#ifdef Q_OS_DARWIN
#   define SYS_ioprio_set  251
#   define SYS_ioprio_get  252
#   define  TEST_DEPRECATED_SYSCALL_ON_OSX
#endif

void test_ioprio()
{
#if (!defined(Q_OS_DARWIN) || defined(TEST_DEPRECATED_SYSCALL_ON_OSX))
    // on OSX, you can check the nice value, by installing htop

    // if not set, ioprio uses the value of priority
    int ret = syscall(SYS_ioprio_set, IOPRIO_WHO_PROCESS, 0, IOPRIO_PRIO_VALUE(IOPRIO_CLASS_IDLE, 0));
    if (0 != ret) {
        printf("syscall(SYS_ioprio_set) : %s\n", strerror(errno));
    }
    ret = setpriority(PRIO_PROCESS, 0, 10);   //-20(HIGH) ~ 20(LOW)
    if (0 != ret) {
        printf("setpriority(): %s", strerror(errno));
    } else {
        int prio = getpriority(PRIO_PROCESS, 0);
        printf("getpriority(): %d\n", prio);
    }
#endif
}
