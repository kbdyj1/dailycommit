#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>

#include "utils.h"

namespace { //=================================================================

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

namespace _1 {

void test()
{
    printf("startDaemon...\n");

    startDaemon(0);

    while (1)
        sleep(1);
}

} //_1 --------------------------------------------------------------

namespace _2 {

const char* LOG_FILE = "/tmp/ch_32_2.log";
const char* CONFIG_FILE = "/tmp/ch_32_2.cfg";

volatile sig_atomic_t sig_hup_received = 0;

FILE* logfd = NULL;

void sig_handler(int sig)
{
    sig_hup_received = 1;
}

void logMessage(const char* format, ...)
{
    va_list argList;
    const char* TIMESTAMP_FMT = "%F %X";
#define TS_BUF_SIZE sizeof("YYYY-MM-DD HH:MM:SS")
    char timestamp[TS_BUF_SIZE];
    time_t t;
    struct tm* loc;

    t = time(NULL);
    loc = localtime(&t);
    if (NULL == loc || 0 == strftime(timestamp, TS_BUF_SIZE, TIMESTAMP_FMT, loc))
        fprintf(logfd, "???Unknown time???");
    else
        fprintf(logfd, "%s: ", timestamp);

    va_start(argList, format);
    vfprintf(logfd, format, argList);
    fprintf(logfd, "\n");
    va_end(argList);
}

void logOpen(const char* filename)
{
    mode_t m;

    m = umask(077);
    logfd = fopen(filename, "a");
    umask(m);

    if (NULL == logfd)
        exit(EXIT_FAILURE);

    setbuf(logfd, NULL);

    logMessage("Opened log file");
}

void logClose()
{
    logMessage("Closing log file");
    fclose(logfd);
}

void readConfig(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (NULL != fp) {
        const int BUF_SIZE = 256;
        char buf[BUF_SIZE];

        if (NULL == fgets(buf, BUF_SIZE, fp)) {
            buf[0] = '\0';
        } else {
            buf[strlen(buf) -1] = '\0';
        }
        logMessage("Read config: %s", buf);
        fclose(fp);
    }
}

void test()
{
    const int SLEEP_TIME = 15;
    int count = 0;
    int unslept;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sig_handler;

    if (-1 == sigaction(SIGHUP, &sa, NULL))
        errorExit("sigaction(SIGHUP) error.\n");


    if (-1 == startDaemon(0))
        errorExit("startDaemon(0) error.\n");

    logOpen(LOG_FILE);
    readConfig(CONFIG_FILE);

    unslept = SLEEP_TIME;

    for ( ;; ) {
        unslept = sleep(unslept);

        if (sig_hup_received) {
            logClose();
            logOpen(LOG_FILE);
            readConfig(CONFIG_FILE);
            sig_hup_received = 0;
        }

        if (unslept == 0) {
            count++;

            logMessage("Main: %d\n", count);
            unslept = SLEEP_TIME;
        }
    }
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_32()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
