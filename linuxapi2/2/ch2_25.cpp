#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void print_termios(const char* name, int fd)
{
    termios t;
    tcgetattr(fd, &t);
    printf("%s\n", name);
    printf("\tc_iflag: %d\n", t.c_iflag);
    printf("\tc_oflag: %d\n", t.c_oflag);
    printf("\tc_cflag: %d\n", t.c_cflag);
    printf("\tc_lflag: %d\n\n", t.c_lflag);

    printf("\t[%c] Ctrl+C\n", (t.c_cc[VINTR] ? 'v' : ' '));
    printf("\t[%c] Ctrl+\\\n", (t.c_cc[VQUIT] ? 'v' : ' '));
    printf("\t[%c] Del\n", (t.c_cc[VERASE] ? 'v' : ' '));
    printf("\t[%c] @\n", (t.c_cc[VKILL] ? 'v' : ' '));
    printf("\t[%c] Ctrl+D\n", (t.c_cc[VEOF] ? 'v' : ' '));
    printf("\t[%c] inter-character timer\n", (t.c_cc[VTIME] ? 'v' : ' '));
    printf("\t[%c] blocking read until 1 character arrives\n", (t.c_cc[VINTR] ? 'v' : ' '));
    printf("\t[%c] \\0\n", (t.c_cc[VINTR] ? 'v' : ' '));
    printf("\t[%c] Ctrl+Q\n", (t.c_cc[VSTART] ? 'v' : ' '));
    printf("\t[%c] Ctrl+S\n", (t.c_cc[VSTOP] ? 'v' : ' '));
    printf("\t[%c] Ctrl+Z\n", (t.c_cc[VSUSP] ? 'v' : ' '));
    printf("\t[%c] EOL\n", (t.c_cc[VEOL] ? 'v' : ' '));
    printf("\t[%c] Ctrl+R\n", (t.c_cc[VREPRINT] ? 'v' : ' '));
    printf("\t[%c] Ctrl+U\n", (t.c_cc[VDISCARD] ? 'v' : ' '));
    printf("\t[%c] Ctrl+W\n", (t.c_cc[VWERASE] ? 'v' : ' '));
    printf("\t[%c] Ctrl+V\n", (t.c_cc[VLNEXT] ? 'v' : ' '));
    printf("\t[%c] EOL0\n", (t.c_cc[VEOL2] ? 'v' : ' '));
}

void test()
{
    int mc = pathconf("/", _PC_MAX_CANON);
    printf("_PC_MAX_CANON: %d\n", mc);

    int mi = pathconf("/", _PC_MAX_INPUT);
    printf("_PC_MAX_INPUT: %d\n", mi);

    print_termios("STDIN_FILENO", STDIN_FILENO);    
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test(int argc, const char* argv[])
{
    int interruptCh;
    if (1 == argc) {
        interruptCh = fpathconf(STDIN_FILENO, _PC_VDISABLE);
        if (-1 == interruptCh) {
            errnoExit("fpathconf(STDIN_FILENO, _PC_VDISABLE)", errno);
        }
    } else if (isdigit(argv[1][0])) {
        interruptCh = strtoul(argv[1], NULL, 0);
    } else {
        interruptCh = argv[1][0];
    }

    termios ti;

    if (-1 == tcgetattr(STDIN_FILENO, &ti)) {
        errnoExit("tcgetattr", errno);
    }

    ti.c_cc[VINTR] = interruptCh;

    if (-1 == tcsetattr(STDIN_FILENO, TCSAFLUSH, &ti)) {
        errnoExit("tcsetattr", errno);
    }
    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_25(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
#endif

    _2::test(argc, argv);
}
