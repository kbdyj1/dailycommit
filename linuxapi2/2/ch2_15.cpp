#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test(int argc, const char* argv[])
{
    mq_attr*pattr = NULL;
    mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 50;
    attr.mq_msgsize = 2048;
    attr.mq_curmsgs = 0;
    int flags = O_RDWR;

    if (2 <= argc) {
        attr.mq_maxmsg = atoi(argv[1]);
        pattr = &attr;
    }
    if (3 <= argc) {
        attr.mq_msgsize = atoi(argv[2]);
    }
    if (4 <= argc) {
        if (argv[3][0] == 'c') {
            flags |= O_CREAT;
        } else if (argv[3][0] == 'x') {
            flags |= O_CREAT;
            flags |= O_EXCL;
        }
    }
    mode_t perms = S_IRUSR | S_IWUSR;

    mqd_t id = mq_open("/test", flags, perms, pattr);
    if (-1 == id) {
        fprintf(stderr, "errno: %s\n", strerror(errno));
        errorExit("mq_open() failed.\n");
    } else {
        printf("mq_open(): %d\n", id);
    }

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test(int argc, const char* argv[])
{
    mqd_t id = mq_open("/test", O_RDONLY);
    if (-1 == id) {
        errorExit("mq_open() failed.\n");
    }

    mq_attr attr;
    if (-1 == mq_getattr(id, &attr)) {
        errorExit("mq_getattr() failed.\n");
    }

    printf("mq_flags: %ld\n", attr.mq_flags);
    printf("mq_maxmsg: %ld\n", attr.mq_maxmsg);
    printf("mq_msgsize: %ld\n", attr.mq_msgsize);
    printf("mq_curmsgs: %ld\n", attr.mq_curmsgs);

    printf("MQ_PRIO_MAX: %ld\n", sysconf(_SC_MQ_PRIO_MAX));  //32768

    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test(int argc, const char* argv[])
{
    if (argc < 4) {
        fprintf(stderr, "usage: %s mqid priority text\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    mqd_t id = mq_open(argv[1], O_WRONLY);
    if (-1 == id) {
        errorExit("mq_open() error.\n");
    }

    unsigned int prio = atoi(argv[2]);
    if (-1 == mq_send(id, argv[3], strlen(argv[3]), prio)) {
        errorExit("mq_send() error.\n");
    }

    exit(EXIT_SUCCESS);
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s mqid\n", argv[0]);
    }

    mqd_t id = mq_open(argv[1], O_RDONLY);
    if (-1 == id) {
        errorExit("mq_open() failed.\n");
    }

    mq_attr attr;
    if (-1 == mq_getattr(id, &attr)) {
        errorExit("mq_getattr() failed.\n");
    }

    char* buffer = (char*)malloc(attr.mq_msgsize);
    if (NULL == buffer) {
        errorExit("malloc() failed.\n");
    }

    unsigned int prio;
    ssize_t reads = mq_receive(id, buffer, attr.mq_msgsize, &prio);
    if (-1 == reads) {
        errorExit("mq_receive() error.\n");
    }

    printf("read: %ld bytes. priority: %u\n", reads, prio);
    write(STDOUT_FILENO, buffer, reads);
    write(STDOUT_FILENO, "\n", 1);

    exit(EXIT_SUCCESS);
}

} //_4 --------------------------------------------------------------

namespace _5 {

#define NOTIFY_SIG  SIGUSR1

void sigHandler(int sig)
{
    fprintf(stdout, "sigHandler(%d)\n", sig);
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s mq-name.\n", argv[0]);
    }

    mqd_t id = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
    if (-1 == id) {
        errorExit("mq_open() failed.\n");
    }

    mq_attr attr;
    if (-1 == mq_getattr(id, &attr)) {
        errorExit("mq_getattr() failed.\n");
    }

    char* buffer = (char*)malloc(attr.mq_msgsize);
    if (NULL == buffer) {
        errorExit("malloc() failed.\n");
    }

    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, NOTIFY_SIG);
    if (-1 == sigprocmask(SIG_BLOCK, &blockMask, NULL)) {
        errorExit("sigprocaction() failed.\n");
    }

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigHandler;
    if (-1 == sigaction(NOTIFY_SIG, &sa, NULL)) {
        errorExit("sigaction() failed.\n");
    }

    sigevent ev;
    ev.sigev_notify = SIGEV_SIGNAL;
    ev.sigev_signo = NOTIFY_SIG;
    if (-1 == mq_notify(id, &ev)) {
        errorExit("mq_notify() error.\n");
    }

    sigset_t emptyMask;
    sigemptyset(&emptyMask);

    ssize_t reads;
    for ( ;; ) {
        sigsuspend(&emptyMask);

        if (-1 == mq_notify(id, &ev)) {
            errorExit("mq_notify() error.\n");
        }

        while ((reads = mq_receive(id, buffer, attr.mq_msgsize, NULL)) >= 0) {
            printf("Read %ld bytes.\n", reads);
        }

        if (errno != EAGAIN) {
            errorExit("mq_receive() failed.\n");
        }
    }
}

} //_5 --------------------------------------------------------------

namespace _6 {

void threadFunc(sigval v);

void notifySetup(mqd_t* pid)
{
    sigevent ev;
    ev.sigev_notify = SIGEV_THREAD;
    ev.sigev_notify_function = threadFunc;
    ev.sigev_notify_attributes = NULL;
    ev.sigev_value.sival_ptr = pid;

    if (-1 == mq_notify(*pid, &ev)) {
        errorExit("notifySetup() failed.\n");
    }
}

void threadFunc(sigval v)
{
    mqd_t* pid = (mqd_t*)v.sival_ptr;
    mq_attr attr;

    if (-1 == mq_getattr(*pid, &attr)) {
        errorExit("mq_getattr() failed.\n");
    }

    char* buffer = (char*)malloc(attr.mq_msgsize);
    if (NULL == buffer) {
        errorExit("malloc() failed.");
    }

    notifySetup(pid);

    ssize_t reads;

    while (0 <= (reads = mq_receive(*pid, buffer, attr.mq_msgsize, NULL))) {
        printf("Reads %ld bytes.\n", reads);
    }

    if (EAGAIN != errno) {
        errorExit("mq_receive() failed.\n");
    }

    free(buffer);

    pthread_exit(NULL);
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s mq-name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mqd_t id = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
    if (-1 == id) {
        errorExit("mq_open() failed.\n");
    }

    notifySetup(&id);
    pause();
}

} //_6 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_15(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
    _2::test(argc, argv);
    _3::test(argc, argv);
    _4::test(argc, argv);
    _5::test(argc, argv);
#endif

    _6::test(argc, argv);
}
