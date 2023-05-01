// PIPE & FIFO

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <sys/wait.h>

#include "utils.h"

namespace { //=================================================================

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

} //namespace =================================================================

void exec_ch_07()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
