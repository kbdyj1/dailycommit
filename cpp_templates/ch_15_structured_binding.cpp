#include <iostream>
#include <string>
#include <utility>
#include <tuple>

namespace { //=================================================================

namespace _1 {

struct X {
    int id;
    std::string name;
};

X f()
{
    return {0, "Qt"};
}

void test()
{
    const auto&& [id, name] = f();
    std::cout << "[" << id << ", " << name << "] = f()\n";

    const auto&& e = f();
    std::cout << "e.id: " << e.id << ", e.name: " << e.name << "\n";

    double pt[3];
    auto& [x, y, z] = pt;
}

} //_1 --------------------------------------------------------------

namespace _2 {

enum M
{};

#if (0)
template <>
class std::tuple_size<M> {
public:
    static const unsigned value = 2;
};

template <>
class std::tuple_element<0, M> {
public:
    using type = int;
};

template <>
class std::tuple_element<1, M> {
public:
    using type = double;
};
#endif

template <int>
auto get(M);

template <>
auto get<0>(M)
{
    return 10;
}

template <>
auto get<1>(M)
{
    return 20.0;
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_structured_binding()
{
    _1::test();
}
