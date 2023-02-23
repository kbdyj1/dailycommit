#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/signalfd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

typedef void(*sig_handler_t)(int);

sig_handler_t my_signal(int sig, sig_handler_t handler)
{
    struct sigaction newDisp, prevDisp;

    newDisp.sa_handler = handler;
    sigemptyset(&newDisp.sa_mask);

#ifdef LEGACY_IMPL
    newDisp.sa_flags = SA_RESETHAND | SA_NODEFER;
#else
    newDisp.sa_flags = SA_RESTART;
#endif

    if (-1 == sigaction(sig, &newDisp, &prevDisp)) {
        return SIG_ERR;
    } else {
        return prevDisp.sa_handler;
    }
}

void sig_int_handler(int sig)
{
    fprintf(stdout, "sig_int_handler(%d) %s\n", sig, strsignal(sig));
}

void test()
{
    int sigQueueMax = sysconf(_SC_SIGQUEUE_MAX);

    fprintf(stdout, "SIGQUEUE_MAX: %d\n", sigQueueMax);

    my_signal(SIGINT, sig_int_handler);

    pause();
}

} //_1 --------------------------------------------------------------

namespace _2 {

static volatile int sleepTime = 0;
static volatile int sigCnt = 0;
static volatile int allDone = 0;

void sig_info_handler(int sig, siginfo_t* info, void* context)
{
    if (sig == SIGINT || sig == SIGTERM) {
        allDone = 1;
        return;
    }

    sigCnt++;

    printf("caught signal %d (%s)\n", sig, strsignal(sig));
    printf("\tsi_signo=%d, si_code=%d (%s), si_errno=%d\n",
           info->si_signo,
           info->si_code,
           (info->si_code == SI_USER) ? "SI_USER" : (info->si_code == SI_QUEUE) ? "SI_QUEUE" : "other",
           info->si_errno);
    printf("\tsi_value=%d, si_pid=%ld, si_uid=%ld\n", info->si_value.sival_int, (long)info->si_pid, (long)info->si_uid);

    sleep(sleepTime);
}

void test(int argc, const char* argv[])
{
    struct sigaction sa;
    int sig;
    sigset_t prevMask, blockMask;

    sleepTime = atoi(argv[2]);

    sa.sa_sigaction = sig_info_handler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);

    for (sig=1; sig<NSIG; sig++) {
        if (sig != SIGTSTP && sig != SIGQUIT) {
            sigaction(sig, &sa, NULL);
        }
    }

    sigfillset(&blockMask);
    sigdelset(&blockMask, SIGINT);
    sigdelset(&blockMask, SIGTERM);

    if (-1 == sigprocmask(SIG_SETMASK, &blockMask, &prevMask)) {
        fprintf(stderr, "sigprocmask(blockMask)\n");
        exit(-1);
    }

    int s = atoi(argv[1]);
    sleep(s);

    printf("%s sleep complete\n", argv[0]);

    if (-1 == sigprocmask(SIG_SETMASK, &prevMask, NULL)) {
        fprintf(stderr, "sigprocmask(prevMask)\n");
        exit(-1);
    }

    while (!allDone) {
        pause();
    }

    printf("caught %d signals\n", sigCnt);

    exit(0);
}

} //_2 --------------------------------------------------------------

namespace _3 {

static volatile sig_atomic_t gotSigQuit = 0;

void sig_handler(int sig)
{
    printf("caught signal %d (%s)\n", sig, strsignal(sig));

    if (SIGQUIT == sig) {
        gotSigQuit = 1;
    }
}

void test(int argc, const char* argv[])
{
    int loopNum;
    time_t startTime;
    sigset_t origMask, blockMask;
    struct sigaction sa;

    printSigMask(stdout, "Initial sigmask:\n");

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGINT);
    sigaddset(&blockMask, SIGQUIT);

    if (-1 == sigprocmask(SIG_SETMASK, &blockMask, &origMask)) {
        fprintf(stderr, "sigprocmask(blockMask)\n");
        exit(-1);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_handler;

    if (-1 == sigaction(SIGINT, &sa, NULL)) {
        fprintf(stderr, "sigaction(SIGINT) error\n");
        exit(-1);
    }
    if (-1 == sigaction(SIGQUIT, &sa, NULL)) {
        fprintf(stderr, "sigaction(SIGQUIT) error\n");
        exit(-1);
    }

    for (loopNum = 1; !gotSigQuit; loopNum++) {
        printf("===== LOOP %d\n", loopNum);

        printSigMask(stdout, "Begin Critical Section, sigmask:\n");
        for (startTime = time(NULL); time(NULL) < startTime + 4; )
            continue;

        printPendingSigs(stdout, "before suspend() - pending signals\n");

        if (-1 == sigsuspend(&origMask) && errno != EINTR) {
            printf("sigsuspend() error.\n");
            exit(-1);
        }
    }

    if (-1 == sigprocmask(SIG_SETMASK, &origMask, NULL)) {
        fprintf(stderr, "sigprocmask(origMask) error.\n");
        exit(-1);
    }

    printSigMask(stdout, "===== End LOOP\nRestored signal mask to:\n");

    exit(0);
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test(int argc, const char* argv[])
{
    int sig;
    siginfo_t info;
    sigset_t allSigs;

    printf("%s PID: %ld\n", argv[0], (long)getpid());

    sigfillset(&allSigs);
    if (-1 == sigprocmask(SIG_SETMASK, &allSigs, NULL)) {
        fprintf(stderr, "sigprocmask() error.\n");
        exit(-1);
    }

    printf("%s signals blocked.\n", argv[0]);

    if (1 < argc) {
        int s = atoi(argv[1]);

        printf("%s: about to delay %d seconds.\n", argv[0], s);
        sleep(s);
        printf("%s: finished delay.\n", argv[0]);
    }

    for ( ;; ) {
        sig = sigwaitinfo(&allSigs, &info);
        if (-1 == sig) {
            fprintf(stderr, "sigwaitinfo() error.\n");
            exit(-1);
        }

        printf("got signal: %d (%s)\n", sig, strsignal(sig));

        printSigInfo(&info);
    }
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test(int argc, const char* argv[])
{
    sigset_t mask;
    int fd;
    int j;
    struct signalfd_siginfo info;
    ssize_t s;

    printf("%s PID: %ld\n", argv[0], (long)getpid());

    sigemptyset(&mask);

    for (j=1; j<argc; j++) {
        sigaddset(&mask, atoi(argv[j]));
    }

    if (-1 == sigprocmask(SIG_BLOCK, &mask, NULL)) {
        fprintf(stderr, "sigprocmask(SIG_BLOCK) error.\n");
        exit(-1);
    }

    fd = signalfd(-1, &mask, 0);
    if (-1 == fd) {
        fprintf(stderr, "signalfd(-1) error.\n");
        exit(-1);
    }

    for ( ;; ) {
        s = read(fd, &info, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo)) {
            fprintf(stderr, "read() error.\n");
            exit(-1);
        }

        printf("%s: got signal %d (%s)\n", argv[0], info.ssi_signo, strsignal(info.ssi_signo));
        if (SI_QUEUE == info.ssi_code) {
            printf("ssi_pid: %d, ssi_int: %d", info.ssi_pid, info.ssi_int);
        }
        printf("\n");
    }
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_22(int argc, const char* argv[])
{
#if (0)
    _1::test();
    _2::test(argc, argv);
    _3::test(argc, argv);
    _4::test(argc, argv);
#endif

    _5::test(argc, argv);
}
