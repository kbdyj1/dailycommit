#include <sys/syslog.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mysock.h"
#include "mydaemon.h"
#include "utils.h"

namespace { //=================================================================

const char* SERVICE = "echo";
const int BUF_SIZE = 4096;

namespace _1 {

void sigChildHandler(int sig)
{
    int savedErrno = errno;

    while (0 < waitpid(-1, NULL, WNOHANG))
        ;

    errno = savedErrno;
}

void handleRequest(int fd)
{
    char buf[BUF_SIZE];
    ssize_t numRead;

    while (0 < (numRead = read(fd, buf, BUF_SIZE))) {
        if (numRead != write(fd, buf, numRead)) {
            errorExit("write() failed.\n");
        }
    }

    if (-1 == numRead) {
        errnoExit("read", errno);
    }
}

void server()
{
    socklen_t addrlen;
    int fd = myBind(SERVICE, SOCK_DGRAM, &addrlen);
    if (-1 == fd) {
        errnoExit("myBind", errno);
    }

    char buf[BUF_SIZE];
    sockaddr_storage claddr;
    for ( ;; ) {
        socklen_t len = sizeof(sockaddr_storage);
        ssize_t numRead = recvfrom(fd, buf, BUF_SIZE, 0, (sockaddr*)&claddr, &len);
        if (-1 == numRead) {
            errorExit("recvfrom() failed.\n");
        }

        if (numRead != sendto(fd, buf, numRead, 0, (sockaddr*)&claddr, len)) {
            errorExit("sendto() failed.\n");
        }
    }
}

void client(int argc, const char* argv[])
{
    int fd = myConnect(argv[2], SERVICE, SOCK_DGRAM);
    if (-1 == fd) {
        errorExit("myConnect() failed.\n");
    }

    char buf[BUF_SIZE];

    for (int j=3; j<argc; j++) {
        ssize_t len = strlen(argv[j]);
        if (len != write(fd, argv[j], len)) {
            errorExit("write() failed.\n");
        }

        ssize_t numRead = read(fd, buf, BUF_SIZE);
        if (-1 == numRead) {
            errnoExit("read", errno);
        }

        printf("[%ld bytes] %s\n", numRead, buf);
    }
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [c|s] ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char c = argv[1][0];

    if (c == 'c') {
        client(argc, argv);
    } else if (c == 's') {
        server();
    }
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_23(int argc, const char* argv[])
{
    _1::test(argc, argv);
}
