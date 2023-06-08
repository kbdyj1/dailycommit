#include "mysock.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

namespace  { //----------------------------------------------------------------

int passiveSocket(const char* service, int type, socklen_t* addrlen, bool isListen, int backlog)
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = type;
    hints.ai_flags = AI_PASSIVE;

    addrinfo* result;
    if (0 != getaddrinfo(NULL, service, &hints, &result)) {
        errno = ENOSYS;
        return -1;
    }

    int opt = 1;
    int fd;
    addrinfo* p;
    for (p = result; p != NULL; p = p->ai_next) {
        fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (-1 == fd)
            continue;

        if (isListen) {
            if (-1 == setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
                close(fd);
                freeaddrinfo(result);
                return -1;
            }
        }

        if (0 == bind(fd, p->ai_addr, p->ai_addrlen))
            break;

        close(fd);
    }

    if (NULL != p && isListen) {
        if (-1 == listen(fd, backlog)) {
            freeaddrinfo(result);
            return -1;
        }
    }

    if (NULL != p && NULL != addrlen)
        *addrlen = p->ai_addrlen;

    freeaddrinfo(result);

    return NULL == p ? -1 : fd;
}

} //namespace -----------------------------------------------------------------

int myConnect(const char* host, const char* service, int type)
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = type;

    addrinfo* result;
    if (0 != getaddrinfo(host, service, &hints, &result)) {
        errno = ENOSYS;
        return -1;
    }

    int fd;
    addrinfo* p;
    for (p = result; p != NULL; p = p->ai_next) {
        fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (-1 == fd)
            continue;

        if (-1 != connect(fd, p->ai_addr, p->ai_addrlen))
            break;

        close(fd);
    }

    freeaddrinfo(result);

    return p == NULL ? -1 : fd;
}

int myListen(const char* service, int backlog, socklen_t* addrlen)
{
    return passiveSocket(service, SOCK_STREAM, addrlen, false, backlog);
}

int myBind(const char* service, int type, socklen_t* addrlen)
{
    return passiveSocket(service, type, addrlen, false, 0);
}

char* myAddressStr(const sockaddr* addr, socklen_t addrlen, char* buf, int bufSize)
{
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (0 == getnameinfo(addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV)) {
        snprintf(buf, bufSize, "(%s, %s)", host, service);
    } else {
        snprintf(buf, bufSize, "(?)");
    }

    buf[bufSize - 1] = '\0';

    return buf;
}
