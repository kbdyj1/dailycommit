#include <iostream>
#include <vector>

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
#endif

    _4::test_has_first();
}
