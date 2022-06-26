#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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

} //===========================================================================

void test_ch_13()
{
    test_stdio_streamming_buffering_mode();
    test_synchronized_io_completion();
}
