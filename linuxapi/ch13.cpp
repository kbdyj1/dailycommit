#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

namespace { //=================================================================

#define BUF_SIZE    1024

char buffer[BUF_SIZE] = {0, };

void print_buffer_element(size_t size)
{
    for (size_t i=0; i<size; i++) {
        std::cout << "buffer[" << i << "] : " << buffer[i] << "\n";
    }
}

void test_stdio_streamming_buffering_mode()
{
    // int setvbuf(FILE* stream, char* buf, int mode, size_t size)
    // mode
    //  _IONBF : stderr (buf:null, size:0)
    //  _IOLBF : line buffer I/O (terminal device)
    //  _IOFBF : disk file stream

#if (1)
    if (0 != setvbuf(stdout, buffer, _IOFBF, BUF_SIZE)) {
        std::cerr << "setvbuf(stdout, ..., _IOFBF, " << BUF_SIZE << ") failed\n";
    } else {
        printf("setvbuf(stdout, buffer, _IOFBF, %d)\n", BUF_SIZE);

        print_buffer_element(64);
    }
#else
    // void setbuf(FILE* stream, char* buf)
    //  buf == nullptr ? _IONBF & size == 0
    //  buf != nullptr ? _IOFBF & size == 8192
    //
    // void setbuffer(FILE* stream, char* buf, size_t size)
#endif
}

void test_synchronized_io_completion()
{
    // 1. synchronized I/O data integrity completion (fdatasync)
    // 2. synchronized I/O file integrity completion (fsync)

#if (0)
    // all kernel buffer flushed to disk
    sync();
#endif

    auto fd = open("test.txt", O_CREAT | O_WRONLY | O_SYNC);    // O_SYNC : always sync
    if (fd != -1) {
        std::cout << "Hello, Qt6 !!!";
        write(fd, buffer, 10);
        close(fd);
    } else {
        std::cerr << strerror(errno) << "\n";
    }
}

void test_posix_advise()
{
    // 1. POSIX_FADV_NORMAL     read ahead window size: 128K
    // 2. POSIX_FADV_SEQUENTIAL read ahead window size: 256K
    // 3. POSIX_FADV_RANDOM     read ahead -> disabled
    // 4. POSIX_FADV_WILLNEED
    // 5. POSIX_FADV_DONTNEED
    // 6. POSIX_FADV_NOREUSE    not valid
}

void test_direct_io(const char*filename, size_t len, off_t offset, size_t align)
{
    auto fd = open(filename, O_RDONLY | O_DIRECT);
    if (0 <= fd) {
        void* buf = (char*)memalign(align*2, len + align) + align;

        if (buf) {
            std::cout << "memalign(" << align*2 << ", " << len+align << ") + " << align << " : " << buf << "\n";

            if (-1 != lseek(fd, offset, SEEK_SET)) {
                auto numRead = read(fd, buf, len);

                std::cout << numRead << " bytes <- read(" << fd << ", " << buf << ", " << len << ") : \n" << (char*)buf <<  "\n";
            } else {
                std::cerr << "lseek(" << fd << ", " << offset << ", SEEK_SET) : failed.\n";
            }

            //free(buf);
        }
        close(fd);
    }
}

void test_io()
{
    char buf[32] = "Linux api";
    printf("\nHello, Qt6");
    write(STDOUT_FILENO, buf, strlen(buf));
}

} //===========================================================================

void test_ch_13()
{
    //test_stdio_streamming_buffering_mode();
    //test_synchronized_io_completion();

#if (0)
    size_t len = 512 * 4;
    off_t offset = 512;
    size_t align = 512;

    test_direct_io("Makefile", len, offset, align);
#endif

    test_io();
}
