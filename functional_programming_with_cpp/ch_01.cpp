#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace { //=================================================================

struct Sums {
    Sums() : evenSum(0)
           , oddSum(0)
           , total(0)
    {}
    Sums(int even, int odd, int total) : evenSum(even), oddSum(odd), total(total)
    {}

    int evenSum;
    int oddSum;
    int total;
};

namespace _1 {

template <typename Container, typename Pred>
const Container filter(const Container& in, Pred pred)
{
    Container filtered;
    std::copy_if(in.begin(), in.end(), std::back_inserter(filtered), pred);

    return filtered;
}

template <typename T, template <class> class Container>
const T sum(const Container<T>& in, const T& init = 0)
{
    return std::accumulate(in.begin(), in.end(), init);
}

bool isEven(int value)
{
    return value % 2 == 0;
}

bool isOdd(int value)
{
    return value % 2 != 0;
}

const Sums sumWithFunctionalLoopsSimplified(const std::vector<int>& in)
{
    Sums theSums{
        sum(filter(in, isEven)),
        sum(filter(in, isOdd)),
        sum(in)
    };
    return theSums;
}

void test()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto sum = sumWithFunctionalLoopsSimplified(v);

    std::cout << "even: " << sum.evenSum << ", odd: " << sum.oddSum << ", total: " << sum.total << "\n";
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_01()
{
    _1::test();
}
