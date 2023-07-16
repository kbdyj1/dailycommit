#include <vector>
#include <string>
#include <iostream>

namespace { //=================================================================

namespace _1 {

class A {
public:
    A() {
        std::cout << "A::A()\n";
    }
    A(const A&) {
        std::cout << "A::A(const A&)\n";
    }
    A(A&&) {
        std::cout << "A::A(A&&)\n";
    }
    A(const char* str) {
        std::cout << "A::A(const char*)\n";
    }
    ~A() {
        std::cout << "A::~A()\n";
    }
};

void test()
{
    std::vector<A> vecs;

#if (0)
    // A::A(const char*)
    // A::A(A&&)
    // A::~A()

    vecs.push_back("Hello, Qt");
#else
    // A::A(const char*)

    vecs.emplace_back("Hello, Qt");
#endif

    std::cout << "end of test.\n" << std::endl;
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_emplaceback()
{
    _1::test();
}
