#include <iostream>
#include <limits>
#include <utility>
#include <type_traits>

#include "IfThenElse.hpp"

namespace { //=================================================================

namespace _1 { //----------------------------------------------------

template <auto N>
struct SmallestIntT {
    using Type =
        typename IfThenElseT<N <= std::numeric_limits<char>::max(), char,
            typename IfThenElseT<N <= std::numeric_limits<short>::max(), short,
                typename IfThenElseT<N <= std::numeric_limits<int>::max(), int,
                    typename IfThenElseT<N <= std::numeric_limits<long>::max(), long,
                        typename IfThenElseT<N <= std::numeric_limits<long long>::max(), long long, void>::Type
                    >::Type
                >::Type
            >::Type
        >::Type;
};

void test()
{
    std::cout << typeid(SmallestIntT<64>::Type).name() << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
struct MakeUnsignedT {
    using Type = typename std::make_unsigned_t<T>;
};

template <typename T>
struct UnsignedT {
    using Type =
#if (0)
        IfThenElse<std::is_integral_v<T> && !std::is_same_v<T, bool>,
#else
        std::conditional_t<std::is_integral_v<T> && !std::is_same_v<T, bool>,
#endif
            MakeUnsignedT<T>,
            IdentityT<T>
        >;
};

void test()
{
    std:: cout << typeid(UnsignedT<bool>::Type).name() << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T, typename = std::void_t<>>
struct IsNoThrowMoveConstructableT : std::false_type
{};

template <typename T>
struct IsNoThrowMoveConstructableT<T, std::void_t<decltype(T(std::declval<T>()))>> : std::bool_constant<noexcept(T(std::declval<T>()))>
{};

template <typename T0, typename T1>
class Pair
{
    T0 first;
    T1 second;

public:
    Pair(Pair&& other) noexcept(IsNoThrowMoveConstructableT<T0>::value && IsNoThrowMoveConstructableT<T1>::value)
        : first(std::forward<T0>(other.first))
        , second(std::forward<T1>(other.second))
    {}
};

} //_3 --------------------------------------------------------------

namespace _4 {

template <typename T>
struct IsPrimitiveT : std::false_type{
    static constexpr bool value = false;
};

#define DEF_IS_PRIMITIVE(T) \
    template <> struct IsPrimitiveT<T> : std::true_type {}

DEF_IS_PRIMITIVE(void);
DEF_IS_PRIMITIVE(bool);
DEF_IS_PRIMITIVE(char);
DEF_IS_PRIMITIVE(unsigned char);
DEF_IS_PRIMITIVE(signed char);
DEF_IS_PRIMITIVE(wchar_t);
DEF_IS_PRIMITIVE(char16_t);
DEF_IS_PRIMITIVE(char32_t);
DEF_IS_PRIMITIVE(signed short);
DEF_IS_PRIMITIVE(unsigned short);
DEF_IS_PRIMITIVE(signed int);
DEF_IS_PRIMITIVE(unsigned int);
DEF_IS_PRIMITIVE(signed long);
DEF_IS_PRIMITIVE(unsigned long);
DEF_IS_PRIMITIVE(signed long long);
DEF_IS_PRIMITIVE(unsigned long long);
DEF_IS_PRIMITIVE(float);
DEF_IS_PRIMITIVE(double);
DEF_IS_PRIMITIVE(long double);
DEF_IS_PRIMITIVE(std::nullptr_t);

void test()
{
    std::cout << IsPrimitiveT<int>::value << "\n";
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <typename T>
struct IsPointerT : std::false_type
{};

template <typename T>
struct IsPointerT<T*> : std::true_type
{
    using BaseT = T;
};

template <typename T>
struct IsLValueRefT : std::false_type
{};

template <typename T>
struct IsLValueRefT<T&> : std::true_type
{
    using BaseT = T;
};

template <typename T>
struct IsRValueRefT : std::false_type
{};

template <typename T>
struct IsRValueRefT<T&&> : std::true_type
{
    using BaseT = T;
};

template <typename T>
struct IsRefT : public IfThenElseT<IsLValueRefT<T>::value, IsLValueRefT<T>, IsRValueRefT<T>>::Type
{};

void test()
{
    auto i = 0;
    auto*p = &i;
    auto&r = i;

    std::cout << IsPointerT<decltype(i)>::value << "\n";
    std::cout << IsPointerT<decltype(p)>::value << "\n";
    std::cout << IsLValueRefT<decltype(r)>::value << "\n";
    std::cout << IsRValueRefT<decltype (std::declval<int>())>::value << "\n";
}

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T>
struct IsArrayT : std::false_type
{};

template <typename T, std::size_t N>
struct IsArrayT<T[N]> : std::true_type
{
    using BaseT = T;
    static constexpr std::size_t size = N;
};

template <typename T>
struct IsArrayT<T[]> : std::true_type
{
    using BaseT = T;
    static constexpr std::size_t size = 0;
};

template <typename T>
struct IsPointerToMemberT : std::false_type
{};

template <typename T, typename C>
struct IsPointerToMemberT<T C::*> : std::true_type
{};

class C {
    int member;
};

template <typename T>
struct IsFunctionT : std::false_type
{};

template <typename R, typename... Params>
struct IsFunctionT<R (Params...)> : std::true_type {
};

void test()
{
    std::cout << IsPointerToMemberT<int(C::*)>::value << "\n";
    std::cout << IsPointerToMemberT<int(C::*)()>::value << "\n";
}

} //_6 --------------------------------------------------------------

namespace _7 {

template <typename T, typename = std::void_t<>>
struct IsClassT : std::false_type
{};

template <typename T>
struct IsClassT<T, std::void_t<int T::*>> : std::true_type
{};

void test()
{
    auto l = []{};
    std::cout << "[]{} is class? " << IsClassT<decltype(l)>::value << "\n";
}

} //_7 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_19_etc()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _2::test();
    _4::test();
    _5::test();
    _6::test();
#endif

    _7::test();
}
