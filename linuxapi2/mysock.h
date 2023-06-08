#ifndef MYSOCK_H
#define MYSOCK_H

#include <sys/socket.h>
#include <netdb.h>

int myConnect(const char* host, const char* service, int type);

int myListen(const char* service, int backlog, socklen_t* addrlen);

int myBind(const char* service, int type, socklen_t* addrlen);

char* myAddressStr(const sockaddr* addr, socklen_t addrlen, char* buf, int bufSize);

#define MY_ADDR_STR_LEN 4096

#endif // MYSOCK_H
