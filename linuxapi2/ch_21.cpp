#include <iostream>

#define _XOPEN_SOURCE   600
#include <signal.h>
#include <unistd.h>
#include <string.h>

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

} //namespace =================================================================

void test_ch_21(int argc, const char** argv)
{
    _1::test(argc, argv);
}
