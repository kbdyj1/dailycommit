#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/utsname.h>

#define PRINT_TN(attr) \
    std::cout << "\t" << #attr << ": " << attr << "\n"

namespace { //=================================================================

const int MAX_LINE = 100;

void test_pid_max()
{
    const char* pidmax = "/proc/sys/kernel/pid_max";
    auto fd = open(pidmax, O_RDONLY);
    if (fd < 0) {
        std::cerr << "open failed. " << pidmax << "\n";
        return;
    }

    char buf[MAX_LINE];
    auto n = read(fd, buf, MAX_LINE);
    if (n == -1) {
        std::cerr << "read failed. " << pidmax << "\n";
        close(fd);
        return;
    }

    std::cout << n << " = read(" << pidmax << "): " << buf << "\n";
    close(fd);
}

/*
uname()
    name.domainname: localdomain
    name.sysname: Linux
    name.nodename: DESKTOP-UP2UQ5A
    name.release: 4.19.128-microsoft-standard
    name.version: #1 SMP Tue Jun 23 12:58:10 UTC 2020
    name.machine: x86_64
*/
void test_uname()
{
    auto name = utsname{};
    auto ret = uname(&name);
    std::cout << "uname()\n";
    PRINT_TN(name.domainname);
    PRINT_TN(name.sysname);     // /sys/kernel/ostype
    PRINT_TN(name.nodename);    // /sys/kernel/hostname
    PRINT_TN(name.release);     // /sys/kernel/osrelease
    PRINT_TN(name.version);
    PRINT_TN(name.machine);     // ???
}

} //===========================================================================

void test_ch_12()
{
    //test_pid_max();
    test_uname();
}
