#include <iostream>

struct A {
    int a;
    char b;
    float c;
    char d;
};

struct B {
    int a;
    float c;
    char b;
    char d;
};

void test_memalign()
{
    auto inst = A{};
    ptrdiff_t diff0 = reinterpret_cast<long>(&inst.b) - reinterpret_cast<long>(&inst.a);
    ptrdiff_t diff1 = reinterpret_cast<long>(&inst.c) - reinterpret_cast<long>(&inst.b);

    std::cout << diff0 << ", " << diff1 << std::endl;

    std::cout << &inst.a << ", " << (void*)&inst.b << ", " << &inst.c << std::endl;

    std::cout << "sizeof(A): " << sizeof(A) << ", sizeof(B): " << sizeof(B) << std::endl;
}
