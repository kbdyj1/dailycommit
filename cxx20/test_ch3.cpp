#include <concepts>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <random>
#include <unordered_set>

#define SHOW(...)   \
    std::cout   << #__VA_ARGS__ << " : "\
                << std:: __VA_ARGS__    \
                << std::endl;

namespace
{

void test_sort()
{
    auto l = std::list<int>{ 0, 10, 2, 13, 40, 25 };
#if (0)
    std::sort(l.begin(), l.end());
#endif
}

template <typename T>
concept integral = std::is_integral_v<T>;

template <typename T>
requires integral<T>
auto gcd(T a, T b) {
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

template <typename T>
auto gcd1(T a, T b) requires integral<T>
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

template <integral T>
auto gcd2(T a, T b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// abbreviated function templates syntax
integral auto gcd3(integral auto a, integral auto b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// abbreviated function template synatax
auto gcd4(auto a, auto b)
{
    if (b == 0)
        return a;
    else
        return gcd4(b, a % b);
}

template <unsigned int i>
requires (i <= 20)
int sum(int j) {
    return i + j;
}

void test_sum()
{
    std::cout << "sum<20>(200) : " << sum<20>(200) << std::endl;

#if (0)
    //    ../cxx20/test_ch3.cpp:57:5: note: candidate template ignored: constraints not satisfied [with i = 21]
    //    int sum(int j) {
    //        ^
    //    ../cxx20/test_ch3.cpp:56:11: note: because '21U <= 20' (21 <= 20) evaluated to false
    //    requires (i <= 20)
    //              ^
    std::cout << "sum<21>(200) : " << sum<21>(200) << std::endl;
#endif
}

/******************************************************************************
 * overload
 */
void overload(auto t)
{
    std::cout << "auto : " << t << std::endl;
}

void overload(integral auto t)
{
    std::cout << "integral : " << t << std::endl;
}

void overload(long t)
{
    std::cout << "long : " << t << std::endl;
}

void test_overload()
{
    std::cout << std::endl;

    overload(3.14);
    overload(2010);
    overload(2020L);

    std::cout << std::endl;
}

/******************************************************************************
 * same_as
 */
template <typename T, typename ... U>
concept IsAnyOf = (std::same_as<T, U> || ...);

template <typename T>
concept IsPrintable = std::integral<T> || std::floating_point<T> ||
        IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, char, wchar_t>;

void println(IsPrintable auto const ... args)
{
    (std::wcout << ... << args) << std::endl;
}

void test_same_as()
{
    println("Example: ", 3.14, " : ", 42, " : [", 'a', L'-', L"Z]");
}

/******************************************************************************
 * derived_from
 */
namespace derived_from
{
class A{};
class B : public A{};
class C : private A{};
class D{};

void test_derived_from()
{
    static_assert(std::derived_from<B, B> == true);
    static_assert(std::derived_from<int, int> == false);
    static_assert(std::derived_from<B, A> == true);
    static_assert(std::derived_from<C, A> == false);
#if (0)
    static_assert(std::is_base_of<B, B>::value == true);
    static_assert(std::is_base_of<int, int>::value == false);
    static_assert(std::is_base_of<A, B>::value == true);
    static_assert(std::is_base_of<A, C>::value == true);
#else
    std::cout << std::boolalpha;
    SHOW( is_base_of_v<A, A> );
    SHOW( is_base_of_v<A, B> );
    SHOW( is_base_of_v<A, C> );
    SHOW( is_base_of_v<A, D> );
    SHOW( is_base_of_v<B, A> );
    SHOW( is_base_of_v<int, int> );
#endif
}
} // derived_from

/******************************************************************************
 * derived_from
 */
namespace convertible_to
{
class A{};
class B : public A{};
class C : private A{};
class D{};

void test_convertible_to()
{
    std::cout << std::boolalpha;
    SHOW( convertible_to<A, B> );
    SHOW( convertible_to<A, C> );
    SHOW( convertible_to<A, D> );
    SHOW( convertible_to<B, A> );
    SHOW( convertible_to<C, A> );
    SHOW( convertible_to<D, A> );
    SHOW( convertible_to<int, double> );
    SHOW( convertible_to<int, bool> );
    SHOW( convertible_to<bool, int> );
    SHOW( convertible_to<const char*, std::string> );
}
} // convertible_to

/******************************************************************************
 * common_reference_with
 */
template <typename T, typename U>
concept common_reference_with =
        std::same_as<std::common_reference_t<T, U>, std::common_reference_t<U, T>> &&
        std::convertible_to<T, std::common_reference_t<T, U>> &&
        std::convertible_to<U, std::common_reference_t<T, U>>;

template <typename T, typename U>
requires common_reference_with<T, U>
void f()
{
    std::cout << "T, U share a common reference type." << std::endl;
}

template <typename T, typename U>
void f()
{
    std::cout << "T, U does not share a common reference type." << std::endl;
}

void test_common_reference_with()
{
    f<std::size_t&, int&>();
    f<std::string&, std::string_view&>();
    f<std::vector<int>, std::vector<int>&>();
    f<std::vector<int>, std::vector<double>>();
    f<std::pair<int&, double&>, std::pair<int, double>>();
}

template <typename T, typename U>
requires std::common_with<T, U>
void g()
{
    std::cout << "T, U share a common type" << std::endl;
}

template <typename T, typename U>
void g()
{
    std::cout << "T, U does not share a common type" << std::endl;
}

void test_common_with()
{
    g<std::size_t&, int&>();
    g<std::string&, std::string_view&>();
    g<std::vector<int>, std::vector<int>&>();
    g<std::vector<int>, std::vector<double>>();
    g<std::pair<int&, double&>, std::pair<int, double>>();
}

} // namespace

namespace assignable_from
{
template <typename T, typename U>
requires std::assignable_from<T, U>
void h(const char *name, const char *arg)
{
    std::cout << name << " is assignable from " << arg << std::endl;
}

template <typename T, typename U>
void h(const char *name, const char *arg)
{
    std::cout << name << " is not assignable from " << arg << std::endl;
}

struct S
{
    S& operator=(const S&) = default;
    S& operator=(S&&) = delete;
};

void test_assign_from()
{
    h<int&, short>("int&", "short");
    h<std::vector<int>&, std::vector<int>>("std::vector<int>&", "std::vector<int>");
    h<std::unique_ptr<int>&, std::unique_ptr<int>>("std::unique_ptr<int>&", "std::unique_ptr<int>");
    h<S&, S&>("S&", "S&");
    h<std::unique_ptr<int>&, std::unique_ptr<int>&>("std::unique_ptr<int>&", "std::unique_ptr<int>&");
    h<S&, S>("S&", "S");
}
} // assignable_from

/******************************************************************************
 * swappalbe
 */
namespace swappable
{

template <typename T>
requires std::swappable<T>
void f(const char *name)
{
    std::cout << name << " is swappable" << std::endl;
}

template <typename T>
void f(const char *name)
{
    std::cout << name << " is not swappable" << std::endl;
}

struct Swappable1
{
    Swappable1(int m) : n(m)
    {}
    Swappable1(Swappable1 &&) = delete;

    int n;
};
void swap(Swappable1 &lhs, Swappable1 &rhs)
{
    std::swap(lhs.n, rhs.n);
}

struct Swappable2
{
    Swappable2(double v) : d(v)
    {}
    Swappable2(Swappable2&&) = delete;

    double d;

    friend void swap(Swappable2& lhs, Swappable2& rhs) {
        std::swap(lhs.d, rhs.d);
    }
};

struct NotSwappalbe
{
    NotSwappalbe(NotSwappalbe &&) = delete;
    NotSwappalbe& operator==(NotSwappalbe&&) = delete;
};

void test_swappable()
{
    // OK
    f<int>("int");
    f<int&&>("int&&");
    f<Swappable1>("Swappable1");
    f<Swappable2>("Swappable2");
    f<int[5]>("int[5]");

    std::cout << std::endl;

    // NG
    f<const int>("const int");
    f<NotSwappalbe>("NotSwappable");
}

} // swappable

/******************************************************************************
 * is_object
 */
namespace is_object
{
class A{};

void test_is_object()
{
    std::cout << std::boolalpha;
    SHOW( is_object_v<int>  );
    SHOW( is_object_v<int&> );
    SHOW( is_object_v<A>    );
    SHOW( is_object_v<A&>   );
}
} // is_object

/******************************************************************************
 * movable concepts
 */
namespace movable
{

template <std::movable T>
void f(const char *name)
{
    std::cout << name << " is movable" << std::endl;
}

template <typename T>
void f(const char *name)
{
    std::cout << name << " is not movable" << std::endl;
}

struct Movable
{
    Movable(Movable&&) = default;
    Movable& operator=(Movable&&) = default;
};

struct NotMovable1
{
    NotMovable1(NotMovable1&&) = delete;
};

struct NotMovable2
{
    NotMovable2& operator=(NotMovable2&&) = delete;
};

void test_movable()
{
    // OK

    f<int>("int");
    f<double>("double");
    f<std::nullptr_t>("std::nullptr_t");
    f<std::size_t>("std::size_t");
    f<Movable>("Movable");

    std::cout << std::endl;

    // NG

    f<void>("void");
    f<NotMovable1>("NotMovable1");
    f<NotMovable2>("NotMovable2");
}

} // movable

/******************************************************************************
 * invocable
 */
namespace invocable
{

template <typename F, typename... Args>
requires std::invocable<F, Args...>
void f(const char *name)
{
    std::cout << name << " is invocalbe" << std::endl;
}

template <typename F, typename... Args>
void f(const char *name)
{
    std::cout << name << " is not invocalbe" << std::endl;
}

void func(int);
auto lambda = [](auto a) { return a; };
auto mut_lambda = [n = 0](auto a) mutable { ++n; return n + a; };

struct Invocable
{
    template <typename T>
    void operator()(T&& t) const {
        return t;
    }
};

struct NotInvocable
{};

void test_invocable()
{
    // OK

    f<decltype(func), int>("func(int)");
    f<decltype(lambda), int>("lambda(int)");
    f<decltype(lambda), int*>("lambda(int*)");
    f<Invocable, int>("Invocable(int)");
    f<Invocable, int***>("Invocable(int***)");

    f<decltype(mut_lambda), int>("mut_lambda(int)");
    f<std::mt19937>("std::mt19937()");

    std::cout << std::endl;

    // NG

    f<decltype(func), int*>("func(int*)");
    f<NotInvocable>("NotInvocable()");
    f<NotInvocable, int>("NotInvocalbe(int)");
}

} // invocable

/******************************************************************************
 * predicate
 */
namespace predicate
{

template <typename F, typename... Args>
requires std::predicate<F, Args...>
void f(const char *name)
{
    std::cout << name << " is predicate" << std::endl;
}

template <typename F, typename... Args>
void f(const char *name)
{
    std::cout << name << " is not predicate" << std::endl;
}

bool func1(int);
int func2(int);
int* func3(int);
auto lambda = [](auto a) { return a < 10; };

struct Predicate {
    bool operator()(int n) const {
        return n < 10;
    }
};

struct NotPredicate {
    void operator()(int)
    {}
};

void test_predicate()
{
    f<decltype(func1), int>("func1(int)");
    f<decltype(func2), int>("func1(int)");
    f<decltype(func3), int>("func1(int)");
    f<decltype(lambda), int>("lambda(int)");
    f<std::mt19937>("std::mt19937()");
    f<Predicate, int>("Predicate(int)");

    std::cout << std::endl;

    f<NotPredicate, int>("NotPredicate(int)");
}

} // predicate

/******************************************************************************
 * custom
 */
namespace custom
{

template <typename T>
concept Addable = requires (T a, T b) {
    a + b;
};

template <Addable T>
T tadd(T a, T b) {
    return a + b;
}

template <typename>
struct Other;

template <>
struct Other<std::vector<int>>
{};

template <typename T>
concept TypeRequirement = requires {
        typename T::value_type;
        typename Other<T>;
};

void test_type_requirement()
{
    TypeRequirement auto vec = std::vector<int>{1, 2, 3};
}

// compound requirement
template <typename T>
concept Equal = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

bool isEqual(Equal auto a, Equal auto b)
{
    return a == b;
}

struct WithoutEqual {
    bool operator==(const WithoutEqual&) = delete;
};

struct WithoutNotEqual {
    bool operator!=(const WithoutNotEqual&) = delete;
};

void test_compound_requirement()
{
    std::cout << std::boolalpha;
    std::cout << "isEqual(1, 5) : " << isEqual(1, 5) << std::endl;

#if (0)
    WithoutEqual we0, we1;

    //note: because 'a == b' would be invalid: overload resolution selected deleted operator '=='
    //{ a == b } -> std::convertible_to<bool>;
    isEqual(we0, we1);
#endif
}

// nested requirement
template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

template <typename T>
concept UnsignedIntegral = Integral<T> &&
requires(T) {
    requires !SignedIntegral<T>;
};

void test_nested_requirement()
{
    UnsignedIntegral auto n = 5u;
}

template <typename T>
concept Ordering =
    Equal<T> &&
    requires (T a, T b) {
        { a <= b } -> std::convertible_to<bool>;
        { a < b } -> std::convertible_to<bool>;
        { a > b } -> std::convertible_to<bool>;
        { a >= b } -> std::convertible_to<bool>;
};

template <Ordering T>
T getSmaller(const T& a, const T& b) {
    return (a < b) ? a : b;
}

void test_ordering()
{
    std::cout << std::boolalpha;
    std::cout << "getSmaller(1, 5) : " << getSmaller(1, 5) << std::endl;
    std::unordered_set<int> set0{1, 2, 3, 4, 5};
    std::unordered_set<int> set1{5, 4, 3, 2, 1};

    std::cout << "isEqual(set0, set1) : " << isEqual(set0, set1) << std::endl;
}

} // custom

/*=============================================================================
 * M A I N
 */
void test_ch3()
{
    //test_sort();
    //test_sum();
    //test_overload();
    //test_same_as();
    //derived_from::test_derived_from();
    //convertible_to::test_convertible_to();
    //test_common_reference_with();
    //test_common_with();
    //assignable_from::test_assign_from();
    //swappable::test_swappable();
    //is_object::test_is_object();
    //movable::test_movable();
    //invocable::test_invocable();
    //predicate::test_predicate();
    //custom::test_type_requirement();
    //custom::test_compound_requirement();
    custom::test_ordering();
}
