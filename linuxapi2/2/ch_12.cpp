//#define USE_MAP_ANON

#ifdef  USE_MAP_ANON
#   define _BSD_SOURCE
#endif

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
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

namespace _3 {

void test()
{
    int fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        errorExit("open(/dev/zero) error.\n");
    } else {
        fprintf(stdout, "/dev/zero -> %d\n", fd);
    }

    void* addr = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        errorExit("mmap() error.\n");
    }

    close(fd);
    exit(EXIT_SUCCESS);
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test()
{
    int* addr;

#ifdef USE_MAP_ANON
    addr = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        errorExit("mmap() failed.\n");
#else
    int fd = open("/dev/zero", O_RDWR);
    if (-1 == fd)
        errorExit("open(/dev/zero) failed.\n");

    addr = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        errorExit("mmap() failed.\n");

    if (-1 == close(fd))
        errorExit("close(fd) error.\n");
#endif

    *addr = 1;

    switch (fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        printf("Child, value: %d\n", *addr);
        (*addr)++;

        if (-1 == munmap(addr, sizeof(int)))
            errorExit("munmap() failed.\n");
        _exit(EXIT_SUCCESS);
        break;

    default:
        if (-1 == wait(NULL))
            errorExit("wait() error.\n");
        printf("Parent, value: %d\n", *addr);
        if (-1 == munmap(addr, sizeof(int)))
            errorExit("munmap() error.\n");
        exit(EXIT_SUCCESS);
        break;
    }
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test()
{
    int fd = open("/dev/zero", O_RDWR);
    void* addr = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE, fd, 0);
    printf("  mmap(): %p\n", addr);
    close(fd);

    addr = mremap(addr, 4096, 8196, MREMAP_MAYMOVE);
    printf("mremap(): %p\n", addr);
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_12(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
    _2::test(argc, argv);
    _3::test();
    _4::test();
#endif

    _5::test();
}
