#include <iostream>

namespace { //=================================================================

// C129. When designing a class hierarchy, distinguish between implementation inheritance and interface inheritance

struct Point
{
    int x;
    int y;
};
struct Color
{
    int r;
    int g;
    int b;
    int a;
};

namespace bad
{

class Shape
{
    Point mPoint;
    Color mColor;
public:
    Shape();
    Shape(Point pt = {0, 0}, Color color = {0, 0, 0, 0})
        : mPoint(pt)
        , mColor(color)
    {}

    Point center() const { return mPoint; }
    Color color() const { return mColor; }

    virtual void rotate(int) = 0;
    virtual void move(Point p)
    {
        mPoint = p;
        redraw();
    }
    virtual void redraw();
};

class Circle : public Shape
{
    int mRad;
public:
    Circle (Point pt, int r)
        : Shape(pt)
        , mRad(r)
    {}
};

class Triangle : public Shape
{
    Point mPoint2;
    Point mPoint3;

public:
    Triangle(Point p0, Point p2, Point p3)
        : Shape(p0)
        , mPoint2(p2)
        , mPoint3(p3)
    {}
};
} // bad

namespace good
{

class Shape
{
public:
    virtual Point center() const = 0;
    virtual Color color() const = 0;
    virtual void rotate(int) = 0;
    virtual void move(Point) = 0;
    virtual void redraw() = 0;
};
#if (0)
class Circle : public Shape
{
    Point mCenter;
    int mRad;
    Color mColor;
public:
    Circle(Point pt, int r, Color c)
        : mCenter(pt)
        , mRad(r)
        , mColor(c)
    {}

    Point center() const { return mCenter; }
    Color color() const { return mColor; }
};
#else
class Circle : public virtual Shape
{
public:
    virtual int radius() = 0;
};
#endif

} // good

} // namespace ================================================================
