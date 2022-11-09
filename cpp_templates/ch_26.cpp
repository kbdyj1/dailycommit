//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.26 Variant
//
//  22/11/08
//=============================================================================

#include <iostream>
#include <type_traits>
#include <variant>

namespace { //=================================================================

namespace _1 {

#if (0)
template <typename V, typename Head, typename... Tail>
void printImpl(const V& v)
{
    if (v.template is<Head>()) {
        std::cout << v.template get<Head>();
    } else if constexpr (0 < sizeof...(Tail)) {
        printImpl<V, Tail...>(v);
    }
}

template <typename... Types>
void print(const Variant<Types...>& v)
{
    printImpl<Variant<Types...>, Types...>(v);
}
#endif

void test()
{
//    Variant<int, short, float, double> v(1.5);

//    print(v);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    std::common_type_t<int, double> v;

    std::cout << typeid(v).name() << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    auto v0 = std::variant<int, double>();
    v0 = 2.0;
    std::cout << v0.index() << "\n";

    v0 = 1;
    std::cout << v0.index() << "\n";
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_26()
{
#if (0)
    _1::test();
    _2::test();
#endif

    _3::test();
}
