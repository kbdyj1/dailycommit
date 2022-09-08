#include <iostream>
#include <memory>
#include <string>

namespace { //=================================================================

using namespace std::string_literals;

class A {
public:
    virtual void print() {
        std::cout << "A::print()\n";
    }
};
class B : public A {
public:
    void print() override {
        std::cout << "B::print()\n";
    }
};

void test_static_cast()
{
    auto b = std::unique_ptr<B>(new B);
    b->print();

    auto* a = static_cast<A*>(b.get());
    a->print();

    enum Option {
        One = 1,
        Two,
        Three
    };
    auto value = 1;
    auto op = static_cast<Option>(value);
}

void test_dynamic_cast()
{
    auto a = A{};
    auto b = B{};

    try {
        A& ra = dynamic_cast<A&>(b);
        B& rb = dynamic_cast<B&>(a);
    } catch (const std::bad_cast& e) {
        std::cout << e.what() << std::endl;
    }
}

void oldFunc(char* str, unsigned int size)
{
    (void)str;
    (void)size;
}

void test_const_cast()
{
    auto s = "Hello, Qt6"s;

    oldFunc(const_cast<char*>(s.c_str()), static_cast<unsigned int>(s.size()));
}

void test_reinterpret_cast()
{

}

} //namespace =================================================================

void test_ch_09_cast()
{
#if (0) // done
    test_static_cast();
    test_dynamic_cast();
    test_const_cast();
#endif

}
