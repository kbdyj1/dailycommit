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

namespace _3 {

void test()
{
    const char* filename = "echo";
    execlp(filename, filename, "Hello, Qt", (char *)NULL);

    errorExit("execlp() error.\n");
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test()
{
    char* env[] = { "GREET=salut", "BYE=adieu", NULL };
    const char* pathname = "/usr/bin/echo";
    char* filename = "echo";

    execle(pathname, filename, "Hello, Qt", (char*)NULL, env);

    errorExit("execlp() error.\n");
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_27()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
