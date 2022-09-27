#include <iostream>
#include <array>

namespace { //=================================================================

namespace legacy {

template <unsigned p, unsigned d>
struct IsPrimeImpl {
    static constexpr bool value = (p%d != 0) && IsPrimeImpl<p, d-1>::value;
};

template <unsigned p>
struct IsPrimeImpl<p, 2> {
    static constexpr bool value = (p%2 != 0);
};

template <unsigned p>
struct IsPrime {
    static constexpr bool value = IsPrimeImpl<p, p/2>::value;
};

template <>
struct IsPrime<0> {
    static constexpr bool value = false;
};
template <>
struct IsPrime<1> {
    static constexpr bool value = false;
};
template <>
struct IsPrime<2> {
    static constexpr bool value = true;
};
template <>
struct IsPrime<3> {
    static constexpr bool value = true;
};

} //legacy ----------------------------------------------------------

namespace cxx11 {

constexpr bool IsPrimeImpl(unsigned p, unsigned d)
{
    return d != 2 ? (p%d != 0) && IsPrimeImpl(p, d-1)
                  : (p%2 != 0);
}
constexpr bool IsPrime(unsigned p)
{
    return p < 4 ? !(p<2) : IsPrimeImpl(p, p/2);
}

} //cxx11 -----------------------------------------------------------

namespace cxx14 {

constexpr bool IsPrime(unsigned p)
{
    for (unsigned int d=2; d<=p/2; d++) {
        if (p % d == 0)
            return false;
    }
    return p > 1;
}

} //cxx14 -----------------------------------------------------------

bool Is57isPrime()
{
    using namespace cxx14;

    return IsPrime(57);
}

void test_prime() {

#if (0)
    using namespace legacy;

    std::cout << "IsPrime<9>::value: " << IsPrime<9>::value << "\n";
    std::cout << "IsPrime<7>::value: " << IsPrime<7>::value << "\n";
#else
    #if (0)
        using namespace cxx11;
    #else
        using namespace cxx14;
    #endif

    constexpr auto b0 = IsPrime(9);
    const auto b1 = IsPrime(7);

    std::cout << "IsPrime(9): " << b0 << "\n";
    std::cout << "IsPrime(7): " << b1 << "\n";

    std::cout << "Is57isPrime(): " << Is57isPrime() << "\n";
#endif
}

template <unsigned N, bool = cxx14::IsPrime(N)>
struct Helper;

template <unsigned N>
struct Helper<N, false> {
    static void print() { std::cout << "Helper<" << N << ", false>\n"; }
};

template <unsigned N>
struct Helper<N, true> {
    static void print() { std::cout << "Helper<" << N << ", true>\n"; }
};

void test_select_helper()
{
    auto h0 = Helper<55>{};
    h0.print();

    auto h1 = Helper<53>{};
    h1.print();
}

template <typename T, std::size_t N>
void foo(const std::array<T, N>& arr)
{
    if constexpr(cxx14::IsPrime(N)) {
        std::cout << "foo(true)\n";
    } else {
        std::cout << "foo(false)\n";
    }
}

void test_array()
{
    auto a0 = std::array<int, 8>{ 1, 2, 3, 4, 5, 6, 7, 8 };
    foo(a0);

    auto a1 = std::array<int, 5>{ 5, 4, 3, 2, 1 };
    foo(a1);
}

} //namespace =================================================================

void test_ch_08_meta_programming()
{
    std::cout << std::boolalpha;

#if (0)
    test_prime();
    test_select_helper();
#endif

    test_array();
}
