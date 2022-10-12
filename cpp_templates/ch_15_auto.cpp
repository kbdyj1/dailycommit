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

void test()
{
    constexpr int C = 10;

    auto s0 = S<C>{};   //S<10>
    auto s1 = S<(V)>{}; //S<(int&)>
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_auto()
{
#if (0)
    _2::test();
    _3::test();
#endif

    _4::test();
}
