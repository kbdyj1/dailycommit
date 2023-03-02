//=============================================================================
//  ch.23
//      timer & sleep
//=============================================================================

#undef  _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

namespace { //=================================================================

namespace _1 {

volatile sig_atomic_t gotAlarm = 0;

void displayTimes(const char* msg, bool includeTimer)
{
    struct itimerval val;
    static struct timeval start;
    struct timeval current;
    static int callCnt = 0;

    if (0 == callCnt) {
        if (-1 == gettimeofday(&start, NULL)) {
            fprintf(stderr, "gettimeofday(start) error.\n");
            exit(-1);
        }
    }

    if (0 == callCnt % 20) {
        printf("          Elapsed Value   Interval\n");
    }

    if (-1 == gettimeofday(&current, NULL)) {
        fprintf(stderr, "gettimeofday(current) error.\n");
        exit(-1);
    }

    printf("%-7s %6.2f", msg, current.tv_sec - start.tv_sec + (current.tv_usec - start.tv_usec)/1000000.0);

    if (includeTimer) {
        if (-1 == getitimer(ITIMER_REAL, &val)) {
            fprintf(stderr, "getitimer(ITIMER_REAL) error.\n");
            exit(-1);
        }

        printf("  %6.2f  %6.2f",
               val.it_value.tv_sec + val.it_value.tv_usec/1000000.0,
               val.it_interval.tv_sec + val.it_interval.tv_usec/1000000.0);
    }

    printf("\n");
    callCnt++;
}

void sig_alarm_handler(int /*sig*/)
{
    gotAlarm = 1;
}

#define ONESHOT

void test()
{
    struct itimerval val;
    clock_t prevClock;
    int maxSigs;
    int sigCnt = 0;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_alarm_handler;

    if (-1 == sigaction(SIGALRM, &sa, NULL)) {
        fprintf(stderr, "sigaction(SIGALRM) error.\n");
        exit(-1);
    }


    displayTimes("START:", false);

#ifdef ONESHOT
    alarm(2);

    maxSigs = 1;
#else
    val.it_value.tv_sec = 1;
    val.it_value.tv_usec = 800000;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;

    maxSigs = (val.it_interval.tv_sec == 0 && val.it_interval.tv_usec == 0) ? 1 : 3;

    if (-1 == setitimer(ITIMER_REAL, &val, 0)) {
        fprintf(stderr, "setitimer(ITIMER_REAL) error.\n");
        exit(-1);
    }
#endif

    prevClock = clock();
    sigCnt = 0;

    for ( ;; ) {
        while ( ((clock() - prevClock) * 10 / CLOCKS_PER_SEC) < 5 ) {
            if (gotAlarm) {
                gotAlarm = 0;
                displayTimes("ALARM:", true);

                sigCnt++;
                if (sigCnt >= maxSigs) {
                    printf("That's all forks\n");
                    exit(-1);
                }
            }
        }

        prevClock = clock();
        displayTimes("Main:", true);
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

#define BUF_SIZE    200

void handler(int sig)
{
    printf("Caught signal(%d)\n", sig);
}

void test()
{
    struct sigaction sa;
    char buf[BUF_SIZE];
    ssize_t numRead;
    int savedErrno;

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (-1 == sigaction(SIGALRM, &sa, NULL)) {
        fprintf(stderr, "sigaction() error.\n");
        exit(-1);
    }

    alarm(5);

    numRead = read(STDIN_FILENO, buf, BUF_SIZE-1);

    savedErrno = errno;
    alarm(0);
    errno = savedErrno;

    if (-1 == numRead) {
        if (EINTR == errno) {
            fprintf(stderr, "read timeout.\n");
        } else {
            fprintf(stderr, "read error.\n");
        }
    } else {
        printf("successfuly read (%ld bytes): %.*s", (long)numRead, (int)numRead, buf);
    }
}

#undef  BUF_SIZE

} //_2 --------------------------------------------------------------

namespace _3 {

void handler(int sig)
{
    printf("Caught signal(%d)\n", sig);
}

void test()
{
    struct sigaction sa;

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (-1 == sigaction(SIGINT, &sa, NULL)) {
        fprintf(stderr, "sigaction() error.\n");
        exit(-1);
    }

    unsigned int left = sleep(10);
    if (left != 0) {
        printf("sleep() interruped. %ds left.\n", left);
    } else {
        printf("sleep() end.\n");
    }
}

} //_3 --------------------------------------------------------------

namespace _4 {

void handler(int sig)
{
    return;
}

void test()
{
    struct timeval start, finish;
    struct timespec request, remain;
    struct sigaction sa;
    int s;

    request.tv_sec = 10;
    request.tv_nsec = 500000;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (-1 == sigaction(SIGINT, &sa, NULL)) {
        fprintf(stderr, "sigaction() error.\n");
        exit(-1);
    }
    if (-1 == gettimeofday(&start, NULL)) {
        fprintf(stderr, "gettimeofday(start) error.\n");
        exit(-1);
    }

    for ( ;; ) {
        s = nanosleep(&request, &remain);
        if (-1 == s && EINTR != errno) {
            fprintf(stderr, "nanosleep() error.\n");
            exit(-1);
        }
        if (-1 == gettimeofday(&finish, NULL)) {
            fprintf(stderr, "gettimeofday(finish) error.\n");
            exit(-1);
        }

        printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec + (finish.tv_usec - start.tv_usec)/1000000.0);

        if (0 == s)
            break;

        printf("Remaining: %2ld.%09ld\n", (int)remain.tv_sec, remain.tv_nsec);

        request = remain;
    }

    printf("Sleep complete.\n");
}

} //_4 --------------------------------------------------------------

namespace _5 {

void handler(int sig)
{
    return;
}

void test_getcpuclockid(pid_t pid)
{
    //pid : 2858 (QtCreator)

    clockid_t cid;
    int ret = clock_getcpuclockid(pid, &cid);
    if (0 == ret) {
        printf("pid: %d, clock id: %d\n", pid, cid);
    } else {
        fprintf(stderr, "clock_getcpuclockid(): %d\n", ret);
    }
}

void test_clock_nanosleep()
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (-1 == sigaction(SIGINT, &sa, NULL)) {
        fprintf(stderr, "sigaction() error.\n");
        exit(-1);
    }

    struct timespec request;

    if (-1 == clock_gettime(CLOCK_REALTIME, &request)) {
        fprintf(stderr, "clock_gettime() error.\n");
        exit(-1);
    }

    request.tv_sec += 20;

    int s = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, NULL);
    if (0 != s) {
        if (EINTR == s) {
            printf("interrupted by signal.\n");
        } else {
            fprintf(stderr, "clock_nanosleep() error.\n");
            exit(-1);
        }
    }
    printf("clock_nanosleep() timeout.\n");
}

void test(int argc, const char* argv[])
{
#if (0) //done
    if (1 < argc) {
        pid_t pid = atoi(argv[1]);
        test_getcpuclockid(pid);
    } else {
        fprintf(stderr, "usage: linuxapi2 pid\n");
    }
#endif

    test_clock_nanosleep();
}

} //_5 --------------------------------------------------------------

namespace _6 {

#define TIMER_SIG   SIGRTMAX

void handler(int sig, siginfo_t* si, void* p)
{
    timer_t* tid;

    tid = (timer_t*)si->si_value.sival_ptr;

    printf("Got signal %d\n", sig);
    printf("\t*sival_ptr: %ld\n", (long)*tid);
    printf("\ttimer_getoverrun(): %ld\n", (long)timer_getoverrun(*tid));
}

void test()
{
    struct itimerspec ts;
    struct sigaction sa;
    struct sigevent se;
    timer_t* tid = (timer_t*) malloc(sizeof(timer_t));

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (-1 == sigaction(TIMER_SIG, &sa, NULL)) {
        fprintf(stderr, "sigaction() error.\n");
        exit(-1);
    }

    se.sigev_notify = SIGEV_SIGNAL;
    se.sigev_signo = TIMER_SIG;
    se.sigev_value.sival_ptr = &tid;

    if (-1 == timer_create(CLOCK_REALTIME, &se, tid)) {
        fprintf(stderr, "timer_create() error.\n");
        exit(-1);
    }

    ts.it_interval.tv_sec = 5;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 10;
    ts.it_value.tv_nsec = 0;

    if (-1 == timer_settime(*tid, 0, &ts, NULL)) {
        fprintf(stderr, "timer_settime() error.\n");
        exit(-1);
    }

    for ( ;; ) {
        pause();
    }

    timer_delete(*tid);
    printf("timer_delete().\n");
}

} //_6 --------------------------------------------------------------

namespace _7 {

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int expireCnt = 0;

void threadFunc(union sigval sv)
{
    timer_t* tid;
    int s;

    tid = (timer_t*)sv.sival_ptr;

    printf("threadFunc()\n");
    printf("\t*sival_ptr: %ld\n", (long)*tid);
    printf("\ttimer_getoverrun(): %ld\n", (long)timer_getoverrun(*tid));

    s = pthread_mutex_lock(&mtx);
    if (0 != s) {
        fprintf(stderr, "pthread_mutex_lock() error.\n");
        exit(-1);
    }

    expireCnt += 1 + timer_getoverrun(*tid);

    s = pthread_mutex_unlock(&mtx);
    if (0 != s) {
        fprintf(stderr, "pthread_mutex_unlock() error.\n");
        exit(-1);
    }

    s = pthread_cond_signal(&cond);
    if (0 != s) {
        fprintf(stderr, "pthread_cond_signal() error.\n");
        exit(-1);
    }
}

void test()
{
    struct sigevent se;
    struct itimerspec ts;
    timer_t* tid;
    int s, j;

    tid = (timer_t*)malloc(sizeof(timer_t));

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_notify_function = threadFunc;
    se.sigev_notify_attributes = NULL;
    se.sigev_value.sival_ptr = tid;

    if (-1 == timer_create(CLOCK_REALTIME, &se, tid)) {
        fprintf(stderr, "timer_create() error.\n");
        exit(-1);
    }

    printf("timerid: %ld\n", (long)*tid);

    ts.it_interval.tv_sec = 2;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 5;
    ts.it_value.tv_nsec = 0;

    if (-1 == timer_settime(tid, 0, &ts, NULL)) {
        fprintf(stderr, "timer_settime() error.\n");
        exit(-1);
    }

    for ( ;; ) {
        s = pthread_cond_wait(&cond, &mtx);
        if (0 != s) {
            fprintf(stderr, "pthread_cond_wait() error.\n");
            exit(-1);
        }

        printf("main(): expireCnt: %d\n", expireCnt);
    }
}


} //_7 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_23(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
    _5::test(argc, argv);
    _6::test();
#endif

    _7::test();
}
