#include <iostream>
#include <memory>

namespace { //=================================================================

// R1. Manage resources automatically using resource handles and RAII(Resource Acquisition Is Initialization)

class A
{
public:
    A() { std::cout << "A()" << std::endl; }
   ~A() { std::cout << "~A()" << std::endl; }
};

void sendA(std::unique_ptr<A> a, std::string_view v)
{
    std::cout << "sendA(" << v << ")" << std::endl;
}

// R2. In interfaces, use raw pointers to denote indivisual objects (only)

} // namespace ================================================================

void test_r_1()
{
    auto s = std::string("Hello, Qt");
    std::string_view v(s);
    sendA(std::unique_ptr<A>(new A), v);
}
