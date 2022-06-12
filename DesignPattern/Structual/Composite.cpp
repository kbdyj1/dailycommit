#include <iostream>
#include <vector>
#include <memory>

namespace { //=================================================================

struct Graphic {
    virtual void draw() = 0;
};
struct Circle : public Graphic {
    void draw() override {
        std::cout << "Circle::draw()" << "\n";
    }
};
struct Rectangle : public Graphic {
    void draw() override {
        std::cout << "Rectangle::draw()" << "\n";
    }
};
struct Group : public Graphic {
    std::vector<Graphic*> graphics;

    void add(Graphic* g) {
        graphics.push_back(g);
    }
    void draw() override {
        std::cout << "Group::draw() begin." << "\n";
        for (auto* g : graphics) {
            std::cout << "\t";
            g->draw();
        }
        std::cout << "Group::draw() begin." << "\n";
    }
};

} //namespace =================================================================

void test_composite()
{
    auto circle = std::make_unique<Circle>();
    auto rectangle = std::make_unique<Rectangle>();
    auto group = std::make_unique<Group>();
    group->add(circle.get());
    group->add(rectangle.get());
    group->draw();
}
