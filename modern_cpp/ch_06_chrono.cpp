#include <iostream>
#include <chrono>
#include <functional>

#include "util.h"

namespace { //=================================================================

using namespace std::chrono_literals;

void test_duration()
{
    auto h = 11h;
    auto m = 20min;
    auto s = 30s;
    auto ms= 500ms;
    auto us= 600us;
    auto ns= 700ns;

    // low precision -> high precision
    std::cout << "h(" << h.count() << "), m(" << m.count() << ")\n";

    auto d = std::chrono::duration<double, std::ratio<10>>{1.2};

    std::cout << "d.count(): " << d.count() << "secs: " << std::chrono::duration_cast<std::chrono::seconds>(d).count() << "\n";
}

void test_duration_cast()
{
    auto secs = 18130s;
    auto h = std::chrono::duration_cast<std::chrono::hours>(secs);
    auto m = std::chrono::duration_cast<std::chrono::minutes>(secs % 1h);
    auto s = std::chrono::duration_cast<std::chrono::seconds>(secs % 1min);

    std::cout << "secs(" << secs.count() << "): " << h.count() << ":" << m.count() << ":" << s.count() << "\n";
}

void test_chrono_arithmetic()
{
    auto t0 = 1h + 20min + 45s;
    auto t1 = 3h + 12min + 50s;

    std::cout << std::boolalpha << "t0 < t1 ? " << (t0 < t1) << "\n";
}

void func(const int count = 100000000)
{
    for (int i=0; i<count; i++)
        ;
}

void test_measurement()
{
#if (0)
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto diff = std::chrono::high_resolution_clock::now() - start;
#else
    auto diff = PerfTimer<>::duration(func, 200000000);
#endif

    std::cout << std::chrono::duration<double, std::milli>(diff).count() << "ms\n";
}

template <typename T>
void printClockPrecision()
{
    std::cout << "precision: " << (1000000.0 * double(T::period::num)) / (T::period::den);
    std::cout << ", steady? " << T::is_steady << "\n";
}
void test_precision()
{
    std::cout << "system_clock ";
    printClockPrecision<std::chrono::system_clock>();
    std::cout << "high_resolution_clock ";
    printClockPrecision<std::chrono::high_resolution_clock>();
    std::cout << "steady_clock ";
    printClockPrecision<std::chrono::steady_clock>();
}

} //namespace =================================================================

void test_ch_06_chrono()
{
#if (0) // done
    test_duration_cast();
    test_duration();
    test_chrono_arithmetic();
    test_measurement();
#endif

    test_precision();
}
