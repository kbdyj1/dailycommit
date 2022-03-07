#include <iostream>
#include <string>
#include <vector>
#include <concepts>
#include <deque>
#include <cmath>
#include <memory>
#include <set>

namespace { //=================================================================

auto sumInt = [](int a, int b) { return a + b; };
auto sumGen = [](auto a, auto b) { return a + b; };
auto sumDec = [](auto a, decltype(a) b) { return a + b; };
auto sumTem = []<typename T>(T a, T b) { return a + b; };

void test1()
{
    std::cout << std::endl;

    std::cout << "sumInt(1, 2) : " << sumInt(1, 2) << std::endl;
    std::cout << "sumGen(1, 2) : " << sumGen(1, 2) << std::endl;
    std::cout << "sumDec(1, 2) : " << sumDec(1, 2) << std::endl;
    std::cout << "sumTem(1, 2) : " << sumTem(1, 2) << std::endl;

    std::cout << "std::endl";

    auto hello = std::string{"Hello, "};
    auto qt = std::string{"Qt"};

    std::cout << "sumGen(hello, qt) : " << sumGen(hello, qt) << std::endl;
    std::cout << "sumDec(hello, qt) : " << sumGen(hello, qt) << std::endl;
    std::cout << "sumTem(hello, qt) : " << sumGen(hello, qt) << std::endl;

    std::cout << std::endl;

    std::cout << "sumInt(true, 20) : " << sumInt(true, 20) << std::endl;
    std::cout << "sumGen(true, 20) : " << sumGen(true, 20) << std::endl;
    std::cout << "sumDec(true, 20) : " << sumDec(true, 20) << std::endl;
    //std::cout << "sumTem(true, 20) : " << sumTem(true, 20) << std::endl;
}

auto containerSizeGen = [](const auto &container) { return container.size(); };
auto vectorSize = []<typename T>(const std::vector<T> &container) { return container.size(); };
//auto integralVectorSize = []<std::integral T>(const std::vector<T> &container) { return container.size(); };

class A
{
public:
    int size() const { return 100; }
};

void test2()
{
    std::cout << std::endl;

    auto dq = std::deque{1, 2, 3};
    auto vd = std::vector{1.0, 2.0, 3.0, 4.0, 5.0};
    auto vi = std::vector{1, 2, 3, 4, 5};

    std::cout << "containerSizeGen(dq) : " << containerSizeGen(dq) << std::endl;

    std::cout << std::endl;

    std::cout << "containerSizeGen(vd) : " << containerSizeGen(vd) << std::endl;
    std::cout << "vectorSize(vd) : " << vectorSize(vd) << std::endl;
    //std::cout << "integralVectorSize(vd) : " << integralVectorSize(vd) << std::endl;

    std::cout << std::endl;

    std::cout << "containerSizeGen(vi) : " << containerSizeGen(vi) << std::endl;
    std::cout << "vectorSize(vi) : " << vectorSize(vi) << std::endl;
//    std::cout << "integralVectorSize(vi) : " << integralVectorSize(vi) << std::endl;

    std::cout << std::endl;

    auto a = A{};
    std::cout << "containerSizeGen(a) : " << containerSizeGen(a) << std::endl;
}

struct LambdaFactory {
    auto foo() const {
        return [=]{ std::cout << s << std::endl; };
    }
    std::string s = "Hello, Qt6";
    ~LambdaFactory() {
        std::cout << "~LambdaFactory" << std::endl;
    }
};

auto makeLambda() {
#if (0)
    auto factory = LambdaFactory{};
#else
    static auto factory = LambdaFactory{};
#endif

    return factory.foo();
}

void test3()
{
    std::cout << std::endl;

    auto l = makeLambda();
    l();

    std::cout << std::endl;
}

int func1(int a, int b);
int func2(int a, int b) { return a + b; }

void test4()
{
    std::cout << std::endl;

    std::cout << "typeid(func1).name() : " << typeid(func1).name() << std::endl;
    decltype (*func1) func = func2;

    std::cout << "func(10, 20) : " << func(10, 20) << std::endl;
}

template <typename Cont>
void printContainer(const Cont &container)
{
    for (const auto &c : container) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

void test5()
{
    std::cout << std::endl;

    auto set1 = std::set<std::string>{"Hello", "Qt", "Creator", "planet.qt.io"};
    printContainer(set1);
#if (0)
    using SetDecreasing = std::set<std::string, decltype([](const auto &l, const auto &r){
        return l > r;
    })>;

    auto set2 = SetDecreasing{"Hello", "Qt", "Creator", "planet.qt.io"};
    printContainer(set2);

    using SetLength = std::set<std::string, decltype([](const auto &l, const auto &r){
        return l.size() < r.size();
    })>;
    auto set3 = SetLength{"Hello", "Qt", "Creator", "planet.qt.io"};
    printContainer(set3);

    auto set4 = std::set<int>{-10, 5, 3, 100, 0, -25};
    printContainer(set4);

    using SetAbsolute = std::set<int, decltype([](int a, int b){
        return std::abs(a) < std::abs(b);
    })>;
    auto set5 = SetAbsolute{-10, 5, 3, 100, 0, -25};
    printContainer(set5);
#endif
}

} // namespace ================================================================

void test_ch3_7()
{
    //test1();
    //test2();
    //test3();
    //test4();
    test5();
}
