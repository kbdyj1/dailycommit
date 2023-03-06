#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace { //=================================================================

namespace _1 {

void handler()
{
    printf("exit!!!\n");
}
void handler2()
{
    printf("exit 2!!!\n");
}

void test()
{
    printf("_SC_ATEXIT_MAX: %ld\n", sysconf(_SC_ATEXIT_MAX));
    atexit(handler);
    atexit(handler2);
}

} //_1 --------------------------------------------------------------

namespace _2 { //on_exit. non standard

void handler(int status, void* arg)
{
    printf("handler(status:%d, arg)\n", status);
}

void test()
{
    on_exit(handler, NULL);

    exit(1);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    printf("test() begin.\n");  // -> user memory space

    write(STDOUT_FILENO, "ABCD\n", 5);  // -> kernel buffer

    if (-1 == fork()) {
        fprintf(stderr, "fork() error.\n");
        exit(-1);
    }

    exit(EXIT_SUCCESS);
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_25()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
