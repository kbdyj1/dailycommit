#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

namespace { //=================================================================

// file I/O
//  O_EXCL, O_APPEND -> Atomic

void printOpenFail(const std::string& filename)
{
    std::cout << "open fail. " << filename << "\n";
}

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

} //===========================================================================

void test_ch_5()
{
    //bad_exclusive_open("/bin/gcc");

    test_dup(); // > results.log 2>&1
}
