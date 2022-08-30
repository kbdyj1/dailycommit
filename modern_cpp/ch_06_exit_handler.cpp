#include <iostream>

namespace { //=================================================================

struct A {
    ~A() { std::cout << "~A()\n"; }
    static A* instance() {
        static A a;
        return &a;
    }
};

void test_exit0()
{
    std::cout << "test_exit()\n";
}

void test_exit1()
{
    std::cout << "test_exit()\n";
}

} //namespace =================================================================

//#define CREATE_STATIC_OBJECT_FIRST

void test_ch_06_exit_handler()
{
#if defined(CREATE_STATIC_OBJECT_FIRST)
    auto* a = A::instance();
#endif

    std::atexit(test_exit0);
    std::atexit(test_exit1);
    std::atexit([]{
        std::cout << "atexit( []{} )\n";
    });

#if !defined(CREATE_STATIC_OBJECT_FIRST)
    auto* a = A::instance();
#endif

    (void)a;
}
