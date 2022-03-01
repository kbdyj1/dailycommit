#include <iostream>

namespace
{

class Point3D
{
public:
    int x;
    int y = 10;
    int z = 20;
};

void printPoint3D(const Point3D& p)
{
    std::cout << "Point3D(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
}

void test1()
{
    auto p0 = Point3D{1, 2, 3};
    auto p1 = Point3D{ .x = 2, .y = 4, .z = 6 };

    printPoint3D(p0);
    printPoint3D(p1);
    printPoint3D({.x = 5, .z = 15});
}

} // namespace

void test_ch3_4_initialize()
{
    test1();
}
