#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

namespace { //=================================================================

// file I/O
//  O_EXCL, O_APPEND -> Atomic

void bad_exclusive_open(const std::string& s)
{
    auto fd = open(s.c_str(), O_RDONLY | O_EXCL);
    if (-1 != fd) {
        std::cout << "file [" << getpid() << "] : " << s << " already exist.\n";
        close(fd);
    } else {
        if (errno != ENOENT) {
            std::cerr << "open error\n";
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

} //===========================================================================

void test_ch_5()
{
    bad_exclusive_open("/bin/gcc");
}
