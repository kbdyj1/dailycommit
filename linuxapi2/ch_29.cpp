#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"

namespace { //=================================================================

void print_pid(pid_t pid, char* msg = "")
{
    auto gid = getpgid(pid);
    auto sid = getsid(pid);
    auto leader = pid == gid;
    auto c = leader ? 'v' : ' ';

    printf("[%c] %spid: %ld, group: %ld, session: %ld.\n", c, msg, (long)pid, (long)gid, (long)sid);
}

namespace _1 {

void test()
{
    auto pid = getpid();
    auto gid = getpgrp();

    print_pid(pid);
    printf("getpgrp(): %ld.\n", (long)gid);

    if (-1 == setpgid(0, 0))
        errorExit("setpgid(0,0) error.\n");

    printf("setpgid(0, 0). ---------------------------------\n");

    print_pid(getpid());
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test_psudo()
{
    pid_t child;
    pid_t pipeGid;

    child = fork();

    switch (child) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        if (-1 == setpgid(0, pipeGid)) {
            // error
        }
        break;

    default:
        if (-1 == setpgid(child, pipeGid) && errno == EACCES) {
            // error
        }
    }
}

void sig_handler(int sig)
{
    fprintf(stderr, "signal %d (%s) occured!!!\n", sig, strsignal(sig));
}

void test_session()
{
    signal(SIGHUP, sig_handler);

    pid_t pid = getpid();
    pid_t child;

    switch (child = fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        pid = getpid();
        print_pid(pid, "child0 ");

        pid = setsid();
        print_pid(pid, "child1 ");

        if (-1 == open("/dev/tty", O_RDWR))
            errorExit("open(/dev/tty) error.\n");

        exit(EXIT_SUCCESS);

    default:
        print_pid(pid, "parent ");
        _exit(EXIT_SUCCESS);
    }
}

void test_ctermid()
{
    char* path = ctermid(NULL);
    printf("ctermid(): %s\n", path);
}

void test()
{
    //test_session();
    test_ctermid();
}

} //_2 --------------------------------------------------------------

namespace _3 {

void handler(int sig)
{}

void test()
{
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (-1 == sigaction(SIGHUP, &sa, NULL)) {
        errorExit("sigaction(SIGHUP, ...) error.\n");
    }

    pid_t child = fork();

    if (0 == child) {
        if (-1 == setpgid(0, 0))
           errorExit("setpgid(0, 0) error.\n");
    }

    print_pid(getpid());

    alarm(60);

    for ( ;; ) {
        pause();

        printf("%ld: caught SIGHUP\n", (long)getpid());
    }
}

} //_3 --------------------------------------------------------------

namespace _4 {

void handler(int sig)
{
    printf("PID: %ld caught signal %2d (%s)\n", (long)getpid(), sig, strsignal(sig));
}

void test(int argc, const char* argv[])
{
    pid_t parent;
    pid_t child;
    struct sigaction sa;

    if (argc < 2) {
        printf("usage: %s {d|s}...");
        exit(-1);
    }

    setbuf(stdout, NULL);

    parent = getpid();
    print_pid(parent, "parent: ");

    printf("foreground process group is: %ld\n", (long)tcgetpgrp(STDIN_FILENO));

    for (int j=1; j<argc; j++) {
        child = fork();
        if (-1 == child) {
            errorExit("fork() error.\n");
        }

        if (0 == child) {
            if (argv[j][0] == 'd') {
                if (-1 == setpgid(0, 0))
                    errorExit("setpgid() error.\n");
            }
        }

        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sa.sa_handler = handler;

        if (-1 == sigaction(SIGHUP, &sa, NULL)) {
            errorExit("sigaction");
        }

        break;
    }

    alarm(30);

    print_pid(getpid());

    for ( ;; )
        pause();
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_29(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _4::test(argc, argv);
}
