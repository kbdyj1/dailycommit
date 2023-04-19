#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>

#include "utils.h"

namespace { //=================================================================

namespace _1 {

void printUsage(const char* program)
{
    printf("usage: %s {p|g|u} who priority\n", program);
}

void test(int argc, const char* argv[])
{
    if (4 != argc) {
        printUsage(argv[0]);
        exit(-1);
    }

    int which;
    char c = argv[1][0];

    switch (c)
    {
    case 'p':
        which = PRIO_PROCESS;
        break;
    case 'g':
        which = PRIO_PGRP;
        break;
    case 'u':
        which = PRIO_USER;
        break;
    default:
        printUsage(argv[0]);
        exit(-1);
        break;
    }

    id_t who = atoi(argv[2]);
    int prio = atoi(argv[3]);

    if (-1 == setpriority(which, who, prio)) {
        errorExit("setpriority() error.\n");
    }

    errno = 0;
    prio = getpriority(which, who);
    if (-1 == prio && 0 != errno) {
        errorExit("getpriority() error.\n");
    }

    printf("nice value: %d\n", prio);

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    int policies[] = {
        SCHED_OTHER,
        SCHED_FIFO,
        SCHED_RR,
        SCHED_BATCH,
        SCHED_ISO,
        SCHED_IDLE,
        SCHED_DEADLINE,
    };
    const int numPolicies = SCHED_DEADLINE + 1;

    for (int i=0; i<numPolicies; i++) {
        int policy = policies[i];
        int min = sched_get_priority_min(policy);
        int max = sched_get_priority_max(policy);

        printf("[%d] min: %d, max: %d\n", i, min, max);
    }
}

} //_2 --------------------------------------------------------------

namespace _3 {

void set_rtprio(int argc, const char* argv[])
{
    int j;
    int policy;
    struct sched_param sp;

    if (argc < 3 || strchr("rfo", argv[1][0]) == NULL) {
        printf("Usage: %s policy priority [pid...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    switch (argv[1][0]) {
    case 'r': policy = SCHED_RR;    break;
    case 'f': policy = SCHED_FIFO;  break;
    case 'o': policy = SCHED_OTHER; break;
    default:
        printf("illigal state.\n");
        exit(EXIT_FAILURE);
    }

    sp.sched_priority = atoi(argv[2]);

    for (j=3; j<argc; j++) {
        pid_t pid = atoi(argv[j]);
        if (-1 == sched_setscheduler(pid, policy, &sp)) {
            fprintf(stderr, "[ERROR] errno: %d (%s)\n", errno, strerror(errno));
            errorExit("sched_setscheduler() error.\n");
        }
    }
}

void get_rtprio(int argc, const char* argv[])
{
    int j;
    int policy;
    struct sched_param sp;

    for (int j=1; j<argc; j++) {
        auto pid = (pid_t)atoi(argv[j]);
        policy = sched_getscheduler(pid);
        if (-1 == policy) {
            errorExit("sched_getscheduler() error.\n");
        }
        if (-1 == sched_getparam(pid, &sp)) {
            errorExit("sched_getparam() error.\n");
        }

        printf("PID: %ld, policy: %d, priority: %d\n", (long)pid, policy, sp.sched_priority);
    }
}

void test(int argc, const char* argv[])
{
#if (0)
    set_rtprio(argc, argv);
#else
    get_rtprio(argc, argv);
#endif

    exit(EXIT_SUCCESS);
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test(int argc, const char* argv[])
{
    pid_t pid = atoi(argv[1]);

    struct timespec ts;

    for ( ;; ) {
        if (-1 == sched_rr_get_interval(pid, &ts)) {
            errorExit("sched_rr_get_interval() error.\n");
        }

        printf("PID: %d interval: %lds %ldns\n", pid, ts.tv_sec, ts.tv_nsec);

        sleep(1);
    }
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        printf("Usage: %s pid.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = atoi(argv[1]);
    cpu_set_t set;
    size_t len = sizeof(cpu_set_t);
    if (-1 == sched_getaffinity(pid, len, &set)) {
        errorExit("sched_getaffinity() error.\n");
    }

    printf("PID: %ld CPU Affinity ------------------------\n", (long)pid);
    int cpuCnt = CPU_COUNT(&set);
    for (int i=0; i<cpuCnt; i++) {
        char c = CPU_ISSET(i, &set) ? 'v' : ' ';
        printf("[%c] CPU-%d\n", c, i);
    }
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_30(int argc, const char* argv[])
{
    (void)argc;
    (void)argv;

#if (0) //done
    _1::test(argc, argv);
    _2::test();
    _3::test(argc, argv);
    _4::test(argc, argv);
#endif

    _5::test(argc, argv);
}
