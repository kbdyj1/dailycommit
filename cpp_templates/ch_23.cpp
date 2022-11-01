//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.23 Meta programming
//
//  22/11/01
//=============================================================================

#include <iostream>
#include <array>

namespace { //=================================================================

namespace _1 { //value meta programming

template <typename T>
constexpr T sqrt(T t)
{
    if (t <= 1)
        return t;

    T lo = 0;
    T hi = t;
    for (;;) {
        auto mid = (hi+lo) / 2;
        auto mid2 = mid*mid;
        if (hi <= lo+1 || t == mid2) {
            return mid;
        }
        if (mid2 < t) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    return t;
}

void test()
{
    auto value = 8.0;
    auto s = sqrt(value);
    std::cout << "sqrt(" << value << "): " << s << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 { //type meta programming

template <typename T>
struct RemoveAllExtentsT {
    using Type = T;
};

template <typename T, std::size_t N>
struct RemoveAllExtentsT<T[N]> {
    using Type = typename RemoveAllExtentsT<T>::Type;
};

template <typename T>
struct RemoveAllExtentsT<T[]> {
    using Type = typename RemoveAllExtentsT<T>::Type;
};

template <typename T>
using RemoveAllExtents = typename RemoveAllExtentsT<T>::Type;

void test()
{
    std::cout << "int[]: " << typeid(RemoveAllExtents<int[]>).name() << "\n";
    std::cout << "int[5][5] : " << typeid(RemoveAllExtents<int[5][5]>).name() << "\n";
    std::cout << "int[][5]: " << typeid(RemoveAllExtents<int[][5]>).name() << "\n";
    std::cout << "int(*)[5]: " << typeid(RemoveAllExtents<int(*)[5]>).name() << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 { //hybrid meta programming

#if (0)
template <typename T, std::size_t N>
auto dotProduct(const std::array<T, N>& x, const std::array<T, N>& y) {
    auto result = T{};

    for (auto i=0; i<N; i++) {
        result += x[i] * y[i];
    }

    return result;
}
#else
template <typename T, std::size_t N>
struct DotProduct {
    static inline T result(const T* x, const T* y) {
        return *x * *y + DotProduct<T, N-1>::result(x+1, y+1);
    }
};

template <typename T>
struct DotProduct<T, 0> {
    static inline T result(const T*, const T*) {
        return T{};
    }
};

template <typename T, std::size_t N>
auto dotProduct(const std::array<T, N>& x, const std::array<T, N>& y) {
    return DotProduct<T, N>::result(x.data(), y.data());
}

void test()
{
    auto a0 = std::array<int, 5>{ 1, 2, 3, 4, 5 };
    auto a1 = std::array<int, 5>{ 1, 2, 3, 4, 5 };
    auto result = dotProduct(a0, a1);

    std::cout << "dotProduct(a0, a1): " << result << "\n";
}

#endif

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_23()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
