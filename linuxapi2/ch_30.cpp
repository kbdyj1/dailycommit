#include <stdio.h>
#include <stdlib.h>
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

} //namespace =================================================================

void test_ch_30(int argc, const char* argv[])
{
    _1::test(argc, argv);
}
