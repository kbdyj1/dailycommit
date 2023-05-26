#define _GNU_SOURCE

#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

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

void errorExit(const char *message, int exitCode)
{
    fprintf(stderr, "%s", message);
    exit(exitCode);
}

void errnoExit(const char* message, int err, int exitCode)
{
    const char* errstr = strerror(err);
    fprintf(stderr, "%s %s\n", message, errstr);
    exit(exitCode);
}

void printWaitStatus(const char* msg, int status)
{
    if (msg != NULL)
        printf("%s", msg);

    if (WIFEXITED(status)) {
        printf("child exited, status=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        printf("child killed by signal %d (%s)", sig, strsignal(sig));

        if (WCOREDUMP(status)) {
            printf(" (core dumped)");
        }

        printf("\n");
    } else if (WIFSTOPPED(status)) {
        int sig = WSTOPSIG(status);
        printf("child stopped by signal %d, (%s)\n", sig, strsignal(sig));
    } else if (WIFCONTINUED(status)) {
        printf("child continued.\n");
    } else {
        printf("what happend to this child? (status=%x)\n", (unsigned int)status);
    }
}

double timeval2double(struct timeval* time)
{
    double ret = time->tv_sec;
    ret += time->tv_usec / 1000000.0;

    return ret;
}
