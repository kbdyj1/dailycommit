#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "utils.h"

namespace { //=================================================================

void test_process_id()
{
    auto ppid = getppid();
    std::cout << "ppid: " << ppid << "\n";

    auto pid = getpid();
    std::cout << "pid: " << pid << "\n";

    catFile("/proc/sys/kernel/pid_max");    //32768
}

} //===========================================================================

void test_ch_6()
{
    test_process_id();
}
