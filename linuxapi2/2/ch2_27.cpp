#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/termios.h>
#include <sys/ioctl.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

const int MAX_BUF = 64;

int ptyMasterOpen(char* slaveName, size_t len)
{
    int fd = posix_openpt(O_RDWR | O_NOCTTY);
    if (-1 == fd)
        return -1;

    int save;
    if (-1 == grantpt(fd)) {
        save = errno;
        close(fd);
        errno = save;
        return -1;
    }

    if (-1 == unlockpt(fd)) {
        save = errno;
        close(fd);
        errno = save;
        return -1;
    }

    char* p = ptsname(fd);
    if (NULL == p) {
        save = errno;
        close(fd);
        errno = save;
        return -1;
    }

    if (strlen(p) < len) {
        strncpy(slaveName, p, len);
    } else {
        close(fd);
        errno = EOVERFLOW;
        return -1;
    }

    return fd;
}

pid_t ptyFork(int* master, char* slaveName, size_t len, const termios* slaveTermios, const winsize* slaveWinSize)
{
    char slname[MAX_BUF];

    int fd = ptyMasterOpen(slname, len);
    if (-1 == fd)
        return -1;

    if (NULL != slaveName) {
        if (strlen(slname) < len) {
            strncpy(slaveName, slname, len);
        } else {
            close(fd);
            errno = EOVERFLOW;
            return -1;
        }
    }

    int save;
    int child = fork();
    switch (child) {
    case -1:
        save = errno;
        close(fd);
        errno = save;
        return -1;

    case 0:
        if (-1 == setsid()) {
            errnoExit("setsid", errno);
        }
        close(fd);
        fd = open(slname, O_RDWR);
        if (-1 == fd)
            errnoExit("open", errno);

#ifdef TIOCSTTY
        if (-1 == ioctl(fd, TIOCSTTY, 0))
            errnoExit("ioctl", errno);
#endif

        if (NULL != slaveTermios) {
            if (-1 == tcsetattr(fd, TCSANOW, slaveTermios)) {
                errnoExit("tcsetattr", errno);
            }
        }

        if (NULL != slaveWinSize) {
            if (-1 == ioctl(fd, TIOCSWINSZ, slaveWinSize)) {
                errnoExit("ioctl", errno);
            }
        }

        if (STDIN_FILENO != dup2(fd, STDIN_FILENO)) {
            errorExit("dup2(STDIN_FILENO)");
        }
        if (STDOUT_FILENO != dup2(fd, STDOUT_FILENO)) {
            errorExit("dup2(STDOUT_FILENO");
        }
        if (STDERR_FILENO != dup2(fd, STDERR_FILENO)) {
            errorExit("dup2(STDERR_FILENO");
        }

        if (fd > STDERR_FILENO)
            close(fd);
        return 0;

    default:
        *master = fd;
        return child;
    }
}

void test()
{
    char buffer[MAX_BUF];
    int fd = ptyMasterOpen(buffer, MAX_BUF);

    if (-1 == fd) {
        errnoExit("ptyMasterOpen", errno);
    }
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_27(int argc, const char* argv[])
{
    _1::test();
}
