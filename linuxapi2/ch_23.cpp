//=============================================================================
//  ch.23
//      timer & sleep
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
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

} //namespace =================================================================

void test_ch_23()
{
    _1::test();
}
