#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test(int argc, const char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s op shm-name ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char opt = argv[1][0];
    int flag = 0;
    size_t size = 0;
    bool copy = false;
    bool print = false;
    struct stat s;

    switch (opt) {
    case 'u':
        if (-1 == shm_unlink(argv[2])) {
            errorExit("shm_unlink() failed.\n");
        } else {
            printf("shm_unlink() done.\n");
        }
        exit(EXIT_SUCCESS);

    case 'c':
    case 'x':
        flag = O_CREAT | O_RDWR;
        if (opt == 'x')
            flag |= O_EXCL;
        size = argc < 4 ? 0 : atoi(argv[3]);
        break;

    case 'w':
        flag = O_RDWR;
        size = argc < 4 ? 0 : strlen(argv[3]);
        copy = true;
        break;

    case 'r':
        flag = O_RDONLY;
        print = true;
        break;

    default:
        break;
    }

    int fd = shm_open(argv[2], flag, S_IRUSR | S_IWUSR);
    if (-1 == fd) {
        errorExit("shm_open() failed.\n");
    }

    if (size && (-1 == ftruncate(fd, size))) {
        errorExit("ftruncate() failed.\n");
    }
    if (print) {
        struct stat s;
        fstat(fd, &s);
        size = s.st_size;
    }

    int prot = PROT_READ;
    if (!print)
        prot |= PROT_WRITE;

    void* addr = mmap(NULL, size, prot, MAP_SHARED, fd, 0);
    if (MAP_FAILED == addr) {
        errorExit("mmap() failed.\n");
    }

    if (copy) {
        memcpy(addr, argv[3], size);
    }
    if (print) {
        close(fd);

        write(STDOUT_FILENO, addr, size);
        write(STDOUT_FILENO, "\n", 1);
    }
    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_17(int argc, const char* argv[])
{
    _1::test(argc, argv);
}
