#include <iostream>
#include <vector>
#include <numeric>

namespace { //=================================================================

namespace _1 {

#define REMOVE_QUALIFIER

template <typename T>
using contained_type_t =
#if defined(REMOVE_QUALIFIER)
        std::remove_cv_t<
        std::remove_reference_t<
#endif
            decltype(*std::begin(T()))
#if defined(REMOVE_QUALIFIER)
        >>
#endif
        ;

template <typename C, typename R = contained_type_t<C>>
R sum(const C& collection)
{
#if (0)
    auto ret = R{};
    for (auto iter = collection.begin(); iter != collection.end(); iter++) {
        ret += *iter;
    }
    return ret;
#else
    return std::accumulate(std::begin(collection), std::end(collection), R{});
#endif
}

#if (0)
template <typename T>
class Error;

//24:50: error: invalid declarator before ‘(’ token
//   24 | Error<contained_type_t<std::vector<std::string>>>();
//      |                                                  ^
Error<contained_type_t<std::vector<std::string>>>();
#else
//41:86: error: static assertion failed
//   41 | static_assert(std::is_same<contained_type_t<std::vector<std::string>>, std::string>::value);
//      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~
//In instantiation of ‘R {anonymous}::_1::sum(const C&) [with C = std::vector<int>; R = const int&]’:
static_assert(std::is_same<contained_type_t<std::vector<std::string>>, std::string>::value);
#endif

void test()
{
    auto vec = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto result = sum(vec);
    std::cout << "sum(vec): " << result << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T1, typename T2>
struct is_same : std::false_type
{};

template <typename T>
struct is_same<T, T> : std::true_type
{};

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

void test()
{
    std::cout << "is_same<int, int>: " << is_same<int, int>::value << "\n";
    std::cout << "is_same<int, double>: " << is_same<int, double>::value << "\n";

    std::cout << is_same<int, remove_reference_t<int&>>::value << "\n";
    std::cout << is_same<int, remove_reference_t<int&&>>::value << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T, typename E>
class Expected
{
public:
    using value_type = T;
};

template <typename...>
using void_t = void;

template <typename C, typename = void_t<>>
struct has_value_type : std::false_type
{};

template <typename C>
struct has_value_type<C, void_t<typename C::value_type>> : std::true_type
{};

template <typename C, typename = void_t<>>
struct is_iterable : std::false_type
{};

template <typename C>
struct is_iterable<C, void_t<decltype(*std::begin(std::declval<C>())),
                             decltype(*std::end(std::declval<C>()))>
    >
    : std::true_type
{};

} //_3 --------------------------------------------------------------

} //===========================================================================

void test_ch_11()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
#endif
    _2::test();
}
