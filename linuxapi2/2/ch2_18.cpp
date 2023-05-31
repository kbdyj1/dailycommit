#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test(int argc, const char* argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if (-1 == fd)
        errorExit("open() failed.\n");

    int lock = 0;
    if (2 < argc) {
        char opt = argv[2][0];

        switch (opt) {
        case 's':
            lock = LOCK_SH;
            break;
        case 'x':
            lock = LOCK_EX;
            break;
        }
    }

    const char* lockName = lock == LOCK_SH ? "LOCK_SH" : "LOCK_EX";
    printf("PID %ld: request %s\n", (long)getpid(), lockName);

    if (-1 == flock(fd, lock)) {
        if (EWOULDBLOCK == errno) {
            errorExit("flock() already locked.\n");
        } else {
            errorExit("flock() failed.\n");
        }
    }

    printf("PID %ld: granted %s\n", (long)getpid(), lockName);

    if (3 < argc) {
        int sleepTime = atoi(argv[3]);

        sleep(sleepTime);
    }

    printf("PID %ld: releasing %s\n", (long)getpid(), lockName);

    if (-1 == flock(fd, LOCK_UN))
        errorExit("flock() unlock failed.\n");

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

namespace _2 {

const int MAX_LINE = 100;

void printUsage(const char* argv[])
{
    fprintf(stderr, "Usage: %s cmd [r|w|u] start length\n", argv[0]);
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        printUsage(argv);
    }

    int fd = open(argv[0], O_RDWR);
    if (-1 == fd) {
        errorExit("open() failed.\n");
    }

    char line[MAX_LINE];
    char c;
    char l;
    long start, len;
    struct flock fl;
    int cmd;
    int lock;
    int status;

    for ( ;; ) {
        printf("PID: %ld\n", (long)getpid());
        fflush(stdout);

        if (NULL == fgets(line, MAX_LINE, stdin)) {
            exit(EXIT_SUCCESS);
        }
        line[strlen(line) -1] = '\0';

        if (*line == '\0')
            continue;

        if (line[0] == '?') {
            printUsage(argv);
            continue;
        }

        int numRead = sscanf(line, "%c %c %ld %ld", &c, &l, &start, &len);
        fl.l_start = start;
        fl.l_len = len;
        cmd = (c == 'g') ? F_GETLK : (c == 's') ? F_SETLK : F_SETLKW;
        fl.l_type = (l == 'r') ? F_RDLCK : (l == 'w') ? F_WRLCK : F_UNLCK;
        fl.l_whence = SEEK_SET;

        status = fcntl(fd, cmd, &fl);

        if (F_GETLK == cmd) {
            if (-1 == status) {
                errorExit("fcntl(F_GETLK) failed.\n");
            } else {
                if (F_UNLCK == fl.l_type) {
                    printf("[PID=%ld] lock can be placed.\n", (long)getpid());
                } else {
                    printf("[PID=%ld] denied by %s lock on %ld:%ld, held by PID %ld\n"
                           , (long)getpid()
                           , (fl.l_type == F_RDLCK ? "READ" : "WRITE")
                           , start
                           , len
                           , (long)fl.l_pid);
                }
            }
        } else {
            if (0 == status) {
                printf("[PID=%ld] %s\n", (long)getpid(), (l == 'u' ? "unlocked" : "got lock"));
            } else if (EAGAIN == errno || EACCES == errno) {
                printf("[PID=%ld] failed (incompatible lock)\n", (long)getpid());
            } else if (EDEADLK == errno) {
                printf("[PID=%ld] failed (deadlock)\n", (long)getpid());
            } else {
                errorExit("fcntl(F_SETLK(W) failed.\n");
            }
        }
    }
}

} //_2 --------------------------------------------------------------

namespace _3 {

int lockRegionCommon(int fd, int cmd, int type, int whence, int start, off_t len)
{
    struct flock fl;

    fl.l_type = type;
    fl.l_whence = whence;
    fl.l_start = start;
    fl.l_len = len;

    return fcntl(fd, cmd, &fl);
}

int lockRegion(int fd, int type, int whence, int start, off_t len)
{
    return lockRegionCommon(fd, F_SETLK, type, whence, start, len);
}

int lockRegionWait(int fd, int type, int whence, int start, off_t len)
{
    return lockRegionCommon(fd, F_SETLKW, type, whence, start, len);
}

pid_t isRegionLocked(int fd, int type, int whence, int start, off_t len)
{
    struct flock fl;

    fl.l_type = type;
    fl.l_whence = whence;
    fl.l_start = start;
    fl.l_len = len;

    if (-1 == fcntl(fd, F_GETLK, &fl))
        return -1;

    return (fl.l_type == F_UNLCK) ? 0 : fl.l_pid;
}

const int BUF_SIZE = 100;
const int CPF_CLOEXEC = 1;

int createPidFile(const char* program, const char* pidFile, int flags)
{
    int fd = open(pidFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (-1 == fd)
        errorExit("open() failed.\n");

    if (flags & CPF_CLOEXEC) {
        flags = fcntl(fd, F_GETFD);
        if (-1 == flags) {
            fprintf(stderr, "Could not get flags for PID %s\n", pidFile);
            exit(EXIT_FAILURE);
        }

        flags |= FD_CLOEXEC;

        if (-1 == fcntl(fd, F_SETFD, flags)) {
            errorExit("fcntl(F_SETFD) failed.\n");
        }
    }

    if (-1 == lockRegion(fd, F_WRLCK, SEEK_SET, 0, 0)) {
        if (EAGAIN == errno || EACCES == errno) {
            fprintf(stderr, "PID file %s is locked; probably '%s' is already running\n", pidFile, program);
        } else {
            fprintf(stderr, "Unable to lock PID file '%s'\n", pidFile);
        }
        exit(EXIT_FAILURE);
    }

    if (-1 == ftruncate(fd, 0))
        errorExit("ftruncate() failed.\n");

    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE, "%ld\n", (long)getpid());
    if (write(fd, buf, strlen(buf)) != strlen(buf))
        fprintf(stderr, "Writing to PID file '%s'\n", pidFile);

    return fd;
}

} //_3 --------------------------------------------------------------

namespace _4 {



} //_4 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_18(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
#endif

    _2::test(argc, argv);
}
