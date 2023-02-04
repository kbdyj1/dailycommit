#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>

namespace { //=================================================================

namespace _1 {

bool compareGreaterFunction(const std::string& l, const std::string& r)
{
    return l > r;
}

struct StringCompareGreater {
    bool operator()(const std::string& l, std::string& r) {
        return l > r;
    }
};

auto compareGreaterLambda = [](const std::string& l, const std::string& r) {
    return l > r;
};

void test()
{
    auto tickers = std::vector<std::string>{ "FDS", "AAPL", "TSLA", "X", "GOOG", "F", "BA" };
    std::sort(std::begin(tickers), std::end(tickers), compareGreaterLambda);

    std::copy(std::begin(tickers), std::end(tickers), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << '\n';
}

} //_1 --------------------------------------------------------------

namespace _2 {

class TickerCache {
public:
    double getOpeningPrice(const std::string& ticker) const;
};

const TickerCache& getCache();

class PriceCompare {
    TickerCache cache;

public:
    PriceCompare(const TickerCache& cache) : cache(cache)
    {}

    bool operator()(const std::string& l, const std::string& r) const
    {
        return cache.getOpeningPrice(l) < cache.getOpeningPrice(r);
    }
};

void sortByPrice(std::vector<std::string>& tickers)
{
    std::sort(std::begin(tickers), std::end(tickers), PriceCompare(getCache()));
}

void test()
{
    auto ticker_cache = TickerCache{};

    //c++14 capturing move-only types
    auto lambda = [cache = std::move(ticker_cache)](const std::string& l, const std::string& r) {
        return cache.getOpeningPrice(l) < cache.getOpeningPrice(r);
    };
}

} //_2 --------------------------------------------------------------

namespace _3 {

class Widget {
    std::string mData;

public:
    Widget() : mData("Hello, Qt6")
    {}

    auto get_functor() {
#if (0)
        const auto copy = mData;

        return [copy]{ std::cout << copy << '\n'; };
#else
        return [copy = mData]{ std::cout << copy << '\n'; };
#endif
    }

#if 201703L <= __cplusplus
    auto get_functor_cxx17() {
        return [*this]{ std::cout << mData << '\n'; };
    }
#endif
};

auto get_functor()
{
    const std::string data = "Hello, Qt6";

    return [=]{ std::cout << data << '\n'; };
}

void test()
{
#if (0)
    const auto functor = get_functor();
#else
    const auto functor = Widget().get_functor();
#endif

    functor();
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test_binary_operations()
{
    std::vector<std::function<int(int,int)>> binary_ops;

    binary_ops.emplace_back([](int l, int r){ return l + r; });
    binary_ops.emplace_back([](int l, int r){ return l - r; });
    binary_ops.emplace_back([](int l, int r){ return l * r; });
    binary_ops.emplace_back([](int l, int r){ return l / r; });

    for (auto fn : binary_ops) {
        std::cout << fn(10, 2) << '\n';
    }
}

struct Widget {
    Widget() = default;
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

    Widget(Widget&&) = default;
    Widget& operator=(Widget&&) = default;
};

void test_move_only_type()
{
#if (0) //compile error
    const std::function<void()> fn = [state = Widget()]{ std::cout << "= Widget()"; };
#else
    const std::function<void()> fn = [state = std::make_shared<Widget>()]{ std::cout << "=Widget\n"; };
#endif

    fn();
}

void test()
{
    //test_binary_operations();

    test_move_only_type();
}

} //_4 --------------------------------------------------------------

} //===========================================================================

void test_modern_c_in_depth_lambdas()
{
#if (0) //done
    _1::test();
    _3::test();
#endif

    _4::test();
}
