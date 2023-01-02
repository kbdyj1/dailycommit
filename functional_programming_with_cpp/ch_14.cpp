#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>

#include <range/v3/all.hpp> //https://github.com/ericniebler/range-v3

namespace { //=================================================================

auto isEven = [](int n) {
    return n % 2 == 0;
};
auto isOdd = [](int n) {
    return !isEven(n);
};
auto is3x = [](int n) {
    return n % 3 == 0;
};

namespace _1 { //----------------------------------------------------

void test()
{
    std::vector<int> v{1,2,3,4,5,6,7,8,9,10};

    for (auto i : v | ranges::views::filter(isOdd) | ranges::views::filter(is3x)) {
        std::cout << "isEven && is3x : " << i << "\n";
    }
}

} //_1 --------------------------------------------------------------

namespace _2 { //ranges view, filter

auto pickNumbers = [](const auto& numbers, auto predicate){
    auto ret = std::vector<int>{};
    std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(ret), predicate);
    return ret;
};

auto sum = [](const auto& numbers){
    return std::accumulate(numbers.begin(), numbers.end(), 0);
};

auto sumOfEvenNumbers = [](const auto& numbers){
    return sum(pickNumbers(numbers, isEven));
};

auto sumOfEvenNumbersLazy = [](const auto& numbers){
    return ranges::accumulate(
        (numbers | ranges::views::filter(isEven) | ranges::views::filter(is3x)),
        0
    );
};

auto testConcatFunctions = [](const auto& numbers) {
    std::cout << "sumOfEvenNumbers(): " << sumOfEvenNumbers(numbers) << "\n";
};

auto testLazyFunction = [](const auto& numbers) {
    std::cout << "sumOfEvenNumbersLazy(): " << sumOfEvenNumbersLazy(numbers) << "\n";
};

void test()
{
    auto v = std::vector<int>{1,2,3,4,5,6,7,18,19};

    testConcatFunctions(v);
    testLazyFunction(v);
}

} //_2 --------------------------------------------------------------

namespace _3 { //ranges action

void test()
{
    auto numbers = std::vector<int>{1, 1, 12, 5, 20, 2, 10, 17, 25, 4};

    numbers |= ranges::actions::sort | ranges::actions::unique;

    std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(std::cout, " "));
}

} //_3 --------------------------------------------------------------

namespace _4 {

auto printAll = [](const auto& values) {
    for (auto value : values) {
        std::cout << value << " ";
    }
};

void test()
{
    printAll( ranges::views::ints(1) | ranges::views::take(5) );

    auto everyTenthOfTwo = ranges::views::ints(1) | ranges::views::for_each([](int i){
        return ranges::yield_if(i % 10 == 1, std::pow(2, i));
    });
    auto values = everyTenthOfTwo | ranges::views::take(4);
    //printAll(values);

    std::cout << "\n";

    auto s = ranges::views::ints(32, 126) |
             ranges::views::sample(10) |
             ranges::views::transform([](int c){ return char(c); });

    std::cout << s << "\n";
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_14()
{
#if (0)
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
