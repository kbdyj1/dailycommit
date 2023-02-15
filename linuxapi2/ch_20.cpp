#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//#define HANDLE_SIGABRT
#define ENABLE_PSIGNAL

static void sig_int_handler(int sig)
{
#if defined(ENABLE_PSIGNAL)
    psignal(sig, "\tSIGINT");
#else
    char* str = strsignal(sig);
    fprintf(stdout, "\t%s[%d] detected\n", str, sig);
#endif
}

static void raise_abort()
{
    //deliver SIGABRT to self
#if (0)
    kill(0, SIGABRT);
#else
    raise(SIGABRT);
#endif
}

static bool check_process_exist(int pid)
{
    int ret = kill(pid, 0);

    switch (ret) {
    case ESRCH:
        return false;

    case EPERM:
    case 0:
        return true;

    default:
        return false;
    }
}

static void sig_tstp_handler(int sig)
{
#if defined(ENABLE_PSIGNAL)
    psignal(sig, "\tSIGTSTP");
#else
    char* str = strsignal(sig);
    fprintf(stdout, "\t%s[%d] detected\n", str, sig);
#endif

#ifdef HANDLE_SIGABRT
    raise_abort();
#endif

    auto exist = check_process_exist(4003);
    fprintf(stdout, "qtcreator exist? %d\n", exist);
}

void test_ch_20(int argc, const char** argv)
{
    if (signal(SIGINT, sig_int_handler) == SIG_ERR) {
        fprintf(stderr, "signal(SIGINT) failed.\n");
        exit(-1);
    }
    if (signal(SIGTSTP, sig_tstp_handler) == SIG_ERR) {
        fprintf(stderr, "signal(SIGTSTP) failed.\n");
        exit(-1);
    }
    for (int i=0; i<20; i++) {
        sleep(3);
    }
    fprintf(stdout, "program end.\n");
}
