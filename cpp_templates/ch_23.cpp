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

namespace _4 {

template <unsigned N, unsigned D = 1>
struct Ratio {
    static constexpr unsigned num = N;
    static constexpr unsigned den = D;
    using Type = Ratio<num, den>;
};

template <typename R0, typename R1>
struct RatioAddImpl {
private:
    static constexpr unsigned den = R0::den * R1::den;
    static constexpr unsigned num = R0::num * R1::den + R0::den * R1::num;

public:
    typedef Ratio<num, den> Type;
};

template <typename R0, typename R1>
using RatioAdd = typename RatioAddImpl<R0, R1>::Type;

template <typename T, typename U = Ratio<1>>
class Duration {
public:
    using ValueType = T;
    using UnitType = U;

private:
    ValueType val;

public:
    constexpr Duration(ValueType v = 0) : val(v)
    {}
    constexpr ValueType value() const {
        return val;
    }
};

template <typename T0, typename U0, typename T1, typename U1>
auto constexpr operator+(const Duration<T0, U0>& l, const Duration<T1, U1>& r)
{
    using VT = Ratio<1, RatioAdd<U0, U1>::den>;
    auto val = l.value() * VT::den / U0::den * U0::num + r.value() * VT::den / U1::den * U1::num;

    return Duration<decltype(val), VT>(val);
}

void test()
{
    using R0 = Ratio<1, 1000>;
    using R1 = Ratio<2, 3>;
    using Result = RatioAdd<R0, R1>;

    auto res = Result{};

    std::cout << "Ratio<" << res.num << ", " << res.den << ">\n";

    int x = 200;
    int y = 3;

    auto a = Duration<int, Ratio<1,1000>>(x);
    auto b = Duration<int, Ratio<1,4>>(y);
    auto c = a + b;

    std::cout << "c: " << c.value() << "\n";
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <int N, int LO=1, int HI=N>
struct Sqrt {
    static constexpr auto mid = (LO+HI+1)/2;
    static constexpr auto value = (N<mid*mid) ? Sqrt<N, LO, mid-1>::value : Sqrt<N, mid, HI>::value;
};

template <int N, int M>
struct Sqrt<N, M, M> {
    static constexpr auto value = M;
};

void test()
{
    std::cout << "Sqrt<150>: " << Sqrt<100>::value << "\n";
}

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T, typename U>
struct Doublify
{};

template <int N>
struct Trouble {
    using LongType = Doublify<typename Trouble<N-1>::LongType, typename Trouble<N-1>::LongType>;
};

template <>
struct Trouble<0> {
    using LongType = double;
};

void test()
{
    Trouble<10>::LongType ouch;
}

} //_6 --------------------------------------------------------------

namespace _7 { //Erwin Unruh

template <int p, int i>
struct IsPrime {
    enum {
        pri = (p==2) || ((p%i) && IsPrime<(i>2?p:0), i-1>::pri)
    };
};

template <>
struct IsPrime<0,0> {
    enum {
        pri = 1
    };
};

template <>
struct IsPrime<0,1> {
    enum {
        pri = 1
    };
};

template <int i>
struct D {
    D(void*);
};

template <int i>
struct CondNull {
    static int const value = i;
};

template <>
struct CondNull<0> {
    static void* value;
};

void* CondNull<0>::value = 0;

template <int i>
struct PrintPrime {
    PrintPrime<i-1> a;

    enum { pri = IsPrime<i,i-1>::pri };

    void f() {
        D<i> d = CondNull<pri ? 1 : 0>::value;
        a.f();
    }
};

template <>
struct PrintPrime<1> {
    enum { pri = 0 };
    void f() {
        D<1> d = 0;
    }
};

void test()
{
#if (0) //error message is a prime number
    PrintPrime<17> a;
    a.f();
#endif
}

} //_7 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_23()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
#endif

    _5::test();
}
