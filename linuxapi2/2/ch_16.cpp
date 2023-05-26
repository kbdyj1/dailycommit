#include <fcntl.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

#define SEM_KEY "/test"

namespace { //=================================================================

namespace _1 {

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s sem-name.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int oFlag = O_CREAT;
    bool post = false;
    bool wait = false;

    if (2 < argc) {
        char opt = argv[2][0];

        printf("argv[2][0]: %c\n", opt);

        switch (opt) {
        case 'u':
            printf("sem_unlink(%s)\n", argv[1]);

            if (-1 == sem_unlink(argv[1])) {
                errorExit("sem_unlink() failed.\n");
            }
            exit(EXIT_SUCCESS);

        case 'x':
            oFlag |= O_EXCL;
            break;

        case 'p':
            post = true;
            break;

        case 'w':
            wait = true;
            break;

        default:
            break;
        }
    }

    mode_t mode = S_IRUSR | S_IWUSR;
    sem_t* sem = sem_open(argv[1], oFlag, mode, 0);
    if (SEM_FAILED == sem) {
        errorExit("sem_open() failed.\n");
    }

    if (post) {
        if (-1 == sem_post(sem)) {
            errorExit("sem_post() failed.\n");
        } else {
            printf("post done!\n");
        }
    }

    if (wait) {
        if (-1 == sem_wait(sem)) {
            errorExit("sem_wait() failed.\n");
        } else {
            printf("wait done!\n");
        }
    }

    int val;
    if (-1 == sem_getvalue(sem, &val)) {
        errorExit("sem_getval() failed.\n");
    } else {
        printf("sem_getvalue(): %d\n", val);
    }

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

#define USE_SEMAPHORE

namespace _2 {

int global = 0;
sem_t sem;

void* threadFunc(void* arg)
{
    int local = global;
    int loops = *((int*)arg);
    for (int i=0; i<loops; i++) {
#if defined(USE_SEMAPHORE)
        if (-1 == sem_wait(&sem)) {
            errorExit("threadFunc sem_wait() failed.\n");
        }
#endif
        local = global;
        local++;
        global = local;

#if defined(USE_SEMAPHORE)
        if (-1 == sem_post(&sem)) {
            errorExit("threadFunc sem_post() failed.\n");
        }
#endif
    }
    return NULL;
}

void test(int argc, const char* argv[])
{
    pthread_t t0;
    pthread_t t1;
    int loops = (argc < 2) ? 0 : atoi(argv[1]);

    if (0 == loops) {
        printf("SEM_NSEMS_MAX: %ld\n", sysconf(_SC_SEM_NSEMS_MAX));
        printf("SEM_VALUE_MAX: %ld\n", sysconf(_SC_SEM_VALUE_MAX));
        exit(EXIT_SUCCESS);
    }

#if defined(USE_SEMAPHORE)
    if (-1 == sem_init(&sem, 0, 1))
        errorExit("sem_init() failed.\n");
#endif

    int s = pthread_create(&t0, NULL, threadFunc, &loops);
    if (0 != s) {
        errnoExit("pthread_create(t0): ", s);
    }

    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (0 != s) {
        errnoExit("pthread_create(t1): ", s);
    }

    s = pthread_join(t0, NULL);
    if (0 != s) {
        errnoExit("pthread_join(t0): ", s);
    }

    s = pthread_join(t1, NULL);
    if (0 != s) {
        errnoExit("pthread_join(t1): ", s);
    }

    sem_destroy(&sem);

    printf("global: %d\n", global);
    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_16(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
#endif

    _2::test(argc, argv);
}
