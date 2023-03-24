#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

namespace { //=================================================================

namespace _1 {

enum Flag {
    NoChDir = 0x1,
    NoCloseFiles = 0x2,
    NoReopenStdFds = 0x4,
    NoUmask = 0x10,

    MaxClose = 8192
};

int startDaemon(int flags)
{
    switch (fork()) {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    if (-1 == setsid())
        return -1;

    switch (fork()) {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    if (flags & NoUmask)
        umask(0);

    if (!(flags & NoChDir))
        chdir("/");

    int fd;
    if (!(flags & NoCloseFiles)) {
        long maxfd = sysconf(_SC_OPEN_MAX);
        if (-1 == maxfd)
            maxfd = MaxClose;

        for (fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if (!(flags & NoReopenStdFds)) {
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);

        if (fd != STDIN_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    return 0;
}

void test()
{
    printf("startDaemon...\n");

    startDaemon(0);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_32()
{
    _1::test();
}
