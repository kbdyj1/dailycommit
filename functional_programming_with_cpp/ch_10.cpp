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
#include <future>
#include <iterator>

namespace { //=================================================================

template <typename Dest>
auto transformAll = [](auto const& source, auto pred) {
    Dest result;
    result.reserve(source.size());
    std::transform(source.begin(), source.end(), std::back_inserter(result), pred);
    return std::move(result);
};

auto increment = [](const auto& value) {
    return value + 1;
};

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

namespace _3 {

std::promise<std::string> secretMessagePromise;

void test()
{
    auto printSecretMessage = [](std::future<std::string>& future) {
        auto message = future.get();
        std::cout << "secret message: " << message << std::endl;
    };
    auto secretMessageFuture = secretMessagePromise.get_future();

    std::thread isPrimeThread(printSecretMessage, std::ref(secretMessageFuture));
    secretMessagePromise.set_value("Hello, Qt6");

    isPrimeThread.join();
}

} //_3 --------------------------------------------------------------

namespace _4 {


auto rangeFromTo = [](const int start, const int end) {
    auto v = std::vector<int>(end-start+1);
    std::iota(v.begin(), v.end(), start);
    return v;
};
auto rangeFrom2To = std::bind(rangeFromTo, 2, std::placeholders::_1);

auto isDivisibleBy = [](auto value, auto factor) {
    return value % factor == 0;
};

auto none_of_collection = [](const auto& collection, auto pred) {
    return std::none_of(collection.begin(), collection.end(), pred);
};

auto isPrime = [](int x) {
    auto xIsDivisibleBy = std::bind(isDivisibleBy, x, std::placeholders::_1);
    return none_of_collection(rangeFrom2To(x -1), xIsDivisibleBy);
};

void test1()
{
    const int value = 7757;

    std::future<bool> futureIsPrime(std::async(isPrime, std::ref(value)));
    std::cout << "calculating..." << std::endl;
    auto result = futureIsPrime.get();

    std::cout << value << " prime? " << result << "\n";
}

void test2() //functional async
{
    auto v = std::vector<int>{2, 27, 1977, 7757};
    auto futures = std::vector<std::future<bool>>{};
    for (auto value : v) {
        futures.push_back(std::async(isPrime, value));
    }

    for (auto& future : futures) {
        std::cout << future.get() << "\n";
    }
    std::cout << std::endl;
}

auto makeFuture = [](auto value) {
    return std::async(isPrime, value);
};

void test3()
{
    auto v = std::vector<int>{2, 27, 1977, 7757};
    std::vector<std::future<bool>> futures = transformAll<std::vector<std::future<bool>>>(v, makeFuture);

    auto results = std::vector<bool>(v.size());
    std::transform(futures.begin(), futures.end(), results.begin(), [](std::future<bool>& future){
        return future.get();
    });

    std::copy(results.begin(), results.end(), std::ostream_iterator<bool>(std::cout, ","));
    std::cout << std::endl;
}

void test()
{
    test3();
}

} //_4 --------------------------------------------------------------

namespace _5 { //Reactive

void test()
{
    const long Size1G = 125000000;
    auto v = std::vector<long long>(Size1G);
    std::fill_n(v.begin(), Size1G, 1000L);

    auto result = transformAll<std::vector<long long>>(v, increment);

    std::cout << result[0] << std::endl;
}

}

} //namespace =================================================================

void test_ch_10()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
#endif

    _5::test();
}
