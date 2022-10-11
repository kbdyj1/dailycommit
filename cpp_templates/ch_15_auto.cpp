#include <iostream>

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

template <typename T>
struct PMClassT
{};

template <typename C, typename M>
struct PMClassT<M C::*> {
    using Type = C;
};

template <typename PM>
using PMClass = typename PMClassT<PM>::Type;

void test()
{
    auto s = S<int, 16>{};

    auto a = A<8u>{};
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_auto()
{
    _2::test();
}
