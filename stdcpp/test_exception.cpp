#include <exception>
#include <stdexcept>
#include <iostream>

namespace { //=================================================================

namespace _1 {

void test()
{
    throw std::bad_alloc();   //terminate() -> abort() -> SIGABRT
}

} //_1 --------------------------------------------------------------

namespace _2 {

int divide(int num, int den)
{
    if (den) {
        return num/den;
    } else {
        throw std::invalid_argument("divide by zero");
    }
}

void f()
{
    std::cout << divide(10, 2) << "\n";
    std::cout << divide(10, 0) << "\n";
}

void g()
{
    try {
        f();
    } catch (const std::exception& e)
    {
        std::cerr << "g() route exception.\n";
        throw;
    }
}

void test()
{
    try {
        g();
    } catch (const std::exception& e)
    {
        std::cerr << "f() " << e.what() << std::endl;
    }
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_exception()
{
#if (0)
    _1::test();
#endif

    _2::test();
}
