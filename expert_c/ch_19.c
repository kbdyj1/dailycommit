#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//=============================================================================

static void handle_user_signals(int signal)
{
    switch (signal) {
    case SIGUSR1:
        printf("SIGUSR1 received.\n");
        break;

    case SIGUSR2:
        printf("SIGUSR2 received.\n");
        break;

    default:
        printf("unsupported signal(%d) is received.\n", signal);
        break;
    }
}

static void handle_sigint(int signal)
{
    printf("interrupt signal(%d) is received.\n", signal);
}

static void handle_sigkill(int signal)
{
    printf("kill signal(%d) is received.\n", signal);

    exit(0);
}

static void test()
{
    signal(SIGUSR1, handle_user_signals);
    signal(SIGUSR2, handle_user_signals);
    signal(SIGINT, handle_sigint);
    signal(SIGKILL, handle_sigkill);

    while(1)
        ;
}

//=============================================================================

void test_ch_19_pipe();
void test_ch_19_msg_q();

void test_ch_19(int argc, char** argv)
{
#if (0) //done
    test();
    test_ch_19_pipe();
#endif

    test_ch_19_msg_q();
}
