#include <iostream>
#include <string>
#include <sstream>

using namespace std;

namespace { //=================================================================

class Shape
{
public:
    virtual string str() const
    {
        return "Shape";
    }
};

class Square : public Shape
{
    int wh;

public:
    Square(int wh = 0) : wh(wh)
    {}

    string str() const override
    {
        ostringstream os;
        os << "Square(" << wh << ")";
        return os.str();
    }

    void resize(int value)
    {
        wh = value;
    }
};

class Circle : public Shape
{
    int width;

public:
    Circle(int w) : width(w)
    {}

    string str() const override
    {
        ostringstream os;
        os << "Circle(" << width << ")";
        return os.str();
    }
};

namespace _1 {

class ColoredShape : public Shape
{
    Shape& shape;
    string color;

public:
    ColoredShape(Shape& shape, const string& color)
        : shape(shape)
        , color(color)
    {}

    string str() const override
    {
        ostringstream os;
        os << shape.str() << " has the color " << color;
        return os.str();
    }
};

class TransparentShape : public Shape
{
    Shape& shape;
    uint8_t transparency;

public:
    TransparentShape(Shape& shape, uint8_t trans)
        : shape(shape)
        , transparency(trans)
    {}

    string str() const override
    {
        ostringstream os;
        os << shape.str() << " has " << static_cast<float>(transparency) / 255.f * 100.f;
        os << "% transparency";
        return os.str();
    }
};

void test()
{
    auto square = Square(3);
    auto demiSquare = TransparentShape(square, 85);

    cout << demiSquare.str() << "\n";

    auto circle = Circle{20};
    auto coloredCircle = ColoredShape{circle, "red"};
    auto transparencyColoredCircle = TransparentShape{coloredCircle, 64};

    cout << transparencyColoredCircle.str() << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
class ColoredShape : public T
{
    static_assert(std::is_base_of_v<Shape, T>, "template argument should be a Shape");

public:
    string color;

    string str() const override
    {
        ostringstream os;
        os << T::str() << " has the color " << color;
        return os.str();
    }
};

template <typename T>
class TransparentShape : public T
{
    static_assert(std::is_base_of_v<Shape, T>, "template argument should be a Shape");


public:
    uint8_t transparency;

    string str() const override
    {
        ostringstream os;
        os << T::str() << " has " << static_cast<float>(transparency) / 255.f * 100.f;
        os << "% transparency";
        return os.str();
    }
};

void test()
{
    auto square = ColoredShape<TransparentShape<Square>>();
    square.color = "red";
    square.transparency = 85;
    square.resize(40);

    cout << square.str() << "\n";
}

} //_2 --------------------------------------------------------------

} // namespace ================================================================

void test_decorator()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
