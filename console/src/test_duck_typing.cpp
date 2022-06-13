#include <iostream>

namespace { //=================================================================

class Duck {
public:
    void quack() {
        std::cout << "Duck::quack()\n";
    }
    void feathers() {
        std::cout << "Duck::feathers()\n";
    }
};

class Person {
public:
    void quack() {
        std::cout << "Person::quack()\n";
    }
    void feathers() {
        std::cout << "Person::feathers()\n";
    }
};

template <typename T>
void inTheRiver(T& t)
{
    t.quack();
    t.feathers();
}

} // namespace ================================================================

void test_duck_typing()
{
    auto duck = Duck{};
    inTheRiver(duck);

    auto jason = Person{};
    inTheRiver(jason);
}
