#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
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

} //namespace =================================================================

void exec_ch_18(int argc, const char* argv[])
{
    _1::test(argc, argv);
}
