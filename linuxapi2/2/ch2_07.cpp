// PIPE & FIFO

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "utils.h"

namespace { //=================================================================

#define SERVER_FIFO             "/tmp/svr"
#define CLIENT_FIFO_TEMPLATE    "/tmp/client.%ld"
#define CLIENT_FIFO_NAME_LEN    (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

struct request_t {
    pid_t pid;
    int seqLen;
};

struct response_t {
    int seqNum;
};

namespace _1 {

void test()
{
    int fd[2];
    char buf[32];

    if (-1 == pipe(fd)) {
        errorExit("pipe() error.\n");
    }

    switch (fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        // close out
        if (-1 == close(fd[1])) {
            errorExit("CHILD: close(fd[1]) error.\n");
        }
        for ( ;; ) {
            int size = read(fd[0], buf, 32);
            if (-1 == size) {
                errorExit("CHILD: read() -1\n");
            } else if (0 == size) {
                // EOF
                printf("CHILD: EOF\n");
                break;
            }
            buf[size] = '\0';
            printf("CHILD: read(%s)\n", buf);
        }
        if (-1 == close(fd[0])) {
            errorExit("CHILD: close(fd[0]) error.\n");
        }
        _exit(EXIT_SUCCESS);
        break;

    default:
        // close in
        if (-1 == close(fd[0])) {
            errorExit("PARENT: close(fd[0]) error.\n");
        }
        memset(buf, 0, 32);
        strncpy(buf, "Hello, Qt", 9);
        write(fd[1], buf, 32);
        if (-1 == close(fd[1])) {
            errorExit("PARENT: close(fd[1] error.\n");
        }
        // wait for the child to exit
        wait(NULL);

        exit(EXIT_SUCCESS);
        break;
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    int fd[2];

    setbuf(stdout, NULL);

    printf("Parent started.\n");

    if (-1 == pipe(fd)) {
        errorExit("pipe() error.\n");
    }

    const int numChild = 3;
    const int sleepChild[] = { 4, 2, 5 };

    for (int j=0; j<numChild; j++) {
        switch (fork()) {
        case -1:
            errorExit("fork() error.\n");

        case 0:
            if (-1 == close(fd[0])) {
                errorExit("CHILD close(fd[0]) error.\n");
            }
            sleep(sleepChild[j]);

            printf("Child %d (PID:%ld) closing pipe\n", j, (long)getpid());
            if (-1 == close(fd[1])) {
                errorExit("CHILD close(fd[1]) error.\n");
            }

            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }

    if (-1 == close(fd[1])) {
        errorExit("PARENT close(fd[1]) error.\n");
    }
    int dummy;
    if (-1 == read(fd[0], &dummy, 1)) {
        errorExit("PARENT read() error.\n");
    }

    printf("PARENT ready.\n");

    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    int fd[2];

    if (-1 == pipe(fd)) {
        errorExit("pipe() error.\n");
    }

    switch (fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        if (-1 == close(fd[0])) {
            errorExit("CHILD close(fd[0]) error.\n");
        }
        if (fd[1] != STDOUT_FILENO) {
            if (-1 == dup2(fd[1], STDOUT_FILENO)) {
                errorExit("dup2(fd[1], STDOUT_FILENO) error.\n");
            }
            if (-1 == close(fd[1])) {
                errorExit("CHILD close(fd[1]) error.\n");
            }
            execlp("ls", "ls", (char*)NULL);
            printf("CHILD execlp ls\n");
            _exit(EXIT_SUCCESS);
        }

    default:
        break;
    }

    switch (fork()) {
    case -1:
        errorExit("fork() #2 error.\n");

    case 0:
        if (-1 == close(fd[1])) {
            errorExit("CHILD close(fd[0]) #2 error.\n");
        }
        if (fd[0] != STDIN_FILENO) {
            if (-1 == dup2(fd[0], STDIN_FILENO)) {
                errorExit("dup2(fd[1], STDIN_FILENO) #2 error.\n");
            }
            if (-1 == close(fd[0])) {
                errorExit("CHILD close(fd[0]) #2 error.\n");
            }
            execlp("wc", "wc", "-l", (char*)NULL);
            printf("CHILD execlp wc\n");
            _exit(EXIT_SUCCESS);
        }

    default:
        break;
    }

    if (-1 == close(fd[0])) {
        errorExit("close(fd[0]) #3 error\n");
    }
    if (-1 == close(fd[1])) {
        errorExit("close(fd[1]) #3 error\n");
    }
    if (-1 == wait(NULL)) {
        errorExit("wait #1 error.\n");
    }
    if (-1 == wait(NULL)) {
        errorExit("wait #2 error.\n");
    }

    exit(EXIT_SUCCESS);
}

} //_3 --------------------------------------------------------------

namespace _4 {

#define POPEN_FMT   "/bin/ls -d %s 2> /dev/null"
#define PATTERN_SIZE    50
#define PCMD_BUF_SIZE   (sizeof(POPEN_FMT) + PATTERN_SIZE)

void test()
{
    char pattern[PATTERN_SIZE];
    char cmd[PCMD_BUF_SIZE];
    FILE* fp;
    bool badPattern;
    char path[PATH_MAX];
    char c;

    for ( ;; ) {
        printf("pattern: ");
        fflush(stdout);

        if (NULL == fgets(pattern, PATTERN_SIZE, stdin)) {
            break;
        }
        int len = strlen(pattern);
        if (len <= 1)
            continue;

        if (pattern[len-1] == '\n')
            pattern[len-1] = '\0';

        badPattern=false;
        for (int j=0; j<len && !badPattern; j++) {
            c = pattern[j];
            if (!isalnum((unsigned char)c) && strchr("_*?[^-]", c) == NULL) {
                badPattern = true;
            }
        }

        if (badPattern) {
            printf("bad pattern: %c\n", c);
            continue;
        }

        snprintf(cmd, PCMD_BUF_SIZE, POPEN_FMT, pattern);
        cmd[PCMD_BUF_SIZE-1] = '\0';

        fp = popen(cmd, "r");
        if (NULL == fp) {
            printf("popen() failed.\n");
            continue;
        }

        int fileCnt = 0;
        while (fgets(path, PATH_MAX, fp) != NULL) {
            printf("%s", path);
            fileCnt++;
        }

        int status = pclose(fp);
        printf("%d matching file%s\n", fileCnt, (fileCnt != 1) ? "s" : "");
        printf("pclose() status == %#x\n", (unsigned int)status);

        if (-1 != status) {
            printWaitStatus("\t", status);
        }

        exit(EXIT_SUCCESS);
    }
}

} //_4 --------------------------------------------------------------

namespace _5 {

void server()
{
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    struct request_t req;
    struct response_t res;
    int seqNum = 0;

    umask(0);

    if (-1 == mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) && errno != EEXIST) {
        errorExit("mkfifo() error");
    }

    int serverFd = open(SERVER_FIFO, O_RDONLY);
    if (-1 == serverFd) {
        errorExit("open(SERVER_FIFO, O_RDONLY) error.\n");
    }

    int dummyFd = open(SERVER_FIFO, O_WRONLY);
    if (-1 == dummyFd) {
        errorExit("open(SERVER_FIFO, O_WRONLY) error.\n");
    }

    if (SIG_ERR == signal(SIGPIPE, SIG_IGN)) {
        errorExit("signal(SIGPIPE, SIG_IGN) error.\n");
    }

    for ( ;; ) {
        if (read(serverFd, &req, sizeof(struct request_t) != sizeof(struct request_t))) {
            fprintf(stderr, "read() error.\n");
            continue;
        }

        memset(clientFifo, 0, CLIENT_FIFO_NAME_LEN);

        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)req.pid);
        int clientFd = open(clientFifo, O_WRONLY);
        if (-1 == clientFd) {
            fprintf(stderr, "open %s error.\n", clientFifo);
            continue;
        }

        res.seqNum = seqNum;

        if (write(clientFd, &res, sizeof(struct response_t)) != sizeof(struct response_t)) {
            fprintf(stderr, "write(lientFd, &res, sizeof(struct response_t)) error.\n");
        }

        if (-1 == close(clientFd)) {
            fprintf(stderr, "close() error.\n");
        }

        seqNum += 1;
    }

    exit(EXIT_SUCCESS);
}

} //_5 --------------------------------------------------------------

namespace _6 {

char clientFifo[CLIENT_FIFO_NAME_LEN];

void removeFifo()
{
    unlink(clientFifo);
}

void client()
{
    umask(0);

    memset(clientFifo, 0, CLIENT_FIFO_NAME_LEN);

    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)getpid());
    if (-1 == mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) && errno != EEXIST) {
        errorExit("CLIENT: mkfifo() error.\n");
    } else {
        printf("CLIENT: %s created.\n", clientFifo);
    }
    if (0 != atexit(removeFifo)) {
        errorExit("atexit(removeFifo) error.\n");
    }

    struct request_t req;
    struct response_t res;

    req.pid = getpid();
    req.seqLen = 1;

    int serverFd = open(SERVER_FIFO, O_WRONLY);
    if (-1 == serverFd) {
        errorExit("CLIENT: open(SERVER_FIFO, O_WRONLY) error.\n");
    }

    if (write(serverFd, &req, sizeof(struct request_t)) != sizeof(struct request_t)) {
        errorExit("CLIENT: write() error.\n");
    }

    int clientFd = open(clientFifo, O_RDONLY);
    if (-1 == clientFd) {
        errorExit("CLIENT: open(, O_RDONLY) error.\n");
    }

    if (read(clientFd, &res, sizeof(struct response_t)) != sizeof(struct response_t)) {
        errorExit("CLIENT, read() error.\n");
    }

    printf("CLIENT: received %d\n", res.seqNum);

    exit(EXIT_SUCCESS);
}

} //_6 --------------------------------------------------------------

} //namespace =================================================================

#define USE_FIFO_SERVER

void exec_ch_07()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
#endif

#if defined(USE_FIFO_SERVER)
    _5::server();
#else
    _6::client();
#endif
}
