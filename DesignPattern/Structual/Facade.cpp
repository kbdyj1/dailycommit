#include <iostream>

namespace { //=================================================================

struct Engine {
    void start() {
        std::cout << "Engine start !!!" << "\n";
    }
};
struct HeadLight {
    void turnOn() {
        std::cout << "Headlight turn on !!!" << "\n";
    }
};

class Car {
    Engine engine;
    HeadLight headlight;

public:
    void start() {
        std::cout << "Car::start !!!" << "\n";
        engine.start();
        headlight.turnOn();
    }
};

} //namespace =================================================================

void test_facade()
{
    auto car = Car{};
    car.start();
}
