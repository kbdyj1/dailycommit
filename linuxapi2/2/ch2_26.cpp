#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/epoll.h>

#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test()
{
    printf("FD_SETSIZE: %d\n", FD_SETSIZE); //1024
}

} //_1 --------------------------------------------------------------

namespace _2 {

void usageError(const char* program)
{
    fprintf(stderr, "Usage: %s timeout fd[rw] ...\n", program);
    exit(EXIT_FAILURE);
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        usageError(argv[0]);
    }
    timeval* pto = NULL;
    timeval timeout;

    if (0 != strcmp(argv[1], "-")) {
        pto = &timeout;
        timeout.tv_sec = atoi(argv[1]);
        timeout.tv_usec = 0;
    }

    int fds = 0;
    fd_set rfds;
    fd_set wfds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    int numRead;
    char buf[10];
    int fd;

    for (int i=2; i<argc; i++) {
        numRead = sscanf(argv[i], "%d%2[rw]", &fd, buf);
        if (2 != numRead) {
            usageError(argv[0]);
        }
        if (FD_SETSIZE <= fd) {
            errorExit("file descriptor exceeds limit (FD_SETSIZE <= %d)\n", fd);
        }
        if (fds <= fd)
            fds = fd + 1;
        if (NULL != strchr(buf, 'r')) {
            FD_SET(fd, &rfds);
        }
        if (NULL != strchr(buf, 'w')) {
            FD_SET(fd, &wfds);
        }
    }

    int ready = select(fds, &rfds, &wfds, NULL, pto);
    if (-1 == ready) {
        errnoExit("select", errno);
    }
    printf("ready: %d\n", ready);

    for (int i=0; i<fds; i++) {
        printf("%d: %s%s\n", i, FD_ISSET(i, &rfds) ? "r" : " ", FD_ISSET(i, &wfds) ? "w" : " ");
    }
    if (NULL != pto) {
        printf("timeout after select(): %lds\n", (long)timeout.tv_sec);
    }
    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s num-pipes [num-writes]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numPipes = atoi(argv[1]);
    int (*pfds)[2];

    pfds = (int(*)[2]) calloc(numPipes, sizeof(int[2]));
    if (NULL == pfds) {
        errorExit("calloc");
    }

    pollfd* pollFd = (pollfd*) calloc(numPipes, sizeof(pollfd));
    if (NULL == pollFd) {
        errorExit("calloc");
    }

    for (int j=0; j<numPipes; j++) {
        if (-1 == pipe(pfds[j])) {
            errnoExit("pipe", errno);
        }
    }

    int numWrites = 1;
    if (2 < argc) {
        numWrites = atoi(argv[2]);
    }

    srand(time(NULL));
    for (int j=0; j<numWrites; j++) {
        int r = random() % numPipes;
        printf("Writing to fd: %3d (read fd: %3d)\n", pfds[r][1], pfds[r][0]);
        if (-1 == write(pfds[r][1], "a", 1)) {
            errnoExit("write", errno);
        }
    }

    for (int j=0; j<numWrites; j++) {
        pollFd[j].fd = pfds[j][0];
        pollFd[j].events = POLL_IN;
    }

    int ready = poll(pollFd, numPipes, -1);
    if (-1 == ready) {
        errnoExit("poll", errno);
    }

    printf("poll() returned: %d\n", ready);

    for (int j=0; j<numPipes; j++) {
        if (pollFd[j].revents & POLL_IN) {
            printf("Readable: %d %3d\n", j, pollFd[j].fd);
        }
    }
}

} //_3 --------------------------------------------------------------

namespace _4 {

volatile sig_atomic_t gotSigIO = 0;

void sigHandler(int sig)
{
    gotSigIO = 1;
}

int ttySetCBreak(int fd, termios* ti)
{
    termios t;
    if (-1 == tcgetattr(fd, &t)) {
        return -1;
    }
    if (NULL != ti)
        *ti = t;

    t.c_lflag &= ~(ICANON | ECHO);
    t.c_lflag |= ISIG;

    t.c_iflag &= ~ICRNL;

    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;

    if (-1 == tcsetattr(fd, TCSAFLUSH, &t)) {
        return -1;
    }

    return 0;
}


void test()
{
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sigHandler;

    if (-1 == sigaction(SIGIO, &sa, NULL)) {
        errnoExit("sigaction", errno);
    }

    if (-1 == fcntl(STDIN_FILENO, F_SETOWN, getpid())) {
        errnoExit("fcntl(F_SETOWN)", errno);
    }

    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (-1 == fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC | O_NONBLOCK)) {
        errnoExit("fcntl(F_SETFL)", errno);
    }

    termios orig;
    if (-1 == ttySetCBreak(STDIN_FILENO, &orig)) {
        errnoExit("ttySetCBreak", errno);
    }

    bool done = false;
    for (int i=0; !done; i++) {
        for (int j=0; j<100000000; j++) {
            continue;
        }
        if (gotSigIO) {
            char c;
            while (0 < read(STDIN_FILENO, &c, 1) && !done) {
                printf("cnt=%d read=%c\n", i, c);
                done = c == '#';
            }
            gotSigIO = 0;
        }
    }

    if (-1 == tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig)) {
        errnoExit("tcsetattr", errno);
    }

    exit(EXIT_SUCCESS);
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test()
{
    int sig = fcntl(STDIN_FILENO, F_GETSIG);
    if (0 == sig) {
        sig = SIGIO;    // default
    }
    printf("fcntl(STDIN_FILENO, F_GETSIG): %s\n", strsignal(sig));
}

} //_5 --------------------------------------------------------------

namespace _6 {

void test()
{
    int fd = epoll_create(8);
    if (-1 == fd) {
        errnoExit("epoll_create", errno);
    }
}

} //_6 --------------------------------------------------------------

namespace _7 {

const int MAX_BUF = 1000;
const int MAX_EVENTS = 5;

void test(int argc, const char* argv[])
{
    int epfd = epoll_create(argc -1);
    if (-1 == epfd)
        errnoExit("epoll_create", errno);

    for (int i=1; i<argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (-1 == fd)
            errnoExit("open", errno);

        printf("Opened \"%s\" on fd %d\n", argv[i], fd);

        epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = fd;

        if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev))
            errnoExit("epoll_ctl", errno);
    }

    int openFds = argc - 1;
    epoll_event evlist[MAX_EVENTS];
    char buf[MAX_BUF];

    while (0 < openFds) {
        printf("About to epoll_wait()\n");
        int ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1);
        if (-1 == ready) {
            if (EINTR == errno)
                continue;
            else
                errnoExit("epoll_wait", errno);
        }

        printf("Ready: %d\n", ready);

        for (int j=0; j<ready; j++) {
            printf("\tfd=%d; events: %s%s%s\n", evlist[j].data.fd,
                   (evlist[j].events & EPOLLIN) ? "EPOLLIN " : "",
                    (evlist[j].events & EPOLLHUP) ? "EPOLLHUP " : "",
                    (evlist[j].events & EPOLLERR) ? "EPOLLERR " : "");

            if (evlist[j].events & EPOLLIN) {
                int s = read(evlist[j].data.fd, buf, MAX_BUF);
                if (-1 == s)
                    errnoExit("read", errno);
            } else if (evlist[j].events & (EPOLLHUP | EPOLLERR)) {
                printf("\tclosing fd %d\n", evlist[j].data.fd);
                if (-1 == close(evlist[j].data.fd))
                    errnoExit("close", errno);

                --openFds;
            }
        }
    }

    printf("All file descriptors closed; bye\n");
    exit(EXIT_SUCCESS);
}

} //_7 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_26(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
    _2::test(argc, argv);
    _3::test(argc, argv);
    _4::test();
    _5::test();
    _6::test();
#endif

    _7::test(argc, argv);
}
