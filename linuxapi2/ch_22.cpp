#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_22()
{
    _1::test();

    pause();
}
