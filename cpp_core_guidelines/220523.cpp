#include <iostream>
#include <exception>
#include <memory.h>

namespace { //=================================================================

// E.14 Use purpose-designed user-defined types as exceptions (not built-in types)
//      catch throw of built-in types and std::exception

class MyException : public std::runtime_error
{
public:
    MyException(const std::string& msg)
        : std::runtime_error(msg)
    {}
};

// E.15 Throw by value, catch exceptions from a hierarchy by reference

namespace e15
{

void bad()
{
    try {
        throw new MyException("throw by pointer");
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "throw !" << std::endl;
        throw;
    }
}

void good()
{
    try {
        throw MyException("throw by value");
    } catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

} //e15

// E.16 Destructors, deallocation, and swap must never fail
//      the standard library assumes that destructor

// E.17 Don't try to catch every exception in every function

// E.18 Minimize the use of explicit try/catch

namespace e18
{

class Gadget
{
    std::string str;

public:
    Gadget(const std::string& s) : str(s)
    {
    }
};

void bad(const std::string& s)
{
    Gadget* p;
    try {
        p = new Gadget(s);
        //...
        delete p;
    } catch (const std::exception& e)
    {
        // error handling
    }
}

void good(const std::string& s)
{
    auto g = Gadget(s);
}

} //e18

// E.19 Use a final_action object to express cleanup if no suitable resource handle is available

namespace e19
{

template <typename F>
class finally
{
    F func;
    bool invoke;

public:
    explicit finally(F f) noexcept
        : func(std::move(f))
        , invoke(true)
    {}

    finally(const finally&) = delete;
    finally& operator=(const finally&) = delete;

    finally(finally&& other) noexcept
        : func(std::move(other.func))
        , invoke(other.invoke)
    {
        other.invoke = false;
    }
    ~finally()
    {
        if (invoke)
            func();
    }
};

void f(int n)
{
    void* p = malloc(n);
    auto _ = finally([p]{
        free(p);
    });
}

} //e19

} //===========================================================================

void test_e_15()
{
#if (1)
    try {
        e15::bad();
    } catch (MyException* e) {
        std::cout << "MyException e->what() : " << e->what() << std::endl;
    } catch (std::exception* e)
    {
        std::cout << "std::exception e->what() : " << e->what() << std::endl;
    }

#else
    e15::good();
#endif
}
