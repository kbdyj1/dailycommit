#include <iostream>
#include <thread>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <numeric>
#include <assert.h>

#include "util.h"

namespace { //=================================================================

unsigned int getThreadCount()
{
    return std::thread::hardware_concurrency();
}

template <typename Iter, typename F>
void parallelMap(Iter begin, Iter end, F func)
{
    auto size = std::distance(begin, end);
    if (size <= 10000) {
        std::transform(begin, end, begin, std::forward<F>(func));
    } else {
        auto threadCount = getThreadCount();
        auto part = size / threadCount;
        auto last = begin;

        auto threads = std::vector<std::thread>{};
        for (auto i=0U; i<threadCount; i++) {
            if (i == threadCount - 1) {
                last = end;
            } else {
                std::advance(last, part);
            }

            threads.emplace_back([=,&func]{
                std::transform(begin, last, begin, std::forward<F>(func));
            });
            begin = last;
        }
        for (auto& t : threads)
            t.join();
    }
}

template <typename Iter, typename R, typename F>
auto parallelReduce(Iter begin, Iter end, R init, F op)
{
    auto size = std::distance(begin, end);

    if (size <= 10000) {
        return std::accumulate(begin, end, init);
    } else {
        auto threadCount = getThreadCount();
        auto last = begin;
        auto part = size / threadCount;
        auto threads = std::vector<std::thread>{};
        auto values = std::vector<R>(threadCount);

        for (auto i=0U; i<threadCount; i++) {
            if (i == threadCount - 1) {
                last = end;
            } else {
                std::advance(last, part);
            }
            threads.emplace_back([=,&op](R& result){
                result = std::accumulate(begin, last, R{}, std::forward<F>(op));
            }, std::ref(values[i]));

            begin = last;
        }

        for (auto& t : threads)
            t.join();

        auto sum = std::accumulate(std::begin(values), std::end(values), init, std::forward<F>(op));

        return sum;
    }
}

void test_map_reduce()
{
    auto sizes = std::vector<int>{ 10000, 100000, 500000, 1000000, 2000000, 5000000, 10000000, 25000000, 50000000 };
    std::cout
            << std::right << std::setw(8) << std::setfill(' ') << "size"
            << std::right << std::setw(8) << "s map"
            << std::right << std::setw(8) << "p map"
            << std::right << std::setw(8) << "s fold"
            << std::right << std::setw(8) << "p fold"
            << std::endl;

    for (auto size : sizes) {
        auto v = std::vector<int>(size);
        std::iota(std::begin(v), std::end(v), 1);

        auto v0 = v;
        auto s0 = 0LL;

        auto tsm = PerfTimer<>::duration([&]{
            std::transform(std::begin(v0), std::end(v0), std::begin(v0), [](const int i){ return i+i; });
        });
        auto tsf = PerfTimer<>::duration([&]{
            s0 = std::accumulate(std::begin(v0), std::end(v0), 0LL, std::plus<>());
        });

        auto v1 = v;
        auto s1 = 0LL;

        auto tpm = PerfTimer<>::duration([&]{
            parallelMap(std::begin(v1), std::end(v1), [](const int i){ return i+i; });
        });
        auto tpf = PerfTimer<>::duration([&]{
            s1 = parallelReduce(std::begin(v1), std::end(v1), 0LL, std::plus<>());
        });

        assert(v0 == v1);
        assert(s0 == s1);

        std::cout
                << std::right << std::setw(8) << std::setfill(' ') << size
                << std::right << std::setw(8) << std::chrono::duration<double, std::micro>(tsm).count()
                << std::right << std::setw(8) << std::chrono::duration<double, std::micro>(tpm).count()
                << std::right << std::setw(8) << std::chrono::duration<double, std::micro>(tsf).count()
                << std::right << std::setw(8) << std::chrono::duration<double, std::micro>(tpf).count()
                << std::endl;
    }
}

} //namespace =================================================================

void test_ch_08_fold()
{
    test_map_reduce();
}
