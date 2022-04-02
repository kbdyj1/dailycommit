#include <thread>
#include <iostream>
#include <future>
#include <algorithm>
#include <numeric>
#include <vector>

namespace { //=================================================================

int asyncJob() noexcept
{
    std::cout << "asyncJob()" << std::endl;

    return 100;
}

void test0()
{
    std::thread t(asyncJob);

    t.join();
}

void test1()
{
    auto fut  = std::async(std::launch::async, asyncJob);

    std::cout << fut.get() << std::endl;
}

std::mutex m;
struct A
{
    void a(int i, const std::string& s) {
        auto lock = std::lock_guard<std::mutex>{m};
        std::cout << "A::a(" << i << ", " << s << ")" << std::endl;
    }
    void b(const std::string& s) {
        auto lock = std::lock_guard<std::mutex>{m};
        std::cout << "A::b(" << s << ")" << std::endl;
    }
    int operator()(int i) {
        auto lock = std::lock_guard<std::mutex>{m};
        std::cout << "A() : " << i << std::endl;
        return i + 10;
    }
};

template <typename RandomIter>
int parallelSum(RandomIter begin, RandomIter end)
{
    auto len = end - begin;
    if (len < 1000)
        return std::accumulate(begin, end, 0);

    RandomIter mid = begin + len/2;
    auto handle = std::async(std::launch::async, parallelSum<RandomIter>, mid, end);
    auto sum = parallelSum(begin, mid);

    return sum + handle.get();
}

void test2()
{
    auto v = std::vector<int>(10000, 1);
    std::cout << "The sum is " << parallelSum(v.begin(), v.end()) << std::endl;

    A a;

    auto a0 = std::async(&A::a, &a, 42, "Hello,");

    auto a1 = std::async(std::launch::deferred, &A::b, a, "Qt6");

    auto a2 = std::async(std::launch::async, A(), 43);
    a1.wait();

    std::cout << a2.get() << std::endl;
}

} // namespace ================================================================

void test_concurrency()
{
    std::cout << std::endl;

    //test0();
    //test1();
    test2();
}
