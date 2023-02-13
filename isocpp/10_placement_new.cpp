#include <iostream>

namespace { //=================================================================

namespace _1 {

struct Point
{
    int x;
    int y;

    Point()
    {
        std::cout << "Point()\n";
    }
    Point(int x, int y) : x(x), y(y)
    {
        std::cout << "Point(" << x << ", " << y << ")\n";
    }
    ~Point()
    {
        std::cout << "~Point()\n";
    }
};

void test()
{
    auto* p = static_cast<Point*>(operator new(sizeof(Point) * 10));

    for (int i=0; i<10; i++) {
        new (&p[i]) Point(i, 0);
    }

    for (int i=0; i<10; i++) {
        p[i].~Point();
    }

    operator delete(p);
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_placement_new()
{
    _1::test();
}
