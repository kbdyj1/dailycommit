#include "utils.h"
#include <string.h>

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

void printSigInfo(siginfo_t* info)
{
    printf("---------------------------------------------------\n");
    printf("signo: %d\n", info->si_signo);
    printf("code: %d\n", info->si_code);        // siginfo.h
    //printf("trapno: %d\n", info->si_trapno);
    printf("sender pid: %d\n", info->si_pid);
    printf("sender uid: %d\n", info->si_uid);
    //printf("errno: %d\n", info->si_errno);    // not used in linux
    printf("addr: %p\n", info->si_addr);        // instruction or memory address
    printf("timer id: %d\n", info->si_timerid);
    printf("band: %d\n", info->si_band);
    printf("fd: %d\n", info->si_fd);
    printf("status: %d\n", info->si_status);
    printf("utime: %d\n", info->si_utime);
    printf("stime: %d\n", info->si_stime);
    printf("---------------------------------------------------\n");
}
