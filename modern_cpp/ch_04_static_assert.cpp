#include <iostream>
#include <string>
#include <type_traits>

namespace { //=================================================================

#define USE_ENABLE_IF

struct alignas(8) Item {
    int id;
    bool active;
    double value;
};

static_assert (sizeof(Item) == 16, "size of Item should be 16 bytes.");

#if !defined(USE_ENABLE_IF)
template <typename T>
class PodWrapper {
    static_assert (std::is_pod<T>::value, "POD type expected!!!");
    T value;
};
#else
template <typename T, typename = typename std::enable_if<std::is_pod<T>::value, T>::type>
class PodWrapper {
    T value;
};
#endif

struct Point {
    int x;
    int y;
};
class A {
    int id;
    double value;

public:
    A() = default;
};

#if !defined(USE_ENABLE_IF)
template <typename T>
auto mul(const T a, const T b)
{
    static_assert (std::is_integral<T>::value, "Integral type expected!!!");

    return a * b;
}
#else
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
auto mul(const T a, const T b)
{
    return a * b;
}
#endif

void test_static_assert()
{
    PodWrapper<int> w0;
    PodWrapper<Point> w1;
    //PodWrapper<std::string> w2;   // assert fail. is_pod<T>
    PodWrapper<A> w3;

    auto v0 = mul(3, 4);
    //auto v1 = mul(1.2, 2.3);      // assert fail. is_integral<T>
}

template <typename T>
void func(const typename T::data_type a)
{
    std::cout << "func<>(" << a << ")\n";
}

void func(const int a)
{
    std::cout << "func(" << a << ")\n";
}

template <typename T>
struct SomeType
{
    using data_type = T;
};
template <typename T>
std::ostream& operator<<(std::ostream& os, const SomeType<T>& s)
{
    os << "SomeType<" << typeid(T).name() << ">";
    return os;
}

void test()
{
    func(40);

    using SomeIntType = SomeType<int>;

    func<SomeIntType>(60);
}

#if !defined(USE_ENABLE_IF)
template <typename T>
auto compute(const T a, const T b)
{
    static_assert (std::is_integral<T>::value, "An integral type expected !!!");

    return a + b;
}

template <typename T, typename = void>
auto compute(const T a, const T b)
{
    static_assert (!std::is_integral<T>::value, "A non integral type expected !!!");

    return a * b;
}
#else
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
auto compute (const T a, const T b)
{
    return a + b;
}

template <typename T, typename = typename std::enable_if<!std::is_integral<T>::value, T>::type, typename = void>
auto compute (const T a, const T b)
{
    return a * b;
}
#endif


template <typename T>
auto compute2(const T a, const T b)
{
    if constexpr (std::is_integral<T>::value)
        return a + b;
    else
        return a * b;
}

void test_compute()
{
#if defined(USE_ENABLE_IF)
    auto v0 = compute(1, 2);
    std::cout << "compute(1, 2): " << v0 << "\n";

    auto v1 = compute(1.0, 2.0);
    std::cout << "compute(1.0, 2.0): " << v1 << "\n";
#endif

    auto v2 = compute2(1, 3);
    std::cout << "compute2(1, 3): " << v2 << "\n";

    auto v3 = compute2(1.5, 2.5);
    std::cout << "compute2(1.5, 2.5): " << v3 << "\n";
}

using byte8 = unsigned char;

namespace binary {

namespace internal {
template <typename CharT, char d, char... bits>
constexpr CharT eval()
{
    if constexpr(sizeof...(bits) == 0)
        return static_cast<CharT>(d-'0');
    else if constexpr(d == '0')
        return eval<CharT, bits...>();
    else if constexpr(d == '1')
        return static_cast<CharT>((1 << sizeof...(bits) | eval<CharT, bits...>()));
}
}// internal

template <char... bits>
constexpr byte8 operator""_b8()
{
    static_assert(sizeof...(bits) <= 8, "binary literal b8 must be up to 8 digits long");

    return internal::eval<byte8, bits...>();
}

}// binary

void test_binary()
{
    using namespace binary;

    auto value = 10001001_b8;

    std::cout << "10001001_b8: " << (int)value << "\n";
}

} //===========================================================================

void test_ch_04_static_assert()
{
    test_static_assert();
    test_compute();
    test_binary();
}
