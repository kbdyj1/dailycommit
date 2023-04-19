#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "utils.h"

extern char** environ;

namespace { //=================================================================

namespace _1 {

#define CPPPATH "/usr/bin/g++"

void test()
{
    const char* pathname = CPPPATH;
    char* env[] = {
        NULL
    };

    char* args[] = {
        CPPPATH,
        "--version",
        NULL
    };

    execve(pathname, args, env);
    errorExit("execve() error.\n");
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    char** pp;

    for (pp=environ; *pp != NULL; pp++) {
        printf("environment %s\n", *pp);
    }
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    const char* filename = "echo";
    execlp(filename, filename, "Hello, Qt", (char *)NULL);

    errorExit("execlp() error.\n");
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test()
{
    char* env[] = { "GREET=salut", "BYE=adieu", NULL };
    const char* pathname = "/usr/bin/echo";
    char* filename = "echo";

    execle(pathname, filename, "Hello, Qt", (char*)NULL, env);

    errorExit("execlp() error.\n");
}

} //_4 --------------------------------------------------------------

namespace _5 {

void necho(int argc, const char* argv[])
{
    for (int j=0; j<argc; j++) {
        printf("argv[%d] = %s\n", j, argv[j]);
    }
}

void test()
{
    printf("$USER=%s\n", getenv("USER"));
    if (putenv("USER=Haerin")) {
        errorExit("putenv() failed.\n");
    }

    execl("/usr/bin/printenv", "printenv", "USER", "SHELL", (char*)NULL);

    errorExit("execl() errorn.\n");
}

} //_5 --------------------------------------------------------------

namespace _6 {

void test()
{
    execl("longest_line.awk", "longest_line.awk", "input.txt", (char*)NULL);

    errorExit("execl() error.\n");
}

} //_6 --------------------------------------------------------------

namespace _7 {

void test_cloexec_dup()
{
    int fd = open("test", O_RDONLY | O_CREAT);
    if (-1 == fd) {
        errorExit("open(test) error.\n");
    }

    int flags = fcntl(fd, F_GETFD);
    flags |= FD_CLOEXEC;

    printf("fd  FD_CLOEXEC = %d\n", flags & FD_CLOEXEC);

    int fd2 = dup(fd);
    flags = fcntl(fd2, F_GETFD);

    printf("fd2 FD_CLOEXEC = %d\n", flags & FD_CLOEXEC);

    close(fd2);
    close(fd);
}

void test_cloexec_stdout()
{
    int flags;

    flags = fcntl(STDOUT_FILENO, F_GETFD);
    if (-1 == flags)
        errorExit("fcntl(STDOUT_FILENO, F_GETFD) error.\n");

    flags |= FD_CLOEXEC;

    if (-1 == fcntl(STDOUT_FILENO, F_SETFD, flags)) {
        errorExit("fcntl(STDOUT_FILENO, F_SETFD) error.\n");
    }
}

void test(int argc, const char* argv[])
{
    //test_cloexec_stdout();

    execlp("ls", "ls", "-l", argv[0], (char*)NULL);

    errorExit("execlp() error.\n");
}

} //_7 --------------------------------------------------------------

namespace _8 {

void check_shell_enable()
{
    int ret = system(NULL);

    printf("shell enable? %s.\n", (ret == 0 ? "false" : "true"));
}

void check_shell_enable2()
{
    int ret = system(":");

    printf(": -> %d.\n", ret);
}

void check_gxx_version()
{
    int ret = system("g++ --version");

    printf("g++ ? %d.\n", ret);
}

void call_invalid_command()
{
    int ret = system("invalid");

    printf("invalid ? 0x%04x (%d, %d).\n", (unsigned int)ret, ret >> 8, ret & 0xff);

    if (-1 == ret)
        errorExit("system(invalid) error.\n");

    printWaitStatus("system(invalid): ", ret);  //child exited, status=127
}

void test()
{
    //check_shell_enable();
    check_shell_enable2();
    //check_gxx_version();

    //call_invalid_command();
}

} //_8 --------------------------------------------------------------

namespace _9 {

void simple(const char* command)
{
    char path[PATH_MAX];

    if (0 == confstr(_CS_PATH, path, PATH_MAX)) {
        _exit(127);
    }

    printf("PATH=%s\n", path);

    if (-1 == setenv("PATH", path, 1)) {
        _exit(127);
    }

    execlp("sh", "sh", "-c", command, (char*)NULL);

    errorExit("execl() error.\n");
}

int simulate_system(const char* command)
{
    pid_t child;
    int status;

    switch (child = fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        execl("/bin/bash", "bash", "-c", command, (char*)NULL);
        _exit(127);
        break;

    default:
        if (-1 == waitpid(child, &status, 0))
            return -1;
        else
            return status;
    }
}

int simulate_system2(const char* command)
{
    sigset_t blockMask, origMask;
    struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
    pid_t child;
    int status, saveErrno;

    if (NULL == command)
        return system(":") == 0;

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &blockMask, &origMask);

    saIgnore.sa_handler = SIG_IGN;
    saIgnore.sa_flags = 0;
    sigemptyset(&saIgnore.sa_mask);
    sigaction(SIGINT, &saIgnore, &saOrigInt);
    sigaction(SIGQUIT, &saIgnore, &saOrigQuit);

    switch (child = fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        saDefault.sa_handler = SIG_DFL;
        saDefault.sa_flags = 0;
        sigemptyset(&saDefault.sa_mask);

        if (saOrigInt.sa_handler != SIG_IGN)
            sigaction(SIGINT, &saDefault, NULL);
        if (saOrigQuit.sa_handler != SIG_IGN)
            sigaction(SIGQUIT, &saDefault, NULL);

        sigprocmask(SIG_SETMASK, &origMask, NULL);

        execl("/bin/bash", "bash", "-c", command, (char*)NULL);
        _exit(127);

    default:
        while (-1 == waitpid(child, &status, 0)) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
        break;
    }

    saveErrno = errno;

    sigprocmask(SIG_SETMASK, &origMask, NULL);

    sigaction(SIGINT, &saOrigInt, NULL);
    sigaction(SIGQUIT, &saOrigQuit, NULL);

    errno = saveErrno;

    return status;
}

void test()
{
    const char* command = "ls | wc";

#if (1)
    simple(command);
#else
    int status =  simulate_system2(command);

    exit(status);
#endif
}

} //_9 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_27(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
    //_5::necho(argc, argv);
    _5::test();
    _6::test();
    _7::test(argc, argv);
    _8::test();
#endif

    _9::test();
}
