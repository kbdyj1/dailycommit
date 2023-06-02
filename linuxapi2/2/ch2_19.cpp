#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

namespace { //=================================================================

#define SOCK_NAME   "/tmp/9Lq7BNBnBycd6nxy.sock"

namespace _1 {

void client()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == fd)
        errorExit("socket(AF_UNIX, SOCK_STREAM, 0) failed.\n");

    printf("socket() return %d\n", fd);

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_NAME);

    if (-1 == connect(fd, (sockaddr*)&addr, sizeof(addr))) {
        errorExit("connect() failed.\n");
    } else {
        printf("connect() OK. address: %s\n", addr.sun_path);
        sleep(1);
    }

    close(fd);

    exit(EXIT_SUCCESS);
}

void server()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == fd)
        errorExit("socket(AF_UNIX, SOCK_STREAM, 0) failed.\n");

    printf("socket() return %d\n", fd);

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_NAME);

    if (-1 == bind(fd, (sockaddr*)&addr, sizeof(addr))) {
        close(fd);
        unlink(SOCK_NAME);

        errnoExit("bind", errno);
    } else {
        printf("bind() OK.\n");
    }

    if (-1 == listen(fd, 5)) {
        errnoExit("listen()", errno);
    } else {
        printf("listen() OK.\n");
    }

    sockaddr_un ret;
    socklen_t len = sizeof(ret);
    if (-1 == accept(fd, (sockaddr*)&ret, &len)) {
        errorExit("accept() failed.\n");
    } else {
        printf("accept() OK. %s\n", ret.sun_path);
    }

    sleep(5);

    close(fd);
    unlink(SOCK_NAME);

    exit(EXIT_SUCCESS);
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [s|c]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argv[1][0] == 'c') {
        client();
    } else if (argv[1][0] == 's') {
        server();
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

void server()
{
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == fd)
        errorExit("[SERVER] socket() failed.\n");

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_NAME);

    if (-1 == bind(fd, (sockaddr*)&addr, sizeof(addr))) {
        close(fd);
        unlink(SOCK_NAME);

        errnoExit("bind() failed", errno);
    }
    const int BUF_LEN = 128;
    char buf[BUF_LEN];
    int len = sizeof(addr);
    ssize_t recvLen = recvfrom(fd, buf, BUF_LEN, 0, (sockaddr*)&addr, (socklen_t*)&len);

    buf[recvLen] = 0;
    printf("%lu bytes read: %s\n", recvLen, buf);

    close(fd);
    unlink(SOCK_NAME);
}

void client(const char* message)
{
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == fd)
        errorExit("[SERVER] socket() failed.\n");

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_NAME);

    ssize_t sendLen = sendto(fd, message, strlen(message), 0, (sockaddr*)&addr, sizeof(addr));
    printf("%lu bytes send\n", sendLen);

    close(fd);
    unlink(SOCK_NAME);
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [s|c]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argv[1][0] == 'c') {
        if (argc < 3) {
            client("Hello, Qt6!");
        } else {
            client(argv[2]);
        }
    } else if (argv[1][0] == 's') {
        server();
    }
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_19(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
#endif

    _2::test(argc, argv);
}
