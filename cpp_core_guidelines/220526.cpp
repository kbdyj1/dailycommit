#include <iostream>
#include <iterator>
#include <string>

using namespace std::literals;

namespace { //=================================================================

// T.10 Specify concepts for all template arguments

template <typename Iter, typename Value>
requires
    std::input_iterator<Iter> &&
    std::equality_comparable_with<std::iter_value_t<Iter>, Value>
Iter find(Iter b, Iter e, Value v)
{
    for (Iter iter = b; iter != e; iter++) {
        if (*iter == v)
            return iter;
    }
    return e;
}

template <std::input_iterator Iter, typename Value>
requires
    std::equality_comparable_with<std::iter_value_t<Iter>, Value>
Iter find2(Iter b, Iter e, Value v)
{
    for (Iter iter = b; iter != e; iter++) {
        if (*iter == v)
            return iter;
    }
    return e;
}

// T.11 Whenever possible use standard concepts

#if (0)
template <typename T>
concept OrderedContainer = Sequence<T> && RandomAccess<Iterator<T>> && Ordered<ValueType<T>>;

void sort(OrderedContainer);
#else
void sort(std::sortable auto& s)
{}
#endif

// T.12 Preper the shorthand notation for simple, single-type argument concepts

template <typename T>
void sort2(T&);

template <std::sortable T>
void sort3(T&);

void sort4(std::sortable auto&);

// Concept definition rules

// T.20 Avoid "concepts" without meaningful semantics

//#define ENABLE_STRING_CONCATENATE

namespace t20 {

template <typename T>
#if defined(ENABLE_STRING_CONCATENATE)
concept Addable = requires(T a, T b) { a+b; };
#else
concept Addable = requires(T a, T b) { a+b; a-b; a*b; a/b; };
#endif

template <Addable N>
auto func(const N& a, const N& b)
{
    return a + b;
}

} //t20

// T.21 Require a complete set of operations for a concept
//      arithmetic: +, -, *, /, +=, -=, *=, /=
//      comparable: <, >, =, !=, <=, >=

// T.22 Specify axioms for concepts

template <typename T>
concept Number = requires(T a, T b) {
    {a+b} -> std::convertible_to<T>;
    {a-b} -> std::convertible_to<T>;
    {a*b} -> std::convertible_to<T>;
    {a/b} -> std::convertible_to<T>;
};

// T.23 Differentiate a refined concept from its more general case by adding new use patterns.

template <typename I>
concept InputIterator = requires(I iter) { ++iter; };

template <typename I>
concept ForwardIterator = InputIterator<I> && requires(I iter) { iter++; };

// T.24 Use tag classes or traits to diffentiate concepts that differ only in semantics

template <typename I>
concept RandomAccessIterator =
    std::bidirectional_iterator<I> &&
    //std::derived_from<<I>, std::random_access_iterator_tag> &&
    std::totally_ordered<I> &&
    std::sized_sentinel_for<I, I> &&
    requires(I i, const I j, std::iter_difference_t<I> n) {
        { i += n } -> std::same_as<I&>;
        { j +  n } -> std::same_as<I>;
        { n +  j } -> std::same_as<I>;
        { i -= n } -> std::same_as<I&>;
        { j -  n } -> std::same_as<I>;
        { j[n]   } -> std::same_as<std::iter_reference_t<I>>;
    };

//template <typename I>
//concept ContiguousIterator =
//    RandomAccessIterator<I> &&
//    std::is_contiguous_v<I>;

// T.25 Avoid complementary constraints

namespace t25{

template <typename T>
concept C = requires(T t) {
    {t} -> std::same_as<T>;
};

#if (0)
namespace bad {
template <typename T>
requires !C<T>
void f()
{}

}// bad
#else
template <typename T>
void f()
{
    std::cout << "f()" << std::endl;
}
#endif

template <typename T>
requires C<T>
void f()
{
    std::cout << "C<T> f()" << std::endl;
}

} //t25

} //===========================================================================

void test_t_20()
{
#if defined(ENABLE_STRING_CONCATENATE)
    auto s0 = "Hello, "s;
    auto s1 = "Qt6"s;

    auto res = t20::func(s0, s1);

    std::cout << "s0 + s1 : " << res << std::endl;
#endif

    auto x = 2;
    auto y = 3;
    auto z = t20::func(x, y);

    std::cout << "x + y : " << z << std::endl;
}

void test_t_25()
{
    t25::f<int>();
}
