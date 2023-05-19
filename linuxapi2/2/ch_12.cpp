#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

//#define TEST_SIGSEGV

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

    close(fd);

    if (sb.st_size != write(STDOUT_FILENO, addr, sb.st_size)) {
        errorExit("write() error.\n");
    }

    munmap(addr, sb.st_size);


#ifdef TEST_SIGSEGV
    char c = *addr;
#endif

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDWR);
    if (-1 == fd) {
        errorExit("open() failed.\n");
    }

    const int MemSize = 10;
    char* addr = (char*) mmap(NULL, MemSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        errorExit("mmap() MAP_FAILED.\n");
    }

    if (-1 == close(fd)) {
        errorExit("close() error.\n");
    }

    printf("string: %.*s\n", MemSize, addr);

    if (2 < argc) { // update
        size_t len = strlen(argv[2]);
        if (MemSize <= len) {
            fprintf(stderr, "'new value(%ld)' is too large.\n", len);
            exit(EXIT_FAILURE);
        }
        memset(addr, 0, MemSize);
        strncpy(addr, argv[2], MemSize - 1);

        if (-1 == msync(addr, MemSize, MS_SYNC)) {
            errorExit("msync() error.\n");
        }

        printf("copied: \"%s\" to shared memory.\n", argv[2]);
    }

    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_12(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
#endif

    _2::test(argc, argv);
}
