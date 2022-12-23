//=============================================================================
//  1. measurement
//  2. guideline
//  3. make code
//  4. performance improvement
//  5. monitoring
//=============================================================================

#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <execution>
#include <chrono>
#include <math.h>

namespace { //=================================================================

auto measure = [](auto f) {
    auto t0 = std::chrono::high_resolution_clock::now();
    f();
    auto t1 = std::chrono::high_resolution_clock::now();
    auto duration = t1 - t0;

    return duration;
};
auto measureAndPrint = [](auto f, const auto& desc) {
    auto duration = measure(f);
    std::cout << desc << ": " << duration.count() << "ns\n";
};

namespace _1 { //parallel execution

void test()
{
    //sort
    //find
    //copy
    //transform
    //reduce

    auto v = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto all_of_parallel = [&v](){
        return std::all_of(std::execution::par, v.begin(), v.end(), [](auto value){
            return 5 < value;
        });
    };
    auto all_of_sequence = [&v](){
        return std::all_of(std::execution::seq, v.begin(), v.end(), [](auto value){
            return 5 < value;
        });
    };

#if (0)
    auto dur0 = measure(all_of_parallel);
    std::cout << "parallel: " << dur0.count() << "ns.\n";

    auto dur1 = measure(all_of_sequence);
    std::cout << "sequence: " << dur1.count() << "ns.\n";
#else
    measureAndPrint(all_of_parallel, "parallel");
    measureAndPrint(all_of_sequence, "sequence");
#endif
}

} //_1 --------------------------------------------------------------

namespace _2 { //memoization

std::function<long long(int, int)> power = [](auto base, auto exp){
    return std::pow(base, exp);
};

using Param = std::tuple<int, int>;
using Cache = std::map<Param, long long>;

template <typename Arg, typename Return>
auto memoize = [](std::function<Return(Arg, Arg)> func) {
    std::map<std::tuple<Arg,Arg>, Return> cache;

    return [=](Arg base, Arg exp) mutable {
        Return result;

        std::tuple<Arg,Arg> param(base, exp);
        auto iter = cache.find(param);
        if (iter == cache.end()) {
            result = func(base, exp);
            cache[param] = result;
        } else {
            result = iter->second;
        }
        return result;
    };
};

void test()
{
    auto memoizedPower = memoize<int, long long>(power);

    auto memoizedPowerWrapper = [memoizedPower](int base, int exp) mutable {
        std::cout << "memoizedPower(" << base << ", " << exp << "): " << memoizedPower(base, exp) << "\n";
    };

    memoizedPowerWrapper(1, 1);
    memoizedPowerWrapper(2, 2);
    memoizedPowerWrapper(3, 3);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_10()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
