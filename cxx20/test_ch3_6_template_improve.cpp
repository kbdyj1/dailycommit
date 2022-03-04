#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <algorithm>

namespace { //=================================================================

struct Implicit
{
    template<typename T>
    Implicit(T t) {
        std::cout << "Implicit(" << t << ")" << std::endl;
    }
};

struct Explicit
{
    template <typename T>
    explicit Explicit(T t) {
        std::cout << "Explicit(" << t << ")" << std::endl;
    }
};

void test1()
{
    std::cout << std::endl;

    Implicit imp1 = "implicit 1";
    Implicit imp2{"implicit 2"};
    Implicit imp3 = 2022;
    Implicit imp4{2022};

    std::cout << std::endl;

    //Explicit exp1 = "explicit 1";
    Explicit exp2{"explicit 2"};
    //Explicit exp3 = 2022;
    Explicit exp4{2002};
}

struct MyBool {
    template <typename T>
    explicit(!std::is_same<T, bool>::value) MyBool(T t) {
        std::string typeName = typeid(t).name();
        std::cout << typeName << std::endl;
    }
};

void needBool(MyBool b)
{}

void test2()
{
    MyBool b1{true};
    MyBool b2 = false;

    needBool(b1);
    needBool(true);
//    needBool(5);
//    needBool("true");
}

struct ClassType {
    constexpr ClassType(int)
    {}
};

template <ClassType c>
auto getClassType() {
    return c;
}

template <double d>
auto getDouble() {
    return d;
}

void test3()
{
    auto c1 = getClassType<ClassType(2020)>();
//    auto d1 = getDouble<5.5>();
//    auto d2 = getDouble<6.5>();
}

template <int N>
class StringLiteral {
public:
    constexpr StringLiteral(char const (&str)[N]) {
        std::copy(str, str+N, data);
    }
    char data[N];
    const int len = N;
};

template <StringLiteral str>
class ClassTemplate
{
public:
    ClassTemplate() {
        std::cout << "ClassTemplate(" << str.data << ", len: " << str.len << ")" << std::endl;
    }
};

template <StringLiteral str>
void FunctionTemplate() {
    std::cout << "FunctionTemplate(" << str.data << ", len: " << str.len << ")" << std::endl;
}

void test4()
{
    std::cout << std::endl;

    ClassTemplate<"Hello, Qt"> c;
    FunctionTemplate<"Hello, Qt6">();
}

} // namespace ================================================================

void test_ch3_6()
{
    //test1();
    //test2();
    test4();
}
