#include <iostream>
#include <memory>

namespace { //=================================================================

#define STRINGFY2(x)    #x
#define STRINGFY(x)     STRINGFY2(x)
#define CONNECT2(x, y)  x##y
#define CONNECT(x, y)   CONNECT2(x, y)

#define NUM 6

void test_stringfy()
{
    auto s0 = std::string{STRINGFY(NUM)};
    std::cout << "STRINGFY(NUM): " << s0 << "\n";

    auto s1 = std::string{STRINGFY2(NUM)};
    std::cout << "STRINGFY2(NUM): " << s1 << "\n";
}

#define DECL_MAKE(x)    DECL_MAKE2(x)
#define DECL_MAKE2(x)   x* make##_##x() { return new x(); }

struct A {
    A() { std::cout << "A()\n"; }
    ~A() { std::cout << "~A()\n"; }
};
struct B {
    B() { std::cout << "B()\n"; }
    ~B() { std::cout << "~B()\n"; }
};

DECL_MAKE(A)
DECL_MAKE(B)

void test_decl()
{
    auto a = std::shared_ptr<A>(make_A());
    auto b = std::shared_ptr<B>(make_B());
}

} //namespace =================================================================

void test_ch_04_stringfy()
{
    //test_stringfy();
    test_decl();
}
