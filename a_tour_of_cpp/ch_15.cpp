//=============================================================================
//  A tour of C++
//  Ch.15  Concurrency
//
//  22/09/18
//=============================================================================

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <numeric>
#include <future>
#include <vector>

using namespace std::chrono_literals;

namespace { //=================================================================

void f() {
    std::cout << "void f()\n";
}
struct F {
    void operator()(){
        std::cout << "F::operator()\n";
    }
};

void test_thread()
{
    auto t0 = std::thread(f);
    auto t1 = std::thread(F());

    t0.join();
    t1.join();
}

std::shared_mutex mutex;
int value = 0;

void reader() {
    auto lock = std::shared_lock{mutex};

    std::cout << "thread(" << std::this_thread::get_id() << "): " << value << "\n";
}

void writer() {
    auto lock = std::unique_lock{mutex};

    value = 10;
}

void test_shared_mutex()
{
    auto t0 = std::thread{reader};
    auto t1 = std::thread{reader};
    auto t2 = std::thread{writer};
    auto t3 = std::thread{reader};
    auto t4 = std::thread{reader};

    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

double accumulate(double* b, double* e, double init)
{
    return std::accumulate(b, e, init);
}

void test_packaged_task()
{
    using TaskType = double(double*, double*, double);

    auto pt0 = std::packaged_task<TaskType>{accumulate};
    auto pt1 = std::packaged_task<TaskType>{accumulate};

    auto f0 = std::future<double>{pt0.get_future()};
    auto f1 = std::future<double>{pt1.get_future()};

    auto v = std::vector<double>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto* p = &v[0];

    auto t0 = std::thread{ std::move(pt0), p, p+v.size()/2, 0.0};
    auto t1 = std::thread{ std::move(pt1), p+v.size()/2, p+v.size(), 0.0};

    t0.join();
    t1.join();

    auto sum0 = f0.get();
    auto sum1 = f1.get();
    auto sum = sum0 + sum1;

    std::cout << "sum: " << sum << ", (" << sum0 << " + " << sum1 << ")\n";
}

} //namespace =================================================================

void test_ch_15()
{
#if (0) // done
    test_thread();
    test_shared_mutex();
#endif

    test_packaged_task();

    std::this_thread::sleep_for(100ms);
}
