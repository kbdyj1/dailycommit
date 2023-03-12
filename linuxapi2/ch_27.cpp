#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
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

void test()
{

}

} //_8 --------------------------------------------------------------

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
#endif

    _7::test(argc, argv);
}
