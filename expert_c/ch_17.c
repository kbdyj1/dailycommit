#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

//=============================================================================

static void _1_test()
{
    printf("this is the parent process with process id: %d\n", getpid());

    printf("fork() #0\n");
    pid_t ret = fork();
    if (ret) {
        printf("the child process is spawned with pid: %d\n", ret);
    } else {
        printf("this is the child process with pid: %d\n", getpid());
    }

    printf("fork() #1\n");
}

static void _2_test()
{
    pid_t ret = fork();
    if (ret) {
        for (size_t i=0; i<5; i++) {
            printf("parent\n");
            usleep(1);
        }
    } else {
        for (size_t i=0; i<5; i++) {
            printf("child\n");
            usleep(1);
        }
    }
}

static void _3_test()
{
    char* args[] = {"echo", "Hello, ", "Qt6!", 0};
    execvp("echo", args);
    printf("execvp() failed. error: %s\n", strerror(errno));
}

//=============================================================================

void test_ch_17_shm();
void test_ch_17_shm_compete();

void test_ch_17()
{
#if (0)
    _1_test();
    _2_test();
    _3_test();
#endif

    test_ch_17_shm_compete();

    sleep(1);
}
