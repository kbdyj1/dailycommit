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

namespace _5 {

int position;

void handler(int sig)
{
    if (getpid() == getpgrp()) {
        fprintf(stderr, "Terminal FG Process group: %ld\n", (long)tcgetpgrp(STDERR_FILENO));
    }

    fprintf(stderr, "Process %ld (%d) received signal %d (%s)\n", (long)getpid(), position, sig, strsignal(sig));

    if (SIGTSTP == sig)
        raise(SIGSTOP);
}

void test()
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;

    if (-1 == sigaction(SIGINT, &sa, NULL))
        errorExit("sigaction(SIGINT) error.\n");
    if (-1 == sigaction(SIGTSTP, &sa, NULL))
        errorExit("sigaction(SIGTSTP) error.\n");
    if (-1 == sigaction(SIGCONT, &sa, NULL))
        errorExit("sigaction(SIGCONT) error.\n");

    if (isatty(STDIN_FILENO)) {
        position = 0;
        fprintf(stderr, "Terminal FG Process group: %ld\n", (long)tcgetpgrp(STDERR_FILENO));
    } else {
        if (read(STDIN_FILENO, &position, sizeof(int)) <= 0) {
            errorExit("read got EOF or error");
        }
    }

    position++;
    fprintf(stderr, "%4d PID: %5ld, PPID: %5ld, GID: %5ld, SID: %5ld\n",
            position,
            (long)getpid(),
            (long)getppid(),
            (long)getpgrp(),
            (long)getsid(0));


    auto pipeOut = !isatty(STDOUT_FILENO);
    if (pipeOut) {
        if (-1 == write(STDOUT_FILENO, &position, sizeof(int)))
            errorExit("write() error.\n");
    }

    // wait signal
    for ( ;; )
        pause();
}

} //_5 --------------------------------------------------------------

namespace _6 {

void handler(int sig)
{
    sigset_t mask, prevMask;
    int saveErrno;
    struct sigaction sa;

    saveErrno = errno;

    printf("caught SIGTSTP\n");

    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        errorExit("signal(SIGTSTP, SIG_DFL) error.\n");

    raise(SIGTSTP);
    printf("raise(SIGTSTP) done.\n");

    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    if (-1 == sigprocmask(SIG_UNBLOCK, &mask, &prevMask))
        errorExit("sigprocmask(SIG_UNBLOCK, ...) error.\n");
    else
        printf("sigprocmask(SIG_UNBLOCK, &mask, &prevMask) done.\n");

    if (-1 == sigprocmask(SIG_SETMASK, &prevMask, NULL))
        errorExit("sigprocmask(SIG_SETMASK, ...) error.\n");
    else
        printf("sigprocmask(SIG_SETMASK, &prevMask, NULL) done.\n");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (-1 == sigaction(SIGTSTP, &sa, NULL))
        errorExit("sigaction(SIGTSTP) error.\n");

    printf("exiting handler.\n");

    errno = saveErrno;
}

void test()
{
    struct sigaction sa;

    if (-1 == sigaction(SIGTSTP, NULL, &sa))
        errorExit("sigaction(SIGTSTP, NULL, &sa) error.\n");

    if (SIG_IGN != sa.sa_handler) {
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = handler;
        if (-1 == sigaction(SIGTSTP, &sa, NULL))
            errorExit("sigaction(SIGTSTP, &sa, NULL) error.\n");
    }

    for ( ;; ) {
        pause();
        printf("Main\n");
    }
}

} //_6 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_29(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
    _2::test();
    _4::test(argc, argv);
    _5::test();
#endif

    _6::test();
}
