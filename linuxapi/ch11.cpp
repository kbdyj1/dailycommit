#include <iostream>
#include <limits.h>
#include "utils.h"

namespace { //=================================================================

// SUSv3
// 1. runtime invariant value
// 2. pathname variable value
// 3. runtime incresable value

void test_runtime_invariant()
{
    PRINT_DEF(MQ_PRIO_MAX);
    PRINT_DEF(_POSIX_MQ_PRIO_MAX);
    PRINT_SYSCONF(_SC_MQ_PRIO_MAX);
}

inline void printPathConf(const char* path, int name=_PC_NAME_MAX)
{
    auto limit = pathconf(path, name);
    switch (name) {
    case _PC_NAME_MAX:
        std::cout << "pathconf(" << path << ", _PC_NAME_MAX): " << limit << "\n";
        break;
    case _PC_PATH_MAX:
        std::cout << "pathconf(" << path << ", _PC_PATH_MAX): " << limit << "\n";
        break;
    case _PC_PIPE_BUF:
        std::cout << "pathconf(" << path << ", _PC_PIPE_BUF): " << limit << "\n";
        break;
    }
}

void test_pathname_variable()
{
    printPathConf("/");
    printPathConf("/etc");
}

void test_runtime_increasable()
{
    PRINT_SYSCONF(_SC_ARG_MAX);
    PRINT_SYSCONF(_SC_CLK_TCK);
    PRINT_SYSCONF(_SC_LOGIN_NAME_MAX);
    PRINT_SYSCONF(_SC_OPEN_MAX);
    PRINT_SYSCONF(_SC_NGROUPS_MAX);
    PRINT_SYSCONF(_SC_PAGESIZE);
    PRINT_SYSCONF(_SC_RTSIG_MAX);
    PRINT_SYSCONF(_SC_SIGQUEUE_MAX);
    PRINT_SYSCONF(_SC_STREAM_MAX);  // FOPEN_MAX

    printPathConf("/boot", _PC_NAME_MAX);
    printPathConf("/", _PC_PATH_MAX);
    printPathConf("/", _PC_PIPE_BUF);
}

void test_option()
{
    PRINT_SYSCONF(_SC_ASYNCHRONOUS_IO);
    PRINT_SYSCONF(_SC_JOB_CONTROL);
    PRINT_SYSCONF(_SC_MESSAGE_PASSING);
    PRINT_SYSCONF(_SC_PRIORITY_SCHEDULING);
    PRINT_SYSCONF(_SC_REALTIME_SIGNALS);
    PRINT_SYSCONF(_SC_SEMAPHORES);
    PRINT_SYSCONF(_SC_SHARED_MEMORY_OBJECTS);
    PRINT_SYSCONF(_SC_THREADS);
    PRINT_SYSCONF(_SC_XOPEN_UNIX);
}

} //===========================================================================

void test_ch_11()
{
    test_runtime_invariant();
    test_pathname_variable();
    test_runtime_increasable();

    std::cout << "\noptions..." << "\n";
    test_option();
}
