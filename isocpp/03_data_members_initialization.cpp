#include <iostream>
#include <string>
#include <array>

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

struct Point {
    float x = 0.0f;
    float y = 0.0f;
};

void test()
{
    Point pt{ 10.0f, 10.0f };
}

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

namespace _6{

struct A {
    int zero{};     //value initialization.
    int a = 0;      //copy initialization
    double b{10.5}; //direct list initialization
#if (0)
    short c(10);    //error: expected parameter declarator
                    //error: expected ')'
                    //note: to match this '('

    int arr2[] = { 1, 2 };  //can't deduce
    auto i = 1;             //type deduction doesn't work
#endif
    int d{ a + 10 };
    int* p = new int(d);
    double e{ *p * 2.0 };
    long arr[4] = { 1, 2, 3, 4 };
    std::array<int, 4> numbers{ 1, 2, 3, 4 };
    double g{ compute() };

    ~A()
    {
        delete p;
    }
    double compute() { return a * b; }
};

} //_6 --------------------------------------------------------------

} //===========================================================================

void test_data_member_initialization()
{
#if (0) //done
    _2::test();
#endif

    _5::test();
}
