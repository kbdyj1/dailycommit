#include <iostream>

#define _XOPEN_SOURCE   600
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <setjmp.h>
#include "utils.h"

namespace { //=================================================================

char* str2 = nullptr;
int handled = 0;

void sig_handler(int sig)
{
    crypt(str2, "xx");
    ++handled;
}

namespace _1 {

void test(int argc, const char** argv)
{
    char* cr1;

    struct sigaction sa;

    if (argc != 3) {
        fprintf(stderr, "usage: %s str1 str2\n", argv[0]);
        exit(-1);
    }

    str2 = (char*)argv[2];
    cr1 = strdup(crypt(argv[1], "xx"));

    if (NULL == cr1) {
        fprintf(stderr, "strdup() error.\n");
        exit(-1);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sig_handler;

    if (-1 == sigaction(SIGINT, &sa, NULL)) {
        fprintf(stderr, "sigaction(SIGINT, ...) error.\n");
        exit(-1);
    }

    int callCnt;
    int mismatch = 0;
    for (callCnt = 1; ; callCnt++) {
        if (0 != strcmp(crypt(argv[1], "xx"), cr1)) {
            ++mismatch;
            fprintf(stdout, "mismatch on call %d (mismatch=%d, handled=%d)\n", callCnt, mismatch, handled);
        }
        if (10 <= mismatch) {
            break;
        }
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

#define USE_SIGSETJMP

#ifdef USE_SIGSETJMP
static sigjmp_buf env;
#else
static jmp_buf env;
#endif

static volatile sig_atomic_t canJump = 0;

void sig_handler(int sig)
{
    printf("Received signal %d (%s), signalmakst is\n", sig, strsignal(sig));
    printSigMask(stdout, NULL);

    if (!canJump) {
        printf("'env' buffer not yet set, doing a simple return.\n");
        return;
    }

#ifdef USE_SIGSETJMP
    siglongjmp(env, 1);
#else
    longjmp(env, 1);
#endif
}

void test()
{
    struct sigaction sa;

    printSigMask(stdout, "Signal mask at startup:\n");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_handler;

    if (-1 == sigaction(SIGINT, &sa, NULL)) {
        fprintf(stderr, "sigaction(SIGINT, &sa, NULL) error.\n");
        exit(-1);
    }

#ifdef USE_SIGSETJMP
    printf("Calling sigsetjmp()\n");
    if (0 == sigsetjmp(env, 1)) {
#else
    if (0 == setjmp(env)) {
#endif
        canJump = 1;
    } else {
        printSigMask(stdout, "After jump from handler, signal mask is:\n");
    }

    for ( ;; )
        pause();
}

} //_2 --------------------------------------------------------------

namespace _3 {

#define USE_SA_SIGINFO

#ifdef USE_SA_SIGINFO
void sig_segv_handler(int sig, siginfo_t* info, void* context) {
    printSigInfo(info);
#else
void sig_segv_handler(int sig) {
#endif
    int x;

    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
    printf("top of stack near %10p\n", (void*)&x);

    fflush(NULL);

    _exit(EXIT_FAILURE);
}

void overflow_stack(int callNum)
{
    char a[100000];

    printf("Call %4d - top of stack near %10p\n", callNum, &a[0]);

    overflow_stack(callNum + 1);
}

void test()
{
    stack_t stk;
    struct sigaction sa;
    int j;

    printf("top of standard stack is near %10p\n", (void*)&j);

    stk.ss_sp = malloc(SIGSTKSZ);
    if (NULL == stk.ss_sp) {
        fprintf(stderr, "malloc error\n");
        exit(-1);
    }
    stk.ss_size = SIGSTKSZ;
    stk.ss_flags = 0;

    if (-1 == sigaltstack(&stk, NULL)) {
        fprintf(stderr, "sigaltstack error.\n");
        exit(-1);
    }

#ifdef USE_SA_SIGINFO
    sa.sa_sigaction = sig_segv_handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
#else
    sa.sa_handler = sig_segv_handler;
    sa.sa_flags = SA_ONSTACK;
#endif
    sigemptyset(&sa.sa_mask);

    if (-1 == sigaction(SIGSEGV, &sa, NULL)) {
        fprintf(stderr, "sigaction() error.\n");
        exit(-1);
    }

    overflow_stack(1);
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_21(int argc, const char** argv)
{
#if (0)
    _1::test(argc, argv);
    _2::test();
#endif

    _3::test();
}
