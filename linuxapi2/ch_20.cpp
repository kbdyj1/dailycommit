#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void sig_int_handler(int sig)
{
    fprintf(stdout, "SIGINT[%d] detected\n", sig);
}

static void sig_tstp_handler(int sig)
{
    fprintf(stdout, "SIGTSTP[%d] detected\n", sig);
}

void test_ch_20()
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
