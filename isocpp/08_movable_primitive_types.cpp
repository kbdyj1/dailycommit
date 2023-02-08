#include <iostream>
#include <utility>
#include <vector>

namespace { //=================================================================

namespace _1 {

template <typename T, T EmptyValue>
struct MovablePrimitive {
    T value = EmptyValue;

    MovablePrimitive() = default;
    MovablePrimitive(const T& init) : value(init)
    {}
    MovablePrimitive(const MovablePrimitive& other) = default;
    MovablePrimitive(MovablePrimitive&& other) : value(std::exchange(value, other.value))
    {}
    MovablePrimitive& operator=(const MovablePrimitive& other) = default;
    MovablePrimitive& operator=(MovablePrimitive&& other)
    {
        value = std::exchange(other.value, EmptyValue);
        return *this;
    }
    operator T() { return value; }
};

enum class Flavor {
    None,
    Vanilla,
    Chocolate
};

struct FlavorVector {
    std::vector<int> v;
    MovablePrimitive<Flavor, Flavor::None> flavor;
};

void test()
{
    MovablePrimitive<int, 0> v;
    v = 64;

    std::cout << v.value << '\n';

    auto v1 = std::move(v);

    std::cout << v.value << '\n';
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_movable_primitive_types()
{
    _1::test();
}
