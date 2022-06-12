#include <iostream>

namespace { //=================================================================

template <typename T>
struct Property
{
    T value;

    Property(const T initialValue) : value(initialValue)
    {}

    operator T() {
        return value;
    }
    T operator=(T other) {
        return value = other;
    }
};

struct Creature {
    Property<std::string> name{"unnamed"};
    Property<int> power{10};
    Property<int> armor{100};
};

} //namespace =================================================================

void test_proxy()
{
    auto creature = Creature();
    creature.armor = 120;

    std::cout << "creature.power: " << creature.power << "creature.armor: " << creature.armor;
}
