#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

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
}

void test()
{
    int mc = pathconf("/", _PC_MAX_CANON);
    printf("_PC_MAX_CANON: %d\n", mc);

    int mi = pathconf("/", _PC_MAX_INPUT);
    printf("_PC_MAX_INPUT: %d\n", mi);

    print_termios("STDIN_FILENO", STDIN_FILENO);
    print_termios("STDOUT_FILENO", STDOUT_FILENO);
    print_termios("STDERR_FILENO", STDERR_FILENO);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_25(int argc, const char* argv[])
{
    _1::test();
}
