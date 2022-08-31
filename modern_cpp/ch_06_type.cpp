#include <iostream>
#include <type_traits>
#include <string>
#include <random>

#include "util.h"

using namespace std::string_literals;

namespace { //=================================================================

template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
T mul(const T t0, const T t1)
{
    return t0 * t1;
}

void test_mul()
{
    auto i0 = 2;
    auto i1 = 3;
    auto i2 = mul(i0, i1);

    auto s0 = "Hello,"s;
    auto s1 = "Qt6"s;
    //auto s2 = mul(s0, s1);
}

template <typename T>
struct PodWrapper {
    static_assert(std::is_pod<T>::value, "not POD");
    T value;
};

struct A {
    int iValue;
    double dValue;
    char array[8];
};
struct B {
    int id;
    std::string name;
};

void test_pod()
{
    auto p0 = PodWrapper<A>();
    //auto p1 = PodWrapper<B>();
}

template <typename T>
struct ConstWrapper {
    typedef typename std::conditional<
        std::is_const<T>::value,
        T,
        typename std::add_const<T>::type>::type const_type;
};

template <int size>
using number_type = typename std::conditional<
    size<=1,
    std::int8_t,
    typename std::conditional<
        size<=2,
        std::int16_t,
        typename std::conditional<
            size<=4,
            std::int32_t,
            std::int64_t
        >::type
    >::type
>::type;

template <typename T,
          typename D = std::conditional_t<
              std::is_integral<T>::value,
              std::uniform_int_distribution<T>,
              std::uniform_real_distribution<T>>,
          typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
std::vector<T> generateRandom(const T min, const T max, const size_t size)
{
    auto v = std::vector<T>(size);
    auto rd = std::random_device{};
    auto mt = std::mt19937{ rd() };

    auto dist = D{min, max};

    std::generate(std::begin(v), std::end(v), [&dist, &mt]{
        return dist(mt);
    });

    return v;
}

void test_conditional()
{
    static_assert(std::is_const<ConstWrapper<int>::const_type>::value);
    static_assert(std::is_const<ConstWrapper<const int>::const_type>::value);

    using long_type = std::conditional<sizeof(void*) <= 4, long, long long>::type;

    auto n = long_type{ 62 };
    PRINT_FUNC(typeid (n).name());

    static_assert(sizeof(number_type<1>) == 1);
    static_assert(sizeof(number_type<2>) == 2);
    static_assert(sizeof(number_type<4>) == 4);
    static_assert(sizeof(number_type<8>) == 8);

#if (0)
    auto v0 = generateRandom(1, 10, 10);
#else
    auto v0 = generateRandom(1.0, 10.0, 10);
#endif
    print(v0);
}

template <typename T>
auto process(T arg)
{
    if constexpr (std::is_same<T, bool>::value) {
        return !arg;
    } else if constexpr (std::is_integral<T>::value) {
        return -arg;
    } else if constexpr (std::is_floating_point<T>::value) {
        return std::abs(arg);
    } else {
        return arg;
    }
}

void test_constexpr()
{
    PRINT_FUNC(process(true));
    PRINT_FUNC(process(2));
    PRINT_FUNC(process(6.2));
    PRINT_FUNC(process("Hello, Qt6"s));
}

template <typename T>
struct IsVoid {
    static const bool value = false;
};

template <>
struct IsVoid<void> {
    static const bool value = true;
};

template <typename T>
struct IsPointer {
    static const bool value = false;
};

template <typename T>
struct IsPointer<T*> {
    static const bool value = true;
};

struct C {
    int value;
};

void test()
{
    std::cout << std::boolalpha;

    PRINT_FUNC(IsVoid<C>::value);
    PRINT_FUNC(IsVoid<void>::value);
    PRINT_FUNC(IsVoid<void()>::value);

    PRINT_FUNC(IsPointer<C*>::value);
}

namespace custom_type_traits {

struct A {
    std::string serialize() {
        return "plain"s;
    }
};

struct B {
    std::string serializeWithEncoding() {
        return "encoded"s;
    }
};

template <typename T>
struct IsSerializeWithEncoding {
    static const bool value = false;
};

template <>
struct IsSerializeWithEncoding<B> {
    static const bool value = true;
};

template <bool b>
struct Serializer {
    template <typename T>
    static auto serialize(T& v) {
        return v.serialize();
    }
};

template <>
struct Serializer<true> {
    template<typename T>
    static auto serialize(T& v) {
        return v.serializeWithEncoding();
    }
};

template <typename T>
auto serialize(T& v) {
    return Serializer<IsSerializeWithEncoding<T>::value>::serialize(v);
};

void test()
{
    std::cout << std::boolalpha;
    PRINT_FUNC(IsSerializeWithEncoding<A>::value);
    PRINT_FUNC(IsSerializeWithEncoding<B>::value);
    PRINT_FUNC(IsSerializeWithEncoding<int>::value);
    PRINT_FUNC(IsSerializeWithEncoding<std::string>::value);

    auto a = A{};
    auto b = B{};

    PRINT_FUNC(serialize(a));
    PRINT_FUNC(serialize(b));
}

} //custom_type_traits ----------------------------------------------

} //namespace =================================================================

void test_ch_06_type()
{
#if (0) // done
    test_constexpr();
    test();
    custom_type_traits::test();
#endif

    test_conditional();
}
