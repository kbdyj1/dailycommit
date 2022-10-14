#include <iostream>
#include <vector>

int V = 10;

namespace { //=================================================================

namespace _1 {

template <typename Container>
void use(const Container& container)
{
    auto iter = container.begin();
    while (iter != container.end()) {
        auto& elem = *iter++;
    }
}

template <typename T>
void deducePos(T pos)
{}

template <typename T>
void deduceElement(T& elem)
{}

void test()
{
    const auto N = 32U;
    auto* p = (void*)nullptr;
}
} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T, T V>
struct S
{};

template <auto V>
struct A
{
    using ArgType = decltype(V);

    A() {
        std::cout << "A::A() " << typeid(V).name() << "\n";
    }
};

void test()
{
    auto s = S<int, 16>{};

    auto a = A<8u>{};
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename>
struct PMClassT;

template <typename C, typename M>
struct PMClassT<M C::*> {
    using Type = C;
};

template <typename PM>
using PMClass = typename PMClassT<PM>::Type;

template <auto PMD>
struct CounterHandle {
    PMClass<decltype(PMD)>& c;
    CounterHandle(PMClass<decltype(PMD)>& c) : c(c)
    {}

    void inc() {
        ++(c.*PMD);

        std::cout << "CounterHandle::inc() -> " << c.*PMD << "\n";
    }
};

struct S {
    int i;
};

template <auto... Vs>
struct Values
{};

template <auto V, decltype (V)... Vs>
struct HomogeneousValues
{};

void g(std::string&& s)
{
    std::cout << "std::is_lvalue_reference_v<decltype(s)>" << std::is_lvalue_reference_v<decltype(s)> << "\n";
    std::cout << "std::is_rvalue_reference_v<decltype(s)>" << std::is_rvalue_reference_v<decltype(s)> << "\n";
    std::cout << "std::is_same_v<decltype(s), std::string&>" << std::is_same_v<decltype(s), std::string&> << "\n";
    std::cout << "std::is_same_v<decltype(s), std::string&&>" << std::is_same_v<decltype(s), std::string&&> << "\n";
    std::cout << "std::is_lvalue_reference_v<decltype((s))>" << std::is_lvalue_reference_v<decltype((s))> << "\n";
    std::cout << "std::is_rvalue_reference_v<decltype((s))>" << std::is_rvalue_reference_v<decltype((s))> << "\n";
    std::cout << "std::is_same_v<decltype((s)), std::string&>" << std::is_same_v<decltype((s)), std::string&> << "\n";
    std::cout << "std::is_same_v<decltype((s)), std::string&&>" << std::is_same_v<decltype((s)), std::string&&> << "\n";
}

void test()
{
    auto s = S{16};

#if 201402L < __cplusplus //c++17
    auto handle = CounterHandle<&S::i>(s);
#else
    auto handle = CounterHandle<int S::*, &S::i>(s);
#endif

    handle.inc();

    auto v0 = Values<1, 2, 3>{};
    auto v1 = Values<1, 'x', nullptr>{};
    auto v2 = HomogeneousValues<1, 2, 3>{};
#if (0)
    auto v3 = HomogeneousValues<>{};
#endif

    g("Hello, Qt");
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test_decltype_auto()
{
    auto i = 10;
    const auto& r = i;
    auto x = r;
    decltype (auto) y = r;

    std::cout << "x: " << typeid(x).name() << ", y: " << typeid(y).name() << "\n";

    auto v = std::vector<int>{1};
    auto elem0 = v[0];  //int
    decltype (auto) elem1 = v[0];   //int&

    int k;
    decltype (auto) kx = k; //int
    decltype (auto) ky = (k);   //int&
}

template <decltype (auto) Val>
class S
{};

template <auto N>
struct A
{};

template <auto N>
int f(A<N> p)
{
    return N;
}

template <auto V>
int g(decltype (V) p)
{}

void test()
{
    constexpr int C = 10;

    auto s0 = S<C>{};   //S<10>
    auto s1 = S<(V)>{}; //S<(int&)>

    auto x = A<20>{};
    auto r = f(x);

    std::cout << "f(A<20>): " << r << "\n";

    auto r0 = g<30>(30);
#if (0)
    auto r1 = g(30);
#endif
}

} //_4 --------------------------------------------------------------

namespace _5 {

//#define USE_INITIALIZE_LIST

template <typename T>
void deduce(T)
{}

#if defined(USE_INITIALIZE_LIST)
template <typename T>
void deduce(std::initializer_list<T>)
{}
#endif

#if (0)
auto subtleError()
#else
auto subtleError() -> std::initializer_list<int>    // can't refer
#endif
{
    return {1, 2, 3};
}

void test()
{
#if defined(USE_INITIALIZE_LIST)
    deduce({1, 2, 3});
    deduce({1});
#else
    auto arg = {1, 2, 3};
    deduce(arg);
#endif

#if (0)
    auto oops {1, 2, 3};
#endif
    auto val {2};

    auto list = subtleError();
    for (auto i : list) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    char c;
    auto* cp = &c, d = c;
#if (0)
    auto e = c, f = c+1;    //e:char, f:int
#endif
}

auto f(int n)
{
#if (0)
    if (1 < n) {
        return n * f(n-1);
    } else {
        return 1;
    }
#else
    if (n <= 1) {
        return 1;
    } else {
        return n * f(n-1);
    }
#endif
}

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u)
{
    return t + u;
}

void add(...)
{
    std::cout << "add(...)\n";
}

template <typename T, typename U>
auto add2(T t, U u) -> decltype (auto)
{
    return t + u;
}

void add2(...)
{
    std::cout << "add2(...)\n";
}

struct X
{};

using Result = decltype(add(X{}, X{}));
#if (0)
using Result2 = decltype(add2(X{}, X{}));
#endif
}

} //namespace =================================================================

void test_ch_15_auto()
{
#if (0)
    _2::test();
    _3::test();
    _4::test();
#endif

    _5::test();
}
