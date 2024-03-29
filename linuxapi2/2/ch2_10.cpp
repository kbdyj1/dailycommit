#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <errno.h>
#include <memory.h>
#include <unistd.h>

#include "utils.h"
#include "semun.h"

namespace { //=================================================================

namespace _1 {

void test(int argc, const char* argv[])
{
    int id;

    if (2 == argc) {
        id = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
        if (-1 == id) {
            errorExit("semget() error.\n");
        }

        int value = atoi(argv[1]);
        if (-1 == semctl(id, 0, SETVAL, value)) {
            errorExit("semctl(SET_VAL) error.\n");
        }

        printf("semaphore id: %d\n", id);
    } else {
        int id = atoi(argv[1]);

        sembuf sop;
        sop.sem_num = 0;
        sop.sem_op = atoi(argv[2]);
        sop.sem_flg = 0;

        long pid = getpid();

        printf("[%ld] before semop\n", pid);

        if (-1 == semop(id, &sop, 1)) {
            errorExit("[%ld] semop() error.\n", pid);
        }

        printf("[%ld] after semop\n", pid);
    }

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test(int argc, const char* argv[])
{
    semid_ds ds;
    semun arg, dummy;
    int id;

    id = atoi(argv[1]);
    arg.buf = &ds;

    if (-1 == semctl(id, 0, IPC_STAT, arg)) {
        errorExit("semctl(IPC_STAT) error.\n");
    }

    arg.array = (unsigned short*) calloc(ds.sem_nsems, sizeof(arg.array[0]));
    if (NULL == arg.array) {
        errorExit("calloc() error.\n");
    }
    if (-1 == semctl(id, 0, GETALL, arg)) {
        errorExit("semctl(GETALL) error.\n");
    }

    printf("Sem #  Value  SEMPID  SEMNCNT  SEMZCNT\n");

    for (int j=0; j<ds.sem_nsems; j++) {
        printf("%3d   %5d   %5d   %5d   %5d\n", j, arg.array[j],
               semctl(id, j, GETPID, dummy),
                semctl(id, j, GETNCNT, dummy),
                semctl(id, j, GETZCNT, dummy));
    }

    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s semid value.\n", argv[0]);
        exit(-1);
    }
    semid_ds ds;
    semun arg;
    int id = atoi(argv[1]);
    if (-1 == id) {
        errorExit("atoi() error.\n");
    }

    arg.buf = &ds;

    if (-1 == semctl(id, 0, IPC_STAT, arg)) {
        errorExit("semctl() error.\n");
    }

    if (ds.sem_nsems != argc -2) {
        fprintf(stderr, "set contains %ld semaphores, but %d values were supplied.\n", (long)ds.sem_nsems, argc - 2);
        exit(-2);
    }

    arg.array = (unsigned short*)calloc(ds.sem_nsems, sizeof(arg.array[0]));
    if (NULL == arg.array) {
        errorExit("arg.array is NULL");
    }

    for (int j=2; j<argc; j++) {
        arg.array[j-2] = atoi(argv[j]);
    }

    if (-1 == semctl(id, 0, SETALL, arg)) {
        errorExit("semctl() #2 error.\n");
    }

    exit(EXIT_SUCCESS);
}

} //_3 --------------------------------------------------------------

namespace _4 {

void sigHandle(int sig)
{
    printf("signal %d.\n", sig);
}

void test()
{
    signal(SIGTERM, sigHandle);

    int id = semget(IPC_PRIVATE, 3, S_IRUSR | S_IWUSR);
    sembuf sops[3];
    sops[0] = {0, -1, 0};
    sops[1] = {1, 2, 0};
    sops[2] = {2, 0, IPC_NOWAIT};

    printf("pid: %ld\n", (long)getpid());

    if (-1 == semop(id, sops, 3)) {
        if (EAGAIN == errno) {
            printf("operation would have blocked\n");
        } else if (EINTR == errno) {
            printf("operation would have interrupted.\n");
        } else {
            errorExit("semop() error.\n");
        }
    }
}

} //_4 --------------------------------------------------------------

namespace _5 {

bool bUseSemUndo = false;
bool bRetryOnInterrupt = true;

int initSemAvailable(int id, int no)
{
    semun arg;

    arg.val = 1;

    return semctl(id, no, SETVAL, arg);
}

int initSemInUse(int id, int no)
{
    semun arg;

    arg.val = 0;

    return semctl(id, no, SETVAL, arg);
}

int reserveSem(int id, int no)
{
    sembuf sops;

    sops.sem_num = no;
    sops.sem_op = -1;
    sops.sem_flg = bUseSemUndo ? SEM_UNDO : 0;

    while (-1 == semop(id, &sops, 1)) {
        if (errno != EINTR || !bRetryOnInterrupt)
            return -1;
    }
    return 0;
}

int releaseSem(int id, int no)
{
    sembuf sops;

    sops.sem_num = no;
    sops.sem_op = 1;
    sops.sem_flg = bUseSemUndo ? SEM_UNDO : 0;

    return semop(id, &sops, 1);
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_10(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
    _2::test(argc, argv);
    _3::test(argc, argv);
#endif

    _4::test();
}

int ch_10_initSemAvailable(int id, int no)
{
    return _5::initSemAvailable(id, no);
}

int ch_10_initSemInUse(int id, int no)
{
    return _5::initSemInUse(id, no);
}

int ch_10_reserveSem(int id, int no)
{
    return _5::reserveSem(id, no);
}

int ch_10_releaseSem(int id, int no)
{
    return _5::releaseSem(id, no);
}
