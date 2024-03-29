#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
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

namespace _3 {

const int IS_ADDR_STR_LEN = 256;

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s service\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    socklen_t len;
    int lfd = myListen(argv[1], 5, &len);
    if (-1 == lfd) {
        errnoExit("myListen", errno);
    }

    int confd = myConnect(NULL, argv[1], SOCK_STREAM);
    if (-1 == confd) {
        errnoExit("myConnect", errno);
    }

    int afd = accept(lfd, NULL, NULL);
    if (-1 == afd) {
        errnoExit("accept", errno);
    }

    void* addr = malloc(len);
    if (NULL == addr) {
        errorExit("malloc() failed.\n");
    }

    if (-1 == getsockname(confd, (sockaddr*)addr, &len)) {
        errnoExit("getsockname(con)", errno);
    }

    char buf[IS_ADDR_STR_LEN];
    printf("getsockname(confd): %s\n", myAddressStr((sockaddr*)addr, len, buf, IS_ADDR_STR_LEN));

    if (-1 == getsockname(afd, (sockaddr*)addr, &len)) {
        errnoExit("getsockname(accept)", errno);
    }

    printf("getsockname(accept): %s\n", myAddressStr((sockaddr*)addr, len, buf, IS_ADDR_STR_LEN));

    if (-1 == getpeername(confd, (sockaddr*)addr, &len)) {
        errnoExit("getpeername(con)", errno);
    }
    printf("getpeername(confd): %s\n", myAddressStr((sockaddr*)addr, len, buf, IS_ADDR_STR_LEN));

    if (-1 == getpeername(afd, (sockaddr*)addr, &len)) {
        errnoExit("getpeername(accept)", errno);
    }
    printf("getpeername(accept): %s\n", myAddressStr((sockaddr*)addr, len, buf, IS_ADDR_STR_LEN));

    sleep(30);
    exit(EXIT_SUCCESS);
}

} //_3 --------------------------------------------------------------

namespace _4 {

const char* SOCK_PATH = "scm_rights";

int unixConnect(const char* path, int type)
{
    return 0;
}

void test(int argc, const char* argv[])
{
    if (1 == argc) {
        fprintf(stderr, "Usage: %s [d] file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    union {
        char buf[CMSG_SPACE(sizeof(int))];
        cmsghdr align;
    } controlMsg;

    bool useDatagramSocket = false;
    int fileIndex = 1;
    if (3 == argc) {
        useDatagramSocket = ('d' == argv[1][0]);
        fileIndex = 2;
    }

    int fd = open(argv[fileIndex], O_RDONLY);
    if (-1 == fd) {
        errnoExit("open", errno);
    }

    msghdr header;
    header.msg_name = NULL;
    header.msg_namelen = 0;

    iovec iov;
    int data = 12345;

    header.msg_iov = &iov;
    header.msg_iovlen = 1;
    iov.iov_base = &data;
    iov.iov_len = sizeof(data);

    printf("sending data = %d\n", data);

    header.msg_control = controlMsg.buf;
    header.msg_controllen = sizeof(controlMsg.buf);

    memset(controlMsg.buf, 0, sizeof(controlMsg.buf));

    cmsghdr* pHeader = CMSG_FIRSTHDR(&header);
    pHeader->cmsg_len = CMSG_LEN(sizeof(int));
    pHeader->cmsg_level = SOL_SOCKET;
    pHeader->cmsg_type = SCM_RIGHTS;
    memcpy(CMSG_DATA(pHeader), &fd, sizeof(int));

    int sfd = unixConnect(SOCK_PATH, useDatagramSocket ? SOCK_DGRAM : SOCK_STREAM);
    if (-1 == sfd) {
        errnoExit("myConnect", errno);
    }

    ssize_t ret = sendmsg(sfd, &header, 0);
    if (-1 == ret) {
        errnoExit("sendmsg", errno);
    }

    printf("sendmsg() returned %zd\n", ret);

    if (-1 == close(fd)) {
        errnoExit("close", errno);
    }

    exit(EXIT_SUCCESS);
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_24(int argc, const char* argv[])
{
    _3::test(argc, argv);
}
