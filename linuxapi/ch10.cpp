#include <iostream>
#include <sys/time.h>

namespace { //=================================================================

void test_get_time_of_day()
{
    timeval tv;

    auto ret = gettimeofday(&tv, nullptr);
    if (0 == ret) {
        std::cout << "tv_sec(UTC): " << tv.tv_sec << ", " << "tv_usec: " << tv.tv_usec << "\n";
    }

    auto t = time(nullptr);
    std::cout << "time(): " << t << "\n";
    std::cout << "ctime(" << t << "): " << ctime(&t) << "\n";
}

void print_tm(tm* t)
{
#if (0)
    std::cout << "tm.\n"
              << "\tzone: " << t->tm_zone << "\n"
              << "\tyear: " << t->tm_year << "(~1900)\n"
              << "\tmonth: " << t->tm_mon << "(0~11)\n"
              << "\tmday: " << t->tm_mday << "\n"
              << "\tyday: " << t->tm_yday << "\n"
              << "\thour: " << t->tm_hour << "\n"
              << "\tmin: " << t->tm_min << "\n"
              << "\tsec: " << t->tm_sec << "\n"
              << "\tisdst: " << t->tm_isdst << "\n"
              << "\tgmtoff: " << t->tm_gmtoff << "(" << (t->tm_gmtoff/3600) << ")\n";
#else
    std::cout << asctime(t);
#endif
}

void test_broken_down_time()
{
    auto t = time(nullptr);
    auto* tm = gmtime(&t);

    std::cout << "gmtime(" << t << "):\n ";
    print_tm(tm);

    tm = localtime(&t);
    std::cout << "localtime(" << t << "):\n ";
    print_tm(tm);
}

#define BUF_SIZE    64

void test_strftime()
{
    char buf[BUF_SIZE + 1];
    auto t = time(nullptr);
    auto* tm = localtime(&t);
    auto len = strftime(buf, BUF_SIZE, "%c", tm);   // %x : 06/20/22
                                                    // %c : Mon Jun 20 22:33:14 2022

    if (0 < len) {
        std::cout << buf << "\n";
    }
}

} //===========================================================================

void test_ch_10()
{
#if (0)
    putenv("TZ=:Asia/Hong_Kong");
#endif
    test_strftime();
}
