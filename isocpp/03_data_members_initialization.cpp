#include <iostream>
#include <string>

namespace { //=================================================================

namespace _1 { //~ c++11

struct Simple {
    int id;
    std::string name;

    Simple() : id(0), name("unnamed")
    {}
};

} //_1 --------------------------------------------------------------

namespace _2 { //c++11 ~

struct Simple {
    int id = 0;
    std::string name{"unnamed"};

    Simple()
    {}
};

void test()
{
    auto simple = Simple{};
    std::cout << "simple(): id(" << simple.id << ", " << simple.name << ")\n";
}
} //_2 --------------------------------------------------------------

namespace _3 { //c++14~

} //_3 --------------------------------------------------------------

namespace _4 { //c++17~

class Simple {
    static const int sValue = 10;
    static inline std::string sClassName = "Simple";

    Simple()
    {}
};

} //_4 --------------------------------------------------------------

namespace _5 {

struct A {
    int a{ initA() };
    std::string b{ initB() };

    int initA() {
        std::cout << "initA()\n";
        return 1;
    }
    std::string initB() {
        std::cout << "initB()\n";
        return "unnamed";
    }

    A() //a(initA()), b(initB())
    {
        std::cout << "A()\n";
    }

    A(int x) : a(x) //a(x), b(initB())
    {
        std::cout << "A(int)\n";
    }

#if (0)
    A(const A& other)
    {
        a = other.a;
        b = other.b;
#else
    A(const A& other) : a(other.a), b(other.b) //same with = default;
    {
#endif
        std::cout << "A(const A&)\n";
    }
};

void test()
{
    auto a = A(20);

    std::cout << "\nb = a;\n\n";
    auto b = a;
}

} //_5 --------------------------------------------------------------

} //===========================================================================

void test_data_member_initialization()
{
#if (0) //done
    _2::test();
#endif

    _5::test();
}
