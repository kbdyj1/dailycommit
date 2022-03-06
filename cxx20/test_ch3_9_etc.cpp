#include <iostream>
#include <vector>

namespace //===================================================================
{

void test1()
{
    auto neck = int{};
    auto tail = int{};
    volatile auto brachiosaur = int{};

    brachiosaur = neck;
    tail = brachiosaur;

    tail = brachiosaur = neck;
    brachiosaur += neck;

    brachiosaur = brachiosaur + neck;
}

void test2()
{
    for (auto vec = std::vector{1, 2, 3}; auto v : vec) {
        std::cout << v << std::endl;
    }

    using namespace std::string_literals;
    for (auto str = "Hello, Qt"s; auto c : str) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

struct A {
    virtual int f() const = 0;
};
struct B : public A {
    constexpr int f() const override { return 2; }
};
struct C : public B {
    int f() const override { return 3; }
};
struct D : public C {
    constexpr int f() const override { return 4; }
};

void test3()
{
    A *a = new D;
    std::cout << "a->f(): " << a->f() << std::endl;

    D d;
    A &a2 = d;
    std::cout << "a2.f(): " << a2.f() << std::endl;
}

void test4()
{
    const char8_t *s0 = u8"Hello, Qt6";
    std::basic_string<char8_t> s2 = u8"Hello, Qt6";
    std::u8string s3 = u8"Hello, Qt";
    s3 += u8".";

    std::cout << "s2.size() : " << s2.size() << std::endl;
    std::cout << "s3.size() : " << s3.size() << std::endl;

    s3.replace(0, 5, u8"Hi");

    std::cout << "s3.size() : " << s3.size() << ", string : " << (char*)s3.c_str() <<std::endl;
}

enum class Color {
    Red,
    Green,
    Blue
};

std::string_view toString(Color c)
{
#if (0)
    swtich (c) {
    using enum Color;
    case Red: return "Red";
    case Green: return "Green";
    case Blue: return "Blue";
    default:
        return "Unknown";
    }
#else
    switch (c) {
    case Color::Red: return "Red";
    case Color::Green: return "Green";
    case Color::Blue: return "Blue";
    default:
        return "Unknown";
    }
#endif
}

void test5()
{
    std::cout << "toString(Color::Red) : " << toString(Color::Red) << std::endl;
}

struct E
{
    int i : 3 = 1;
    int j : 4 = 2;
    int k : 5 = 3;
    int l : 6 = 4;
    int m : 7 = 5;
    int n : 7 = 5;
};

void test6()
{
    E e;
    std::cout << "sizeof(E) : " << sizeof(E) << ", value : " << *((int *)&e) << std::endl;
}

} // namespace ================================================================

void test_ch3_9()
{
    std::cout << std::endl;

    //test2();
    //test3();
    //test4();
    //test5();
    test6();
}
