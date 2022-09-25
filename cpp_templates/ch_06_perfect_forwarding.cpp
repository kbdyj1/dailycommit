#include <iostream>

namespace { //=================================================================

class A
{};

void fun0(A&) {
    std::cout << "fun0(A&)\n";
}
void fun0(const A&) {
    std::cout << "fun0(const A&)\n";
}
void fun0(A&&) {
    std::cout << "fun0(A&&)\n";
}

//#define USE_FORWARD_REFERENCE

#if defined(USE_FORWARD_REFERENCE)
template <typename T>
void fun1(T&& val) {
    fun0(std::forward<T>(val));
}
#else
void fun1(A& a) {
    fun0(a);
}
void fun1(const A& a) {
    fun0(a);
}
void fun1(A&& a) {
    fun0(std::move(a));
}
#endif

} //namespace =================================================================

void test_ch_06_perfect_forwarding()
{
    A a;
    const A ca;

    fun1(a);
    fun1(ca);
    fun1(A{});
    fun1(std::move(a));
}
