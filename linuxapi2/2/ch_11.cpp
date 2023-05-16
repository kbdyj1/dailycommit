#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
#include "semun.h"

int ch_10_initSemAvailable(int id, int no);
int ch_10_initSemInUse(int id, int no);
int ch_10_reserveSem(int id, int no);
int ch_10_releaseSem(int id, int no);

namespace { //=================================================================

namespace _1 {

void test()
{
#if (1)
    int flags = S_IRUSR | S_IWUSR;
#else
    int flags = 0;  //Permission denied
#endif

    int id = shmget(IPC_PRIVATE, 256, flags);
    if (-1 == id) {
        errorExit("shmget() error");
    }

    void* p = shmat(id, NULL, SHM_RDONLY);
    if ((void*)-1 == p) {
        char* str = strerror(errno);
        fprintf(stderr, "%s\n",  str);
    } else {
        printf("shmat(): %p\n", p);

        //*(int*)p = 0; //segment fault
    }

    shmdt(p);
}

} //_1 --------------------------------------------------------------

namespace _2 {

const int SHM_KEY = 0x1234;
const int SEM_KEY = 0x5678;
const int PERMS = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
const int WRITE_SEM = 0;
const int READ_SEM = 1;
const int BUF_SIZE = 1024;

struct ShmSeg {
    int cnt;
    char buf[BUF_SIZE];
};

void sender()
{
    int semid = semget(SEM_KEY, 2, IPC_CREAT | PERMS);
    if (-1 == semid) {
        errorExit("semget() error.\n");
    }
    if (-1 == ch_10_initSemAvailable(semid, WRITE_SEM)) {
        errorExit("initSemAvailable(id, WRITE_SEM) error.\n");
    }
    if (-1 == ch_10_initSemInUse(semid, READ_SEM)) {
        errorExit("initSemInUse(id, READ_SEM) error.\n");
    }

    int shmid = shmget(SHM_KEY, sizeof(ShmSeg), IPC_CREAT | PERMS);
    if (-1 == shmid) {
        errorExit("shmget() error.\n");
    }
    ShmSeg* pseg = (ShmSeg*)shmat(shmid, NULL, 0);
    if ((void*)-1 == pseg) {
        errorExit("shmat() error.\n");
    }

    int bytes = 0;
    int i;
    for (i=0; ; i++) {
        bytes += pseg->cnt;

        if (-1 == ch_10_reserveSem(semid, WRITE_SEM)) {
            errorExit("reserveSem(WRITE_SEM) error.\n");
        }

        pseg->cnt = read(STDIN_FILENO, pseg->buf, BUF_SIZE);
        if (-1 == pseg->cnt) {
            errorExit("read(STDIN_FILENO, ...) error.\n");
        } else {
            if (1 == pseg->cnt && pseg->buf[0] == '\n') {
                pseg->cnt = 0;
            }
        }
        if (-1 == ch_10_releaseSem(semid, READ_SEM)) {
            errorExit("releaseSem(READ_SEM) error.\n");
        }

        if (0 == pseg->cnt)
            break;
    }

    if (-1 == ch_10_reserveSem(semid, WRITE_SEM)) {
        errorExit("ch_10_reserveSem() error. #2\n");
    }

    semun dummy;
    if (-1 == semctl(semid, 0, IPC_RMID, dummy)) {
        errorExit("semctl(IPC_RMID) error.\n");
    }
    if (-1 == shmdt(pseg)) {
        errorExit("shmdt() error.\n");
    }
    if (-1 == shmctl(shmid, IPC_RMID, 0)) {
        errorExit("shmctl(IPC_RMID) error.\n");
    }

    fprintf(stderr, "sent %d bytes (%d xfrs)\n", bytes, i);
    exit(EXIT_SUCCESS);
}

void receiver()
{
    int semid = semget(SEM_KEY, 0, 0);
    if (-1 == semid) {
        errorExit("semget() error.\n");
    }
    int shmid = shmget(SHM_KEY, 0, 0);
    if (-1 == shmid) {
        errorExit("shmget() error.\n");
    }
    ShmSeg* pseg = (ShmSeg*)shmat(shmid, NULL, SHM_RDONLY);
    if ((void*)-1 == pseg) {
        errorExit("shtat() error.\n");
    }

    int i=0;
    int bytes = 0;
    for ( ;; i++) {
        if (-1 == ch_10_reserveSem(semid, READ_SEM)) {
            errorExit("reserveSem() error.\n");
        }
        if (0 == pseg->cnt)
            break;

        bytes += pseg->cnt;

        if (pseg->cnt != write(STDOUT_FILENO, pseg->buf, pseg->cnt)) {
            errorExit("fatal! write(STDOUT_FILENO, ...) failed.\n");
        }
        if (-1 == ch_10_releaseSem(semid, WRITE_SEM)) {
            errorExit("releaseSem() error.\n");
        }
    }

    if (-1 == shmdt(pseg)) {
        errorExit("shmdt() error.\n");
    }
    if (-1 == ch_10_releaseSem(semid, WRITE_SEM)) {
        errorExit("releaseSem() error.\n");
    }

    fprintf(stderr, "received: %d bytes (%d xfrs)\n", bytes, i);
    exit(EXIT_SUCCESS);
}

void test(int argc, const char* argv[])
{
    if (1 < argc && 0 == strcmp(argv[1], "r")) {
        receiver();
    } else {
        sender();
    }
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_11(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
#endif

    _2::test(argc, argv);
}
