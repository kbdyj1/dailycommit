#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

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

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_modern_c_in_depth_lambdas()
{
    _1::test();
}
