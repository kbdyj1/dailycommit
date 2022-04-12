#include <iostream>
#include <memory>
#include <vector>
#include <map>

namespace { //=================================================================

// Concrete types

// C10. Prefer concrete types ovver class hierarchies

class Point1
{
    int mX;
    int mY;

public:
    Point1(int x, int y) : mX(x), mY(y)
    {}
};

class Point2
{
    int mX;
    int mY;

public:
    Point2(int x, int y) : mX(x), mY(y)
    {}
    virtual void print()
    {
        std::cout << "Point2(" << mX << ", " << mY << ")" << std::endl;
    }

    int x() const { return mX; }
    int y() const { return mY; }

    virtual ~Point2()
    {}
};
class Point3 : public Point2
{
    int mZ;

public:
    Point3(int x, int y, int z)
        : Point2(x, y)
        , mZ(z)
    {}
    virtual void print() override
    {
        std::cout << "Point2(" << x() << ", " << y() << "," << mZ << ")" << std::endl;
    }
};

// C11. Make concrete types regular

struct Record
{
};
bool operator==(const Record& r0, const Record& r1)
{
    return true;
}

// C12. Don't make data members const or reference
struct Bundle
{
    std::string name;
    std::vector<Record> records;
};
bool operator==(const Bundle& b0, const Bundle& b1)
{
    return b0.name == b1.name && b0.records == b1.records;
}
bool operator!=(const Bundle& b0, const Bundle& b1)
{
    return !(b0 == b1);
}

// C20. If you can avoid defining any default operations, do

class NamedMap
{
    std::string name;
    std::map<int, int> rep;
};

// C21. If you define or =delete any copy, move, or destructor function, define or =delete them all

struct Bad
{
public:
    ~Bad() { destroy(); }

    void create(int size)
    {
        destroy();
        p = new int[size];
    }
    void destroy()
    {
        delete [] p;
    }
private:
    int *p = nullptr;
};

class Good
{
public:
    virtual ~Good() = default;
};

class Cloneable
{
public:
    Cloneable() = default;
    virtual ~Cloneable() = default;

    virtual std::unique_ptr<Cloneable> clone() const;

    // to prevent slicing ... protected or =delete
    Cloneable(const Cloneable&) = delete;
    Cloneable& operator=(const Cloneable&) = delete;
    Cloneable(Cloneable&&) = delete;
    Cloneable& operator=(Cloneable&&) = delete;
};

std::unique_ptr<Cloneable> Cloneable::clone() const
{
    return std::unique_ptr<Cloneable>(new Cloneable{});
}

// C22. Make default operations consistent

class Silly
{
    class Impl {};
    std::shared_ptr<Impl> d;
public:
    Silly(const Silly& other) : d(std::make_shared<Impl>())
    {
        // deep copy
        *d = *other.d;
    }
    Silly& operator=(const Silly& other)
    {
        // shallow copy
        if (this != &other) {
            d = other.d;
        }
        return *this;
    }
};

} // namespace ================================================================

void test_c_10()
{
    auto p0 = Point1{1, 2};
    auto p1 = Point1{p0};

    auto p2 = std::make_unique<Point2>(3, 4);
    p2->print();

    auto p3 = std::make_unique<Point3>(4, 5, 6);
    p3->print();
}

void test_c_11()
{
    auto r0 = Record{};
    auto r1 = Record{};
    auto r2 = Record{};

    auto b0 = Bundle{"Qt6", {r0, r1, r2}};
    auto b1 = b0;

    if (b0 != b1) {
        std::cerr << "b0 != b1 : impossible !!!" << std::endl;
    }

    b0.name = "Qt5";

    std::cout << "b0 == b1 : " << (b0 == b1) << std::endl;
}

void test_c_20()
{
    auto n0 = NamedMap{};
    auto n1 = NamedMap{n0};
}

void test_c_21()
{
    auto b0 = Bad{};
    b0.create(100);

    auto b1 = b0;
}
