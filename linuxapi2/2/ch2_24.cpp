#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "mysock.h"
#include "utils.h"

namespace { //=================================================================

namespace _1 {

ssize_t readn(int fd, void* buffer, size_t n)
{
    char* buf = (char*)buffer;
    size_t total;

    for (total=0; total<n; ) {
        ssize_t numRead = read(fd, buf, n - total);
        if (0 == numRead) //EOF
            return total;
        if (-1 == numRead) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        total += numRead;
        buf += numRead;
    }
    return total;
}

ssize_t writen(int fd, const void* buffer, size_t n)
{
    const char* buf = (const char*)buffer;
    size_t total;

    for (total = 0; total < n; ) {
        ssize_t numWrite = write(fd, buf, n - total);

        if (numWrite <= 0) {
            if (numWrite == -1 && errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        }
        total += numWrite;
        buf += numWrite;
    }
    return total;
}

} //_1 --------------------------------------------------------------

namespace _2 {

const int BUF_SIZE = 100;
const char* SERVICE = "echo";

void test(int argc, const char* argv[])
{
    int fd = myConnect(argv[1], SERVICE, SOCK_STREAM);
    if (-1 == fd) {
        errnoExit("myConnect", errno);
    }

    char buf[BUF_SIZE];

    switch (fork()) {
    case -1:
        errnoExit("fork", errno);

    case 0:
        for ( ;; ) {
            ssize_t numRead = read(fd, buf, BUF_SIZE);
            if (numRead <= 0)
                break;
            printf("%ld bytes read, %s\n", numRead, buf);
        }
        exit(EXIT_SUCCESS);

    default:
        for ( ;; ) {
            ssize_t numRead = read(STDIN_FILENO, buf, BUF_SIZE);
            if (numRead <= 0)
                break;
            if (write(fd, buf, numRead) != numRead) {
                errorExit("write() failed.\n");
            }
        }
        if (-1 == ::shutdown(fd, SHUT_WR))
            errnoExit("shutdown", errno);
        exit(EXIT_SUCCESS);
    }
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_24(int argc, const char* argv[])
{

}
