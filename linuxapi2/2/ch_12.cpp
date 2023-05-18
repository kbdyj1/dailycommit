#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (-1 == fd) {
        errorExit("open() error.\n");
    }
    struct stat sb;
    if (-1 == fstat(fd, &sb)) {
        errorExit("fstat() error.\n");
    }
    char* addr = (char*)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == addr) {
        errorExit("mmap() error.\n");
    }

    if (sb.st_size != write(STDOUT_FILENO, addr, sb.st_size)) {
        errorExit("write() error.\n");
    }

    close(fd);

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_12(int argc, const char* argv[])
{
    _1::test(argc, argv);
}
