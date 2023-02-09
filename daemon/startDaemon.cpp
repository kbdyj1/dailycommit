#include "startDaemon.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#define LOG_FILENAME    "/tmp/sd.log"
#define CONFIG_FILENAME "/tmp/sd.conf"

static volatile sig_atomic_t sig_hup_received = 0;

void sig_hup_handler(int sig)
{
    sig_hup_received = 1;
}

int startDaemon(int flags)
{
    int maxfd;
    int fd;

    switch (fork()) {
    case -1:
        return -1;

    case 0:
        //Child -> continue
        break;

    default:
        //Parent -> exit
        _exit(0);
    }

    if (setsid() == -1)
        return -1;

    switch (fork()) {
    case -1:
        return -1;

    case 0:
        break;

    default:
        _exit(0);
    }

    if (flags & SD_NO_UMASK0)
        umask(0);

    if (!(flags & SD_NO_CHDIR))
        chdir("/");

    if (!(flags & SD_NO_CLOSE_FILES)) {
        maxfd = sysconf(_SC_OPEN_MAX);
        if (-1 == maxfd) {
            maxfd = SD_MAX_CLOSE;
        }

        for (fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if (!(flags & SD_NO_REOPEN_STD_FDS)) {
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);

        if (fd != STDIN_FILENO)
            return -1;

        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;

        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    sleep(10);

    return 0;
}
