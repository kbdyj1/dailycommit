#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

namespace { //=================================================================

template <typename ForwardIter, typename T>
ForwardIter lowerBound(ForwardIter first, ForwardIter last, const T& value)
{
    ForwardIter iter;
    typename std::iterator_traits<ForwardIter>::difference_type count, step;
    count = std::distance(first, last);

    while (count > 0) {
        iter = first;
        step = count / 2;
        std::advance(iter, step);
        if (*iter < value) {
            first = ++iter;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

template <typename ForwardIter, typename T, typename Compare>
ForwardIter lowerBound(ForwardIter first, ForwardIter last, const T& value, Compare comp)
{
    ForwardIter iter;
    typename std::iterator_traits<ForwardIter>::difference_type count, step;
    count = std::distance(first, last);

    while (count > 0) {
        iter = first;
        step = count / 2;
        std::advance(iter, step);
        if (comp(*iter, value)) {
            first = ++iter;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

struct PriceInfo { double price; };

void test_lower_bound()
{
    const auto data = std::vector<int>{ 1, 2, 4, 5, 5, 6 };
    for (int i=0; i<8; i++) {
        auto lower = lowerBound(data.begin(), data.end(), i);
        std::cout << i << " <= ";
        lower != data.end()
                ? std::cout << *lower << " at index " << std::distance(data.begin(), lower)
                : std::cout << " not found";
        std::cout << std::endl;
    }

    auto prices = std::vector<PriceInfo>{ {100.0}, {101.5}, {102.5}, {102.5}, {107.3}};
    for (auto target : {102.5, 110.2}) {
        auto info = lowerBound(prices.begin(), prices.end(), target, [](const PriceInfo& info, double value){
            return info.price < value;
        });
        info != prices.end()
                ? std::cout << info->price << " at index " << std::distance(prices.begin(), info)
                : std::cout << target << " not found";

        std::cout << std::endl;
    }
}

struct S
{
    int number;
    char name;
    bool operator < (const S& s) const { return number < s.number; }
};

void test_equal_range()
{
    const auto v = std::vector<S>{ {1, 'A'}, {2, 'B'}, {2, 'C'}, {3, 'H'}, {2, 'D'}, {4, 'G'}, {3, 'F'}};
    const auto value = S{2, '?'};

    const auto p = std::equal_range(v.begin(), v.end(), value);
    for (auto i = p.first; i != p.second; i++) {
        std::cout << i->name << " ";
    }
    std::cout << std::endl;
}

// Per.10 Rely on the static type system

// Per.11 Move computation from run time to compile time

constexpr double times(double d, int n)
{
    double ret = 1.0;
    while (n--)
        ret *= d;

    return ret;
}

constexpr double D0 = times(2.0, 3);

constexpr int StackMax = 20;

template <typename T>
struct Scoped
{
    T obj;
};

template <typename T>
struct OnHeap
{
    T* pObj;

    OnHeap() : pObj(new T)
    {
        std::cout << "OnHeap(" << sizeof(T) << ")" << std::endl;
    }
   ~OnHeap()
    {
        delete pObj;
        std::cout << "~OnHeap()" << std::endl;
    }
};

template <typename T>
using Handle = typename std::conditional<(sizeof(T) <= StackMax), Scoped<T>, OnHeap<T>>::type;

// Per.19 Access memory predicatably

void test_per_19()
{
    constexpr int Rows = 4;
    constexpr int Cols = 4;

    int mat[Rows][Cols];
    int sum = 0;

    // Bad
    for (auto c = 0; c < Cols; c++) {
        for (auto r = 0; r < Rows; r++) {
            sum += mat[r][c];
        }
    }
}

} // namespace ================================================================

void test_per_7()
{
    //test_lower_bound();
    test_equal_range();
}

void test_per_11()
{
    Handle<double> v0;
    Handle<std::array<int, 32>> v1;
}
