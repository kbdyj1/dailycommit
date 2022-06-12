#include <iostream>
#include <sstream>

namespace { //=================================================================

class Shape {
public:
    virtual std::string print() {
        return "Shape::print()";
    }
};
class ColoredShape : Shape {
    Shape& shape;
    std::string color;
public:
    ColoredShape(Shape& s, const std::string& c)
        : shape(s)
        , color(c)
    {}
    std::string print() override {
        std::ostringstream os;
        os << shape.print() << ", color: " << color;
        return os.str();
    }
};
class Circle : public Shape {
    float radius;

public:
    explicit Circle(float r) : radius(r)
    {}
    void resize(float r) {
        radius = r;
    }
    std::string print() override {
        std::ostringstream os;
        os << " a circle of radius: " << radius;
        return os.str();
    }
};

} //namespace =================================================================

void test_decorater()
{
    auto circle = Circle(1.0);
    auto redCircle = ColoredShape(circle, "red");
    std::cout << redCircle.print() << "\n";
}
