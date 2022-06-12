#include <iostream>

namespace { //=================================================================

class Vehicle {
public:
    void start() {
        engineStart();
        lcdOn();
        displayHomeScreen();
    }

protected:
    virtual void engineStart() = 0;
    virtual void lcdOn() = 0;
    virtual void displayHomeScreen() = 0;
};

class Suv : public Vehicle {
public:
    void engineStart() override {
        std::cout << "Suv::engineStart()\n";
    }
    void lcdOn() override {
        std::cout << "Suv::lcdOn\n";
    }
    void displayHomeScreen() override {
        std::cout << "Suv::displayHomeScreen()\n";
    }
};

class Ev : public Vehicle {
public:
    void engineStart() override {
        std::cout << "Ev::engineStart()\n";
    }
    void lcdOn() override {
        std::cout << "Ev::lcdOn\n";
    }
    void displayHomeScreen() override {
        std::cout << "Ev::displayHomeScreen()\n";
    }
};

} //namespace =================================================================

void test_template_method()
{
    auto suv = Suv();
    suv.start();

    auto ev = Ev();
    ev.start();
}
