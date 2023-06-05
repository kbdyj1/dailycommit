#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

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

} //namespace =================================================================

void exec_ch_22(int argc, const char* argv[])
{
    _2::test();
}
