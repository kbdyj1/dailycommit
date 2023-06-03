#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include "utils.h"

#define SOCK_NAME   "/tmp/linux2.ch20.sock"
#define BACKLOGS    5

#define STREAM_SERVER   0
#define DATAGRAM_SERVER 1

#if (0)
#   define SERVER_TYPE STREAM_SERVER
#else
#   define SERVER_TYPE DATAGRAM_SERVER
#endif

namespace { //=================================================================

namespace _1 {

const int BUF_SIZE = 128;

void streamClient()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == fd) {
        errnoExit("socket", errno);
    }

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_NAME, sizeof(addr.sun_path)-1);

    if (-1 == connect(fd, (sockaddr*)&addr, sizeof(addr))) {
        errnoExit("connect", errno);
    }

    ssize_t numReads;
    char buf[BUF_SIZE];
    while (0 < (numReads = read(STDIN_FILENO, buf, BUF_SIZE))) {
        if (numReads != write(fd, buf, numReads)) {
            errorExit("write() failed.\n");
        }
    }

    if (-1 == numReads) {
        errnoExit("read", errno);
    }
}

void datagramClient(int argc, const char* argv[])
{
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == fd) {
        errnoExit("socket", errno);
    }

    sockaddr_un claddr;
    memset(&claddr, 0, sizeof(claddr));
    claddr.sun_family = AF_UNIX;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path), "%s.%ld", SOCK_NAME, (long)getpid());

    if (-1 == bind(fd, (sockaddr*)&claddr, sizeof(claddr))) {
        errnoExit("bind", errno);
    }

    sockaddr_un svaddr;
    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SOCK_NAME, sizeof(svaddr.sun_path)-1);

    char buf[BUF_SIZE];
    for (int j=2; j<argc; j++) {
        ssize_t msgLen = strlen(argv[j]);
        if (msgLen != sendto(fd, argv[j], msgLen, 0, (sockaddr*)&svaddr, sizeof(svaddr))) {
            errorExit("sendto() failed.\n");
        }

        ssize_t numBytes = recvfrom(fd, buf, BUF_SIZE, 0, NULL, NULL);
        if (-1 == numBytes) {
            errnoExit("recvfrom", errno);
        }
        buf[numBytes] = '\0';

        printf("response %ld: %s\n", numBytes, buf);
    }

    remove(claddr.sun_path);
}

void client(int argc, const char* argv[])
{
#if (SERVER_TYPE == STREAM_SERVER)
    streamClient();
#else
    datagramClient(argc, argv);
#endif

    exit(EXIT_SUCCESS);
}

void streamServer(int fd)
{
    if (-1 == listen(fd, BACKLOGS)) {
        errnoExit("listen", errno);
    }

    ssize_t numReads;
    char buf[BUF_SIZE];
    for ( ;; ) {
        int cfd = accept(fd, NULL, NULL);
        if (-1 == cfd) {
            errnoExit("accept", errno);
        }

        while (0 < (numReads = read(cfd, buf, BUF_SIZE))) {
            if (numReads != write(STDOUT_FILENO, buf, numReads)) {
                errorExit("write() failed.");
            }
        }

        if (-1 == numReads) {
            errnoExit("read", errno);
        }

        if (-1 == close(cfd)) {
            errnoExit("close", errno);
        }
    }
}

void datagramServer(int fd)
{
    sockaddr_un addr;
    char buf[BUF_SIZE];

    for ( ;; ) {
        socklen_t len = sizeof(sockaddr_un);
        ssize_t numBytes = recvfrom(fd, buf, BUF_SIZE, 0, (sockaddr*)&addr, &len);
        if (-1 == numBytes) {
            errnoExit("recvfrom", errno);
        }

        printf("server received %ld bytes from %s\n", numBytes, addr.sun_path);

        for (int j=0; j<numBytes; j++)
            buf[j] = toupper(buf[j]);

        if (numBytes != sendto(fd, buf, numBytes, 0, (sockaddr*)&addr, len)) {
            errorExit("sendto() failed.\n");
        }
    }
}

void server()
{
#if (SERVER_TYPE == STREAM_SERVER)
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
#else
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
#endif
    if (-1 == fd) {
        errnoExit("socket", errno);
    }

    if (-1 == remove(SOCK_NAME) && errno != ENOENT) {
        errnoExit("remove", errno);
    }

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_NAME, sizeof(addr.sun_path)-1);

    if (-1 == bind(fd, (sockaddr*)&addr, sizeof(addr))) {
        errnoExit("bind", errno);
    }

#if (SERVER_TYPE == STREAM_SERVER)
    streamServer(fd);
#else
    datagramServer(fd);
#endif
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [s|c]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argv[1][0] == 'c') {
        client(argc, argv);
    } else if (argv[1][0] == 's') {
        server();
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    int fds[2];
    if (-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, fds)) {
        errnoExit("socketpair", errno);
    }

    printf("socketpair() fd[0]: %d, fd[1]: %d\n", fds[0], fds[1]);

    close(fds[0]);
    close(fds[1]);
}

} //_2 --------------------------------------------------------------

namespace _3 { // abstract socket

void test()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == fd) {
        errnoExit("socket", errno);
    }

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(&addr.sun_path[1], SOCK_NAME, strlen(SOCK_NAME));

    if (-1 == bind(fd, (sockaddr*)&addr, sizeof(sa_family_t) + strlen(SOCK_NAME) + 1)) {
        errnoExit("bind", errno);
    }
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_20(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
    _2::test();
#endif

    _3::test();
}
