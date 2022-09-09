#include <iostream>
#include <memory>
#include <string>

namespace { //=================================================================

using namespace std::string_literals;

class A {
    int value;
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
class C {
    int value;
public:
    virtual void print() {
        std::cout << "C::print()\n";
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
#if (0)
    const auto c = 62;
    auto* pc = static_cast<int*>(&c);   //error: invalid ‘static_cast’ from type ‘const int*’ to type ‘int*’
                                        //       39 |     auto* pc = static_cast<int*>(&c);
                                        //          |                ^~~~~~~~~~~~~~~~~~~~~
#endif
}

void test_dynamic_cast()
{
    auto a = A{};
    auto b = B{};
    auto c = C{};

    try {
        A& ra = dynamic_cast<A&>(b);
        B& rb = dynamic_cast<B&>(a);

        A* pa = dynamic_cast<A*>(&c);
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
    int* pi = new int{62};
    double* pd = reinterpret_cast<double*>(pi);

    delete pi;
}

} //namespace =================================================================

void test_ch_09_cast()
{
#if (0) // done
    test_static_cast();
    test_const_cast();
    test_dynamic_cast();
#endif

}
