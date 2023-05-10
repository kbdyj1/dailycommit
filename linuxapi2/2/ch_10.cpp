#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"

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

} //namespace =================================================================

void exec_ch_10(int argc, const char* argv[])
{
    _1::test(argc, argv);
}
