#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

namespace { //=================================================================

namespace _1 {

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

void test()
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

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    sigset_t ss;

    int ret = sigfillset(&ss);
    fprintf(stdout, "%d = sigfillset()\n", ret);
    /*
    Locals
        ss	@0x7fffffffe3f0	sigset_t
            __val	@0x7fffffffe3f0	unsigned long[16]
                [0]	(hex) fffffffe7fffffff	unsigned long
                [1]	(hex) ffffffffffffffff	unsigned long
                [2]	(hex) ffffffffffffffff	unsigned long
                [3]	(hex) ffffffffffffffff	unsigned long
                [4]	(hex) ffffffffffffffff	unsigned long
                [5]	(hex) ffffffffffffffff	unsigned long
                [6]	(hex) ffffffffffffffff	unsigned long
                [7]	(hex) ffffffffffffffff	unsigned long
                [8]	(hex) ffffffffffffffff	unsigned long
                [9]	(hex) ffffffffffffffff	unsigned long
                [10](hex) ffffffffffffffff	unsigned long
                [11](hex) ffffffffffffffff	unsigned long
                [12](hex) ffffffffffffffff	unsigned long
                [13](hex) ffffffffffffffff	unsigned long
                [14](hex) ffffffffffffffff	unsigned long
                [15](hex) ffffffffffffffff	unsigned long
    */

    ret = sigemptyset(&ss);
    fprintf(stdout, "%d = sigemptyset()\n", ret);

    ret = sigaddset(&ss, sigmask(SIGINT));
    ret = sigaddset(&ss, sigmask(SIGABRT));

    fprintf(stdout, "%d = sigaddset()\n", ret);

    ret = sigdelset(&ss, sigmask(SIGINT));
    fprintf(stdout, "%d = sigdelset()\n", ret);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void printSigset(FILE* of, const char* prefix, const sigset_t* ss)
{
    int cnt = 0;
    for (int sig=1; sig<NSIG; sig++) {
        if (sigismember(ss, sig)) {
            ++cnt;
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }
    if (cnt == 0) {
        fprintf(of, "%s<empty signal set>\n", prefix);
    }
}

int printSigMask(FILE* of, const char* msg)
{
    sigset_t cur;

    if (msg != NULL) {
        fprintf(of, "%s", msg);
    }

    if (-1 == sigprocmask(SIG_BLOCK, NULL, &cur))
        return -1;

    printSigset(of, "\t\t", &cur);

    return 0;
}

int printPendingSigs(FILE* of, const char* msg)
{
    sigset_t ss;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (-1 == sigpending(&ss))
        return -1;

    printSigset(of, "\t\t", &ss);

    return 0;
}

void test()
{
    sigset_t ss;

#if (0)
    sigfillset(&ss);
    printSigset(stdout, "\t", &ss);
#endif

#if (0)
    sigsetmask(sigmask(SIGINT) | sigmask(SIGABRT)); //deprecated
#else
    sigemptyset(&ss);
    sigaddset(&ss, SIGINT);
    sigaddset(&ss, SIGABRT);

    sigset_t old;
    sigprocmask(SIG_BLOCK, &ss, &old);

    printSigset(stdout, "new\t", &ss);
    printSigset(stdout, "old\t", &old);
#endif

//    printSigMask(stdout, "sigmask");
}

} //_3 --------------------------------------------------------------

namespace _4 {

static void sig_int_handler2(int sig)
{
    fprintf(stdout, "\t[%d] %s\n", sig, strsignal(sig));
}

static void sig_tstp_handler2(int sig)
{
    fprintf(stdout, "\t[%d] %s\n", sig, strsignal(sig));

    sigset_t ss;
#if (0)
    sigprocmask(SIG_BLOCK, NULL, &ss);
    sigprocmask(SIG_UNBLOCK, &ss, NULL);
#else
    int ret = sigpending(&ss);
    if (sigismember(&ss, SIGINT)) {
        fprintf(stdout, "\tpending [%d]%s\n", SIGINT, strsignal(SIGINT));
    }
#endif
}

void test()
{
    signal(SIGINT, sig_int_handler2);
    signal(SIGTSTP, sig_tstp_handler2);

    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGINT);
    sigprocmask(SIG_BLOCK, &ss, NULL);

    for (;;) {
        sleep(1);
    }
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_20(int argc, const char** argv)
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
