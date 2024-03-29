#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <memory.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include "utils.h"

namespace { //=================================================================

ssize_t readLine(int fd, void* buffer, size_t n)
{
    if (n <= 0 || NULL == buffer) {
        errno = EINVAL;
        return -1;
    }

    char* buf = (char*)buffer;
    size_t toRead = 0;
    ssize_t numRead;
    char c;

    for ( ;; ) {
        numRead = read(fd, &c, 1);
        if (-1 == numRead) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        } else if (0 == numRead) {
            if (0 == toRead) {
                return 0;
            } else {
                break;
            }
        } else {
            if (toRead < n - 1) {
                toRead++;
                *buf++ = c;
            }
            if ('\n' == c)
                break;
        }
    }
    *buf = '\0';

    return toRead;
}

namespace _1 {

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    const char* ip4 = "192.168.0.1";
    in_addr in;
    if (0 == inet_aton(ip4, &in)) {
        errorExit("invalid address\n");
    }

    printf("%s -> %d\n", ip4, in.s_addr);

    in.s_addr = 16820416;
    char* p = inet_ntoa(in);
    printf("%d -> %s\n", in.s_addr, p);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    addrinfo* result;
    int ret = getaddrinfo("qt.io", "443", &hints, &result);
    if (0 != ret) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    for (addrinfo* info = result; info != NULL; info = result->ai_next) {
        int s = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
        if (-1 == s)
            continue;

        printf("socktype: %s\n", info->ai_family == AF_INET ? "IP4" : "IP6");
        switch (info->ai_family) {
        case AF_INET:
            {
                sockaddr_in* sin = (sockaddr_in*)info->ai_addr;
                char* p = inet_ntoa(sin->sin_addr);
                printf("IP4: %s, port: %d\n", p, sin->sin_port);
            }
            break;

        case AF_INET6:
            {
                char buf[128] = {0, };
                sockaddr_in6* sin6 = (sockaddr_in6*)info->ai_addr;
                inet_ntop(AF_INET6, &sin6->sin6_addr, buf, 128);
                printf("address: %s\n", buf);
            }
            break;

        default:
            fprintf(stderr, "Unknown AF\n");
            break;
        }
    }

    freeaddrinfo(result);
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test()
{
    sockaddr_in in;
    memset(&in, 0, sizeof(in));
    if (0 == inet_aton("52.18.144.254", &in.sin_addr)) {
        errorExit("invalid address");
    }
    in.sin_family = AF_INET;
    in.sin_port = 47873;

    sockaddr* addr = (sockaddr*)&in;
    socklen_t addrlen = sizeof(in);
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    if (0 == getnameinfo(addr, addrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NAMEREQD | NI_NUMERICSERV)) {
        printf("host=%s, serv=%s\n", hbuf, sbuf);
    }
}

} //_4 --------------------------------------------------------------

namespace _5 {

const int BUF_SIZE = 10;
const int PORT_NUM = 50002;

void server(int argc, const char* argv[])
{
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (-1 == fd) {
        errnoExit("socket", errno);
    }

    sockaddr_in6 svaddr;
    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_addr = in6addr_any;
    svaddr.sin6_port = htons(PORT_NUM);

    if (-1 == bind(fd, (sockaddr*)&svaddr, sizeof(svaddr))) {
        errnoExit("bind", errno);
    }

    char buf[BUF_SIZE];
    sockaddr_in6 claddr;

    char ip6[INET6_ADDRSTRLEN];

    for ( ;; ) {
        socklen_t len = sizeof(sockaddr_in6);
        ssize_t numBytes = recvfrom(fd, buf, BUF_SIZE, 0, (sockaddr*)&claddr, &len);
        if (-1 == numBytes) {
            errnoExit("recvfrom", errno);
        }

        if (NULL == inet_ntop(AF_INET6, &claddr.sin6_addr, ip6, INET6_ADDRSTRLEN)) {
            printf("Couldn't convert client address to string.\n");
        } else {
            printf("Server received %ld bytes from (%s, %u)\n", (long)numBytes, ip6, ntohs(claddr.sin6_port));
        }

        for (int j=0; j<numBytes; j++)
            buf[j] = toupper(buf[j]);

        if (numBytes != sendto(fd, buf, numBytes, 0, (sockaddr*)&claddr, len)) {
            errorExit("sendto() failed.\n");
        }
    }
}

void client(int argc, const char* argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: %s c host message\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (-1 == fd) {
        errnoExit("socket", errno);
    }

    sockaddr_in6 svaddr;
    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_port = htons(PORT_NUM);

    if (0 >= inet_pton(AF_INET6, argv[2], &svaddr.sin6_addr)) {
        errorExit("inet_pton() failed.\n");
    }

    char resp[BUF_SIZE];

    for (int j=3; j<argc; j++) {
        ssize_t len = strlen(argv[j]);
        if (len != sendto(fd, argv[j], len, 0, (sockaddr*)&svaddr, sizeof(svaddr))) {
            errorExit("sendto() failed.\n");
        }

        ssize_t numBytes = recvfrom(fd, resp, BUF_SIZE, 0, NULL, NULL);
        if (-1 == numBytes) {
            errnoExit("recvfrom", errno);
        }

        printf("Response: %d: %s\n", j-1, resp);
    }
}

void test(int argc, const char* argv[])
{
    if (argc < 2 || (argv[1][0] != 'c' && argv[1][0] != 's')) {
        fprintf(stderr, "Usage: %s [c|s] ...\n", argv[0]);
    }

    char c = argv[1][0];
    if (c == 's') {
        server(argc, argv);
    } else {
        client(argc, argv);
    }
}

} //_5 --------------------------------------------------------------

namespace _6 {

const char* PORT_NUM = "50000";
const int INT_LEN = 30;
const int BACKLOG = 50;

void server(int argc, const char* argv[])
{
    if (SIG_ERR == signal(SIGPIPE, SIG_IGN)) {
        errorExit("signal(SIGPIPE, SIG_IGN) failed.\n");
    }

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    addrinfo* result;

    if (0 != getaddrinfo(NULL, PORT_NUM, &hints, &result)) {
        errorExit("getaddrinfo() failed.\n");
    }

    int optVal = 1;
    addrinfo* p;
    int fd;
    for (p = result; p != NULL; p = p->ai_next) {
        fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (-1 == fd)
            continue;

        if (-1 == setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal))) {
            errnoExit("setsockopt", errno);
        }

        if (0 == bind(fd, p->ai_addr, p->ai_addrlen))
            break;

        close(fd);
    }

    if (NULL == p) {
        errorExit("Could not bind socket to any address.\n");
    }

    if (-1 == listen(fd, BACKLOG)) {
        errnoExit("listen", errno);
    }

    freeaddrinfo(result);

    sockaddr_storage claddr;
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char reqBuf[INT_LEN];
    char resBuf[INT_LEN];
    uint32_t seqNum = argc ? atoi(argv[0]) : 0;

    for ( ;; ) {
        socklen_t addrLen = sizeof(sockaddr_storage);
        int cfd = accept(fd, (sockaddr*)&claddr, &addrLen);
        if (-1 == cfd) {
            fprintf(stderr, "accept() failed.\n");
            continue;
        }

        if (0 == getnameinfo((sockaddr*)&claddr, addrLen, host, NI_MAXHOST, service, NI_MAXSERV, 0)) {
            printf("Connection from (%s, %s)\n", host, service);
        } else {
            printf("Connection from: ?\n");
        }

        if (0 >= readLine(cfd, reqBuf, INT_LEN)) {
            close(cfd);
            continue;
        }

        int reqLen = atoi(reqBuf);
        if (reqLen <= 0) {
            close(cfd);
            continue;
        }

        snprintf(resBuf, INT_LEN, "%d\n", seqNum);

        ssize_t len = strlen(resBuf);
        if (len != write(cfd, resBuf, len)) {
            fprintf(stderr, "write() error.\n");
        }

        seqNum += reqLen;

        if (-1 == close(cfd)) {
            errnoExit("close", errno);
        }
    }
}

void client(int argc, const char* argv[])
{
    if (argc < 1) {
        fprintf(stderr, "Usage: program c host (number)\n");
        return;
    }
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    addrinfo* result;
    if (0 != getaddrinfo(argv[0], PORT_NUM, &hints, &result)) {
        errorExit("getaddrinfo() failed.\n");
    }

    int cfd;
    addrinfo* p;
    for (p = result; p != NULL; p = result->ai_next) {
        cfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (-1 == cfd)
            continue;

        if (-1 != connect(cfd, p->ai_addr, p->ai_addrlen))
            break;

        close(cfd);
    }

    if (NULL == p) {
        errorExit("Could not connect socket to any address.\n");
    }

    freeaddrinfo(result);

    const char* reqStr = (1 < argc) ? argv[1] : "1";
    ssize_t reqLen = strlen(reqStr);

    if (reqLen != write(cfd, reqStr, reqLen)) {
        errorExit("write() failed.\n");
    }
    if (1 != write(cfd, "\n", 1)) {
        errorExit("write(\\n) failed.");
    }

    char buf[INT_LEN];
    ssize_t numRead = readLine(cfd, buf, INT_LEN);
    if (-1 == numRead) {
        errnoExit("readLine", errno);
    }
    if (0 == numRead) {
        errorExit("Unexpected EOF from server");
    }

    printf("Sequence: %s", buf);
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        return;
    }
    char c = argv[1][0];

    if (c == 's') {
        server(argc-2, argv+2);
    } else  if (c == 'c'){
        client(argc-2, argv+2);
    }
}

} //_6 --------------------------------------------------------------

namespace _7 {

void test(int argc, const char* argv[])
{
    char str[INET6_ADDRSTRLEN];

    for (int i=1; i<argc; i++) {
        hostent* h = gethostbyname(argv[i]);
        if (NULL == h) {
            fprintf(stderr, "gethostbyname(%s) failed.\n", argv[i]);
            continue;
        }

        printf("Canonical name: %s\n", h->h_name);
        printf(" alias(es): ");
        for (char** pp = h->h_aliases; *pp != NULL; pp++)
            printf(" %s", *pp);

        int type = h->h_addrtype;
        printf("\n address type: %s\n", (type == AF_INET) ? "AF_INET" : (type == AF_INET6) ? "AF_INET6" : "?");
        if (type == AF_INET || type == AF_INET6) {
            printf(" address(es): ");
            for (char** pp = h->h_addr_list; *pp != NULL; pp++) {
                printf(" %s", inet_ntop(h->h_addrtype, *pp, str, INET6_ADDRSTRLEN));
            }
            printf("\n");
        }
    }
}

} //_7 --------------------------------------------------------------

namespace _8 {

void test(int argc, const char* argv[])
{
    if (2 < argc) {
#if (1)
        servent* s = getservbyname(argv[1], argv[2]);
#else
        int port = htons(atoi(argv[1]));
        servent* s = getservbyport(port, argv[2]);
#endif
        if (s) {
            printf("Canonical name: %s\n", s->s_name);
            printf(" alias(es): ");
            for (char** pp = s->s_aliases; *pp != NULL; pp++) {
                printf(" %s", *pp);
            }
            printf("\n port: %d\n", ntohs(s->s_port));
            printf(" protocol: %s\n", s->s_proto);
        }
    }
}

} //_8 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_22(int argc, const char* argv[])
{
#if (0) //done
    _2::test();
    _3::test();
    _4::test();
    _5::test(argc, argv);
    _6::test(argc, argv);
    _7::test(argc, argv);
#endif

    _8::test(argc, argv);
}
