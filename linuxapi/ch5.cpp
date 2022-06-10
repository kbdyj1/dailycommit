#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

#include "utils.h"

namespace { //=================================================================

// file I/O
//  O_EXCL, O_APPEND -> Atomic

void bad_exclusive_open(const std::string& s)
{
    auto fd = open(s.c_str(), O_RDWR | O_EXCL);
    if (-1 != fd) {
        std::cout << "file [" << getpid() << "] : " << s << " already exist.\n";

        auto flags = fcntl(fd, F_GETFL);
        if (-1 != flags) {
            if (flags & O_SYNC) {
                std::cout << "writes are synchronized.\n";
            }

            auto accessMode = flags & O_ACCMODE;
            if (accessMode == O_WRONLY || accessMode == O_RDWR) {
                std::cout << "file is writable.\n";
            }
        } else {
            std::cerr << "flags : " << flags << ".\n";
        }

        close(fd);
    } else {
        if (errno != ENOENT) {
            printOpenFail(s);
        } else {
            fd = open(s.c_str(), O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);
            if (-1 != fd) {
                std::cout << "file [" << getpid() << "] : " << s << " created exclusively.\n";
                close(fd);
            } else {
                std::cerr << "open error\n";
            }
        }
    }
}

void test_dup()
{
    std::string filename = "my.txt";
    auto fd = open(filename.c_str(), O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        printOpenFail(filename);
    } else {
        auto fd2 = dup(fd);

        std::cout << "fd: " << fd << ", fd2: " << fd2 << "\n";

        std::string message0 = "Hello, ";
        write(fd, message0.c_str(), message0.size());

        std::string message1 = "Qt6";
        write(fd2, message1.c_str(), message1.size());

        close(fd2);
    }
}

void scatter_gather_io()
{
    const char* filename = "sg.txt";
    auto maxIovCnt = sysconf(_SC_IOV_MAX);

    std::cout << "max iov: " << maxIovCnt << "\n";  //1024

    //-------------------------------------------------------------------------
    //  Gather
    //-------------------------------------------------------------------------

    std::string s0 = "Hello, ";
    std::string s1 = "Qt 6 !!!";
    std::string s2 = "\nLinux API";

    iovec vlist[3] = {{(void*)s0.c_str(), s0.size()}, {(void*)s1.c_str(), s1.size()}, {(void*)s2.c_str(), s2.size()}};

    auto fd = open(filename, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    auto written = writev(fd, vlist, 3);

    std::cout << "written: " << written << "\n";

    close(fd);

    //-------------------------------------------------------------------------
    //  Scatter
    //-------------------------------------------------------------------------

    fd = open(filename, O_RDONLY);

    char buf0[8] = {0,};
    char buf1[9] = {0,};
    char buf2[11] = {0,};

    iovec gatter[3] = {{(void*)buf0, 8-1}, {(void*)buf1, 9-1}, {(void*)buf2, 11-1}};
    auto readed = readv(fd, gatter, 3);
    std::cout << "gattered: " << buf0 << buf1 << buf2 << "\n";

    //-------------------------------------------------------------------------
    //  Truncate
    //      the only system call that allows changing the contents of a file
    //      without openning the file.
    //-------------------------------------------------------------------------

    fd = open(filename, O_RDWR);
    ftruncate(fd, 18);
    close(fd);
}

void test_lfs()
{
    // 1. use LFS api (transitional extension), #define _LARGEFILE64_SOURCE
    //      fopen64(), open64(), lseek64(), truncate64(), stat64(), mmap64(), setrlimit64()
    // 2. #define _FILE_OFFSET_BITS 64

    const char* filename = "largefile";
    auto fd = open64(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printOpenFail(filename);
    } else {
        printOpenSucceeded(filename);
        close(fd);
    }
}

void test_mkstemp()
{
    char filename[] = "myXXXXXX";
    auto fd = mkstemp(filename);
    if (fd == -1) {
        printOpenFail(filename);
    } else {
        printOpenSucceeded(filename);   //open succeeded. myI6UmQI

        std::string message = "Hello, Qt6 !!!";
        write(fd, message.c_str(), message.size());

        unlink(filename);
        close(fd);
    }
}

} //===========================================================================

void test_ch_5()
{
    //bad_exclusive_open("/bin/gcc");

    //test_dup(); // > results.log 2>&1
    //scatter_gather_io();
    //test_lfs();
    test_mkstemp();
}
