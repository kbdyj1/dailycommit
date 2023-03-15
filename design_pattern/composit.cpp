#include <iostream>
#include <array>
#include <numeric>
#include <algorithm>
#include <memory>

namespace { //=================================================================

namespace _1 {

class Creature
{
public:
    enum Abilities {
        Strength,
        Agility,
        Intelligent,
        Count
    };
    std::array<int, Count> abilities;

    int get(Abilities ability) const {
        return abilities[ability];
    }
    void set(Abilities ability, int value) {
        abilities[ability] = value;
    }

    int sum() const {
        return std::accumulate(std::begin(abilities), std::end(abilities), 0);
    }
    double average() const {
        return sum() / (double)Count;
    }
    int max() const {
        return *std::max_element(std::begin(abilities), std::end(abilities));
    }
};

void test()
{
    auto human = Creature();
    human.set(Creature::Abilities::Strength, 100);
    human.set(Creature::Abilities::Agility, 50);
    human.set(Creature::Abilities::Intelligent, 120);

    std::cout << "human.sum(): " << human.sum() << "\n";
    std::cout << "human.average(): " << human.average() << "\n";
    std::cout << "human.max(): " << human.max() << std::endl;
}

} //_1 --------------------------------------------------------------

namespace _2 {

class GraphicObject
{
public:
    virtual void draw() = 0;
};

class Circle : public GraphicObject
{
public:
    virtual void draw() override {
        std::cout << "Circle::draw()\n";
    }
};
class Rectangle : public GraphicObject
{
public:
    virtual void draw() override {
        std::cout << "Rectangle::draw()\n";
    }
};

class Group : public GraphicObject
{
    std::vector<GraphicObject*> children;

public:
    virtual void draw() override {
        std::cout << "GraphicObject::draw()\n";
        std::for_each(std::begin(children), std::end(children), [](GraphicObject* child){
            std::cout << "\t";
            child->draw();
        });
    }
    void push_back(GraphicObject* child) {
        children.push_back(child);
    }
};

void test()
{
    std::vector<GraphicObject*> graphics;
    {
        auto c0 = std::unique_ptr<GraphicObject>(new Circle);
        graphics.push_back(c0.get());
        auto r0 = std::unique_ptr<GraphicObject>(new Rectangle);
        graphics.push_back(r0.get());
        auto g0 = std::unique_ptr<Group>(new Group);
        auto c1 = std::unique_ptr<GraphicObject>(new Circle);
        auto r1 = std::unique_ptr<GraphicObject>(new Rectangle);
        g0->push_back(c1.get());
        g0->push_back(r1.get());
        graphics.push_back(g0.get());

        for (auto* child : graphics) {
            child->draw();
        }
    }
}

} //_2 --------------------------------------------------------------

} // namespace ================================================================

void test_composit()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
