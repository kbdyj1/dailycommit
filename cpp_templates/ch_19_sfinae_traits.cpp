#include <iostream>

namespace { //=================================================================

struct A {
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

template <typename T>
struct TypeT {
    using Type = T;
};

template <typename T>
constexpr auto type = TypeT<T>::Type;

template <typename T>
T valueT(TypeT<T>);

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_19_sfinae_traits()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
