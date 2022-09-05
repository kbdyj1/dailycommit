#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>

#include "util.h"

namespace { //=================================================================

using namespace std::chrono_literals;

namespace mutex_lock {

std::mutex gMutex;

template <typename T>
struct Container {
    std::mutex mutex;
    std::vector<T> data;
};

#define SCOPED_LOCK

#ifndef SCOPED_LOCK
//#define DEAD_LOCK
#endif

template <typename T>
void moveBetween(Container<T>& c0, Container<T>& c1, const T value) {
#if defined(SCOPED_LOCK)
    auto lock = std::scoped_lock(c0.mutex, c1.mutex);
#else
    std::lock(c0.mutex, c1.mutex);

#   if defined(DEAD_LOCK)
    auto l0 = std::lock_guard<std::mutex>(c0.mutex);    // dead lock
    auto l1 = std::lock_guard<std::mutex>(c1.mutex);
#   else
    auto l0 = std::lock_guard<std::mutex>(c0.mutex, std::adopt_lock);
    auto l1 = std::lock_guard<std::mutex>(c1.mutex, std::adopt_lock);
#   endif //DEAD_LOCK

#endif //SCOPED_LOCK

    c0.data.erase(std::remove(c0.data.begin(), c0.data.end(), value), c0.data.end());
    c1.data.push_back(value);
}

void thread_func()
{
    {
        auto lock = std::lock_guard<std::mutex>(gMutex);
        std::cout << "running thread: " << std::this_thread::get_id() << std::endl;
    }

    std::this_thread::yield();
    std::this_thread::sleep_for(2s);

    {
        auto lock = std::lock_guard<std::mutex>(gMutex);
        std::cout << "done in thread: " << std::this_thread::get_id() << std::endl;
    }
}

void test()
{
#if (0)
    auto threads = std::vector<std::thread>{};
    for (auto i=0; i<5; i++) {
        threads.emplace_back(thread_func);
    }
    for (auto& t : threads) {
        t.join();
    }
#else
    auto c0 = Container<int>{};
    c0.data.assign({1, 2, 3});

    auto c1 = Container<int>{};
    c1.data.assign({4, 5, 6});

    auto t0 = std::thread(moveBetween<int>, std::ref(c0), std::ref(c1), 3);
    auto t1 = std::thread(moveBetween<int>, std::ref(c1), std::ref(c0), 6);

    t0.join();
    t1.join();

    std::cout << "c0: "; print(c0.data);
    std::cout << "c1: "; print(c1.data);

    std::this_thread::sleep_for(1s);
#endif
}

} //mutex_lock ------------------------------------------------------

//#define USE_RECURSIVE_MUTEX

class Rec {
    int data;

public:
    Rec(const int d = 0) : data(d)
    {}

#if defined(USE_RECURSIVE_MUTEX)
private:
    std::recursive_mutex m;

public:
    void update(const int d) {
        auto lock = std::lock_guard<std::recursive_mutex>(m);
        data = d;
    }

    int updateWithReturn(const int d) {
        auto lock = std::lock_guard<std::recursive_mutex>(m);
        auto temp = data;
        update(d);
        return temp;
    }
#else
private:
    std::mutex m;

public:
    void update(const int d) {
        auto lock = std::lock_guard<std::mutex>(m);
        internalUpdate(d);
    }

    void internalUpdate(const int d) {
        data = d;
    }

    int updateWithReturn(const int d) {
        auto lock = std::lock_guard<std::mutex>(m);
        auto temp = data;
        internalUpdate(d);
        return temp;
    }
#endif
};

} //namespace =================================================================

void test_ch_08_lock()
{
#if (0) // done
    mutex_lock::test();
#endif
}
