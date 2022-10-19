//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.18 Template polymorphism
//
//  22/10/19
//=============================================================================

#include <iostream>

namespace { //=================================================================

namespace _1 {

class Circle {
public:
    void draw() const { std::cout << "Circle::draw()\n"; }
};

class Line {
public:
    void draw() const { std::cout << "Line::draw()\n"; }
};

template <typename GeoObj>
void draw(const GeoObj& obj)
{
    obj.draw();
}

void test()
{
    auto circle = Circle{};
    auto line = Line{};

    draw(circle);
    draw(line);
}

}

} //namespace =================================================================

void test_ch_18()
{
#if (0) //done
    _1::test();
#endif
}
