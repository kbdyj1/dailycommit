#include <iostream>
#include <vector>
#include <type_traits>
#include <utility>

#include "IsConvertible.hpp"

namespace { //=================================================================

struct A {
    A() { std::cout << "A::A()\n"; }
};

struct B {
    B() = delete;
};

namespace _1 {

template <typename T>
struct IsDefaultConstructalbe {
private:
    template<typename U, typename = decltype(U())>
    static char test(void*);

    template<typename>
    static long test(...);

public:
    static constexpr bool value = std::is_same_v<decltype(test<T>(nullptr)), char>;
};

void test()
{
    std::cout << "IsDefaultConstructalbe<A>? " << IsDefaultConstructalbe<A>::value << "\n";
    std::cout << "IsDefaultConstructalbe<B>? " << IsDefaultConstructalbe<B>::value << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

#if (0)
using Size1 = char;
using Size2 = struct { char _[2]; };
#else
using Size1 = char(&)[1];
using Size2 = char(&)[2];
#endif

template <typename...>
static Size1 test(void*);

template <typename...>
static Size2 test(...);

template <typename T>
struct IsDefaultConstructalbeHelper {
private:
    template<typename U, typename = decltype(U())>
    static std::true_type test(void*);

    template<typename>
    static std::false_type test(...);

public:
    using Type = decltype(test<T>(nullptr));
};

template <typename T>
struct IsDefaultConstructable : IsDefaultConstructalbeHelper<T>::Type
{};

void test()
{
    std::cout << "IsDefaultConstructable<A>? " << IsDefaultConstructable<A>::value << "\n";
    std::cout << "IsDefaultConstructable<B>? " << IsDefaultConstructable<B>::value << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 { //partial specialization

template <typename...>
using Void = void;

template <typename, typename = Void<>>
struct IsDefaultConstructable : std::false_type
{};

template <typename T>
#if defined(__cpp_lib_void_t)
struct IsDefaultConstructable<T, std::void_t<decltype(T())>> : std::true_type
#else
struct IsDefaultConstructable<T, Void<decltype(T())>> : std::true_type
#endif
{};

#if defined(__cpp_lib_void_t)
void test()
{
    std::cout << "__cpp_lib_void_t implemented.\n";

    std::cout << "IsDefaultConstructable<A>? " << IsDefaultConstructable<A>::value << "\n";
    std::cout << "IsDefaultConstructable<B>? " << IsDefaultConstructable<B>::value << "\n";
}
#else
void test()
{
    std::cout << "__cpp_lib_void_t not implemented.\n";
}
#endif

} //_3 --------------------------------------------------------------

namespace _4 {

template <typename F, typename... Args, typename = decltype(std::declval<F>()(std::declval<Args&&>()...))>
std::true_type isValidImpl(void*);

template <typename F, typename... Args>
std::false_type isValidImpl(...);

inline constexpr auto isValid = [](auto f){
    return [](auto&&... args) {
        return decltype(isValidImpl<decltype(f), decltype(args)&&...>(nullptr)){};
    };
};

void func(int i0, int i1, const std::string& s)
{}

void test()
{
    auto validFunc = isValid(func);
    {
        auto ret = validFunc(0, 1, "Qt");
        std::cout << "validFunc(0, 1, 'Qt'): " << ret.value << "\n";
    }
    {
        auto ret = validFunc(1.0, "Qt", 6.0);
        std::cout << "validFunc(1.0, 'Qt', 6.0): " << ret.value << "\n";
    }
}

template <typename T>
struct TypeT {
    using Type = T;
};

template <typename T>
constexpr auto type = TypeT<T>{};

template <typename T>
T valueT(TypeT<T>);

constexpr auto isDefaultConstructalbe =
        isValid([](auto x) -> decltype((void)decltype(valueT(x))()){
        });

void test_default_constructalbe()
{
    {
        auto ret = isDefaultConstructalbe(type<A>);
        std::cout << "A: " << ret.value << "\n";
    }
    {
        auto ret = isDefaultConstructalbe(type<B>);
        std::cout << "B: " << ret.value << "\n";
    }
    std::cout << typeid(decltype(A())).name() << "\n";
}

constexpr auto hasFirst = isValid([](auto x) -> decltype((void)valueT(x).first){
});

void test_has_first()
{
    std::cout << "std::pair<int,int> hasFirst()? " << hasFirst(type<std::pair<int,int>>).value << "\n";
    std::cout << "std::vector<int> hasFirst()? " << hasFirst(type<std::vector<int>>).value << "\n";
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <typename, typename, typename = std::void_t<>>
struct HasPlus : std::false_type
{};

template <typename T0, typename T1>
struct HasPlus<T0, T1, std::void_t<decltype(std::declval<T0>() + std::declval<T1>())>> : std::true_type
{};

template <typename T0, typename T1, bool = HasPlus<T0, T1>::value>
struct PlusResult {
    using Type = decltype(std::declval<T0>() + std::declval<T1>());
};

template <typename T0, typename T1>
struct PlusResult<T0, T1, false>
{};

} //_5 --------------------------------------------------------------

namespace _6 {

#if (0)
template <typename From, typename To, bool = std::is_void_v<To> ||
                                             std::is_array_v<To> ||
                                             std::is_function_v<To>>
struct IsConvertibleHelper {
    using Type = std::integral_constant<bool, std::is_void_v<To> && std::is_void_v<From>>;
};

template <typename From, typename To>
struct IsConvertibleHelper<From, To, false> {
private:
    static void helper(To);

    template<typename F, typename = decltype(helper(std::declval<F>()))>
    static std::true_type test(void*);

    template<typename>
    static std::false_type test(...);

public:
    using Type = decltype(test<From>(nullptr));
};

template <typename From, typename To>
struct IsConvertibleT : IsConvertibleHelper<From, To>::Type
{};

template <typename From, typename To>
using IsConvertable = typename IsConvertibleT<From, To>::Type;

template <typename From, typename To>
constexpr bool isConvertible = IsConvertibleT<From, To>::value;
#endif

class Base
{};
class Derived : public Base
{};

void test()
{
    std::cout << "IsConvertibleT<Derived, Base>? " << IsConvertibleT<Derived, Base>::value << "\n";
    std::cout << isConvertible<Derived, Base> << "\n";
    std::cout << "IsConvertibleT<Base, Derived>? " << IsConvertibleT<Base, Derived>::value << "\n";
    std::cout << isConvertible<Base, Derived> << "\n";
    std::cout << isConvertible<const char*, std::string> << "\n";
}

} //_6 --------------------------------------------------------------

namespace _7 {

template <typename...>
using Void = void;

template <typename, typename = Void<>>
struct HasSizeT : std::false_type
{};

template <typename T>
struct HasSizeT<T, Void<typename std::remove_reference_t<T>::size_type>> : std::true_type
{};

struct C {
    using size_type = char;
};
struct CR {
    using size_type = char&;
};

struct size_type
{};
struct Sizable : size_type
{};

#define DEF_HAS_TYPE(Member)                        \
    template <typename, typename = std::void_t<>>   \
    struct HasType_##Member                         \
        : std::false_type                           \
    {};                                             \
    template <typename T>                           \
    struct HasType_##Member<T, std::void_t<typename T::Member>>  \
        : std::true_type                            \
    {}

DEF_HAS_TYPE(value_type);

void test()
{
    std::cout << "HasSizeT<C>? " << HasSizeT<C>::value << "\n";
    std::cout << "HasSizeT<C&>? " << HasSizeT<C&>::value << "\n";
    std::cout << "HasSizeT<CR&>? " << HasSizeT<C&>::value << "\n";
    std::cout << "HasSizeT<Sizable>? " << HasSizeT<Sizable>::value << "\n";

    auto v = std::vector<int>{};
    std::cout << HasType_value_type<decltype(v)>::value << "\n";
}

} //_7 --------------------------------------------------------------

namespace _8 {

#define DEF_HAS_MEMBER(Member)  \
    template <typename, typename = std::void_t<>>   \
    struct HasMember_##Member                       \
        : std::false_type                           \
    {};                                             \
    template <typename T>                           \
    struct HasMember_##Member<T, std::void_t<decltype(&T::Member)>> \
        : std::true_type                            \
    {}

DEF_HAS_MEMBER(internal);
DEF_HAS_MEMBER(external);

class A
{
    int internal;

public:
    int external;
};

void test()
{
    std::cout << "HasMember_internal<A>? " << HasMember_internal<A>::value << "\n";
    std::cout << "HasMember_external<A>? " << HasMember_external<A>::value << "\n";

}
} //_8 --------------------------------------------------------------

namespace _9 {

class A
{};

template <typename, typename = std::void_t<>>
struct HasBegin : std::false_type
{};

template <typename T>
struct HasBegin<T, std::void_t<decltype (std::declval<T>().begin())>> : std::true_type
{};

void test()
{
    auto v = std::vector<int>{ 1, 2, 3 };
    std::cout << "HasBegin<decltype(v)>? " << HasBegin<decltype(v)>::value << "\n";
    std::cout << "HasBegin<A>? " << HasBegin<A>::value << "\n";
}

} //_9 --------------------------------------------------------------

namespace _10 {

template <typename, typename, typename = std::void_t<>>
struct HasLess : std::false_type
{};

template <typename T0, typename T1>
struct HasLess<T0, T1, std::void_t<decltype(std::declval<T0>() < std::declval<T1>())>> : std::true_type
{};

void test()
{
    std::cout << HasLess<std::string, std::string>::value << "\n";
    std::cout << HasLess<A, B>::value << "\n";
}

} //_10--------------------------------------------------------------

namespace _11 {

using namespace _4;

constexpr auto hasFirst = isValid([](auto x) -> decltype((void)valueT(x).first){});

struct C {
    using size_type = std::size_t;
};

void test()
{
    std::cout << hasFirst(type<std::pair<int,int>>) << "\n";
}

} //_11--------------------------------------------------------------

namespace _12 {

using namespace _4;

constexpr auto hasFirst = isValid([](auto&& x) -> decltype((void)&x.first){
});

template <typename T>
using HasFirst = decltype(hasFirst(std::declval<T>()));

constexpr auto hasSizeType = isValid([](auto&& x) -> typename std::decay_t<decltype(x)>::size_type{
});

template <typename T>
using HasSizeType = decltype(hasSizeType(std::declval<T>()));

constexpr auto hasLess = isValid([](auto&& x, auto&& y) -> decltype(x < y){
});

template <typename T0, typename T1>
using HasLess = decltype(hasLess(std::declval<T0>(), std::declval<T1>()));

void test()
{
    std::cout << HasFirst<std::pair<int,int>>::value << "\n";
}

} //_12--------------------------------------------------------------

} //namespace =================================================================

void test_ch_19_sfinae_traits()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
    _4::test_default_constructalbe();
    _4::test_has_first();
    _6::test();
    _7::test();
    _8::test();
    _9::test();
    _10::test();
#endif

    _11::test();
}
