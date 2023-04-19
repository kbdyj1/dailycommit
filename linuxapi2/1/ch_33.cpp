#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void writeToDummyFile(const char* filename)
{
    auto fp = fopen(filename, "w");
    if (fp) {
        fprintf(fp, "Hello, Qt\n");
        fclose(fp);
    } else {
        fprintf(stderr, "fopen error!\n");
    }
}

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s filename [r]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("user id: %d\n", getuid());
    printf("effective user id: %d\n", geteuid());

    const char* filename = argv[1];
    auto removePrivilege = (argc < 3) ? false : argv[2][0] == 'r';

    auto euid = geteuid();

    if (removePrivilege) {
        if (-1 == seteuid(getuid()))
            errorExit("setuid(getuid()) error.\n");
        else
            printf("remove privilege\n");

        // test to fail
        writeToDummyFile(filename);

        // reacquiring privileges
        if (-1 == seteuid(euid)) {
            fprintf(stderr, "seteuid() error. reason: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    writeToDummyFile(filename);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_33(int argc, const char* argv[])
{
    _1::test(argc, argv);
}
