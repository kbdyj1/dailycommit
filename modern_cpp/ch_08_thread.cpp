#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include <exception>
#include <mutex>
#include <algorithm>

#include "util.h"

using namespace std::chrono_literals;

namespace { //=================================================================

inline void printTime()
{
    auto now = std::chrono::system_clock::now();
    auto st = std::chrono::system_clock::to_time_t(now);
    auto* lt = std::localtime(&st);

    std::cout << std::put_time(lt, "%c") << "\n";
}

void func1()
{
    std::cout << "func1()" << std::endl;
}

void func2(int& mul2) {
    mul2 *= 2;
}

void test()
{
    std::thread t0(func1);
    std::thread t1([](){
        std::cout << "thread[](){}" << std::endl;
    });
    std::thread t2(printTime);

    auto n = 5;

    std::thread t3(func2, std::ref(n));

    t0.detach();
    t1.join();
    t2.join();
    t3.join();

    std::cout << "t3.join() end. n = " << n << "\n";
}

void fun3()
{
    using namespace std::chrono;

    printTime();
#if (0)
    std::this_thread::sleep_for(2s);
#else
    std::this_thread::sleep_until(system_clock::now() + 2s);
#endif
    printTime();
}

void test_sleep()
{
    auto t = std::thread(fun3);
    t.join();
}

void fun4(std::chrono::seconds secs)
{
    using namespace std::chrono;

    auto now = std::chrono::system_clock::now();
    auto then= now + 2s;

    std::cout << "before yield." << std::endl;

    auto count = 0;
    do {
        std::this_thread::yield();
        ++count;
    } while (std::chrono::system_clock::now() < then);

    std::cout << "after yield. count: " << count << "\n";
}

void test_yield()
{
    using namespace std::chrono;

    auto t = std::thread(fun4, 2s);
    t.join();
}

namespace thread_exceptions {

std::vector<std::exception_ptr> gExceptions;
std::mutex gMutex;

void func0()
{
    throw std::logic_error{"func0"};
}

void func1()
{
    throw std::logic_error{"func1"};
}

void thread_func0()
{
    try {
        func0();
    } catch (...) {
        std::lock_guard<std::mutex> lock(gMutex);
        gExceptions.push_back(std::current_exception());
    }
}

void thread_func1()
{
    try {
        func1();
    } catch (...) {
        std::lock_guard<std::mutex> lock(gMutex);
        gExceptions.push_back(std::current_exception());
    }
}

void test()
{
    auto t0 = std::thread{thread_func0};
    auto t1 = std::thread{thread_func1};
    t0.join();
    t1.join();

    for (const auto& e : gExceptions) {
        try {
            if (e != nullptr) {
                std::rethrow_exception(e);
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::this_thread::sleep_for(1s);
}

} //thread_exceptions -----------------------------------------------

} //namespace =================================================================

void test_ch_08_thread()
{
#if (0)
    test();
    test_sleep();
    test_yield();
#endif

    thread_exceptions::test();
}
