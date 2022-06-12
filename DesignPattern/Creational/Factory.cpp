#include <iostream>
#include <cmath>
#include <memory>
#include <map>
#include <functional>

namespace { //=================================================================

//-------------------------------------------------------------------
//  Factory method
//-------------------------------------------------------------------
class Point {
    float x;
    float y;

    friend class Factory;

protected:
    Point(float x, float y) : x{x}, y{y}
    {}

public:
    static Point* newCartesian(float x, float y) {
        return new Point(x, y);
    }
    static Point* newPolar(float r, float theta) {
        return new Point(r*cos(theta), r*sin(theta));
    }
};

//-------------------------------------------------------------------
//  Factory
//-------------------------------------------------------------------

class Factory {
public:
    static Point* newCartesian(float x, float y) {
        return new Point(x, y);
    }
    static Point* newPolar(float r, float theta) {
        return new Point(r*cos(theta), r*sin(theta));
    }
};

void test_factory_method()
{
    // factory method
    auto p0 = std::unique_ptr<Point>(Point::newCartesian(0.0, 0.0));

    // factory
    auto p1 = std::unique_ptr<Point>(Factory::newCartesian(0.0, 0.0));
}

//-------------------------------------------------------------------
//  Abstract Factory
//-------------------------------------------------------------------

struct HotDrink {
    int volume = 0;

    virtual void prepare(int volume) = 0;
    virtual void print() = 0;
};
struct Tea : HotDrink {
    void prepare(int volume) override {
        this->volume = volume;
    }
    void print() override {
        std::cout << "Tea(" << volume << ")\n";
    }
};
struct Coffee : HotDrink {
    void prepare(int volume) override {
        this->volume = volume;
    }
    void print() override {
        std::cout << "Coffee(" << volume << ")\n";
    }
};

struct HotDrinkFactory {
    virtual std::unique_ptr<HotDrink> make() = 0;
};
struct TeaFactory : public HotDrinkFactory {
    std::unique_ptr<HotDrink> make() override {
        return std::make_unique<Tea>();
    }
};
struct CoffeeFactory : public HotDrinkFactory {
    std::unique_ptr<HotDrink> make() override {
        return std::make_unique<Coffee>();
    }
};

class DrinkFactory {
    std::map<std::string, std::unique_ptr<HotDrinkFactory>> maker;

public:
    DrinkFactory() {
        maker["tea"] = std::make_unique<TeaFactory>();
        maker["coffee"] = std::make_unique<CoffeeFactory>();
    }

    std::unique_ptr<HotDrink> make(const std::string& name, int volume) {
        auto drink = maker[name]->make();
        drink->prepare(volume);
        return drink;
    }
};

//-------------------------------------------------------------------
//  Functional
//-------------------------------------------------------------------
class DrinkWithVolumeFactory {
    std::map<std::string, std::function<std::unique_ptr<HotDrink>(int)>> factories;

public:
    DrinkWithVolumeFactory() {
        factories["coffee"] = [](int volume) {
            auto coffee = std::make_unique<Coffee>();
            coffee->prepare(volume);
            return coffee;
        };
        factories["tea"] = [](int volume) {
            auto tea = std::make_unique<Tea>();
            tea->prepare(volume);
            return tea;
        };
    }
    std::unique_ptr<HotDrink> make(const std::string& name, int volume) {
        return factories[name](volume);
    }
};

void test_abstract_factory() {
#if (0)
    DrinkFactory drinkMaker;
#else
    DrinkWithVolumeFactory drinkMaker;
#endif
    auto coffee = drinkMaker.make("coffee", 30);
    coffee->print();

    auto tea = drinkMaker.make("tea", 25);
    tea->print();
}

//-------------------------------------------------------------------
//  Factory Method Pattern
//-------------------------------------------------------------------
class DrinkCreator {
public:
    HotDrink* create(int volume) {
        return createMethod(volume);
    }
    virtual HotDrink* createMethod(int volume) = 0;
};
class CoffeeCreator : public DrinkCreator {
public:
    HotDrink* createMethod(int volume) {
        auto* coffee = new Coffee;
        coffee->prepare(volume);
        return coffee;
    }
};
class TeaCreator : public DrinkCreator {
public:
    HotDrink* createMethod(int volume) {
        auto* tea = new Tea;
        tea->prepare(volume);
        return tea;
    }
};

void test_factory_method_pattern() {
    auto coffeeMaker = std::unique_ptr<CoffeeCreator>(new CoffeeCreator);
    auto coffee = std::unique_ptr<HotDrink>(coffeeMaker->create(45));
    coffee->print();

    auto teaMaker = std::unique_ptr<TeaCreator>(new TeaCreator);
    auto tea = std::unique_ptr<HotDrink>(teaMaker->create(70));
    tea->print();
}

} //namespace =================================================================

void test_factory()
{
    //test_factory_method();
    //test_abstract_factory();
    test_factory_method_pattern();
}
