#include <stdio.h>
#include <unistd.h>
#include "utils.h"

extern char** environ;

namespace { //=================================================================

namespace _1 {

#define CPPPATH "/usr/bin/g++"

void test()
{
    const char* pathname = CPPPATH;
    char* env[] = {
        NULL
    };

    char* args[] = {
        CPPPATH,
        "--version",
        NULL
    };

    execve(pathname, args, env);
    errorExit("execve() error.\n");
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    char** pp;

    for (pp=environ; *pp != NULL; pp++) {
        printf("environment %s\n", *pp);
    }
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_27()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
