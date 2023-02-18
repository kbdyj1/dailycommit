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
