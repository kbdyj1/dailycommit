//=============================================================================
//  ch.17 POSIX shared memory
//
//  tmpfs(memory?) default size : 256M
//  create  : permission RW
//  read    : permission R
//=============================================================================

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <unistd.h>
#include <memory.h>

namespace { //=================================================================

namespace _1 {

const char* shmKey = "linux api2 shared memory test2";

void test_create()
{
    int flags = O_RDWR;

    flags |= O_CREAT;
//    flags |= O_EXCL;

    size_t shmSize = 64;
    mode_t shmPerms = S_IWUSR | S_IRUSR;

    int fd = shm_open(shmKey, flags, shmPerms);
    if (-1 == fd) {
        std::cerr << "shm_open failed.\n";
    } else {
        std::cout << "shm_open OK.\n";

        if (-1 == ftruncate(fd, shmSize)) {
            std::cerr << "ftruncate error. size: " << shmSize << "\n";
        } else {
            std::cout << "ftruncate(" << shmSize << ") OK.\n";

            void* addr = mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if (addr == MAP_FAILED) {
                std::cerr << "mmap failed.\n";
            } else {
                char buf[64] = "Hello, Linux API2.";
                memset(addr, 0, shmSize);
                memcpy(addr, buf, strlen(buf));

                std::cout << "mmap OK.\n";
            }
        }
        if (-1 == close(fd)) {
            std::cerr << "close failed.\n";
        }
    }
}

void test_read()
{
    int flags = O_RDONLY;

    int fd = shm_open(shmKey, flags, S_IRUSR);
    if (-1 == fd) {
        std::cerr << "shm_open failed.\n";
    } else {
        std::cout << "shm_open OK.\n";

        struct stat sb;

        fstat(fd, &sb);

        std::cout << "sb.st_size: " << sb.st_size << "\n";

        void* addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED) {
            std::cerr << "mmap failed.\n";
        } else {
            std::cout << "mmap OK." << std::endl;

            int len = strlen((const char*)addr);
            write(STDOUT_FILENO, addr, len);

            std::cout << std::endl;
        }

        if (-1 == close(fd)) {
            std::cerr << "close failed.\n";
        }
    }
}

void test_delete()
{
    int result = shm_unlink(shmKey);

    std::cout << "shm_unlink(" << shmKey << ") : " << (result != -1) << "\n";
}

void test()
{
    int cmd = 2;

    switch (cmd) {
    case 0: // create
        test_create();
        break;

    case 1: // read
        test_read();
        break;

    case 2: // delete
        test_delete();
        break;
    }
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_17()
{
    _1::test();
}
