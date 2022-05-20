#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <array>
#include <memory>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string_view>

namespace { //=================================================================

#if (0) // compile error
constexpr int lastElem() {
    std::vector vec = {1, 2, 4, 3};
    std::sort(vec.begin(), vec.end());
    return vec.back();
}
void test1()
{
    constexpr int maxValue = lastElem();
    std::cout << "maxValue: " << maxValue << std::endl;

    constexpr int maxValue2 = []{
        std::vector vec = { 1, 3, 4, 2 };
        std::sort(vec.begin(), vec.end());
        return vec.back();
    }();
    std::cout << "maxValue2: " << maxValue2 << std::endl;
}
#endif

void test2()
{
    auto a0 = std::to_array("Hello, Qt6");
    for (auto c : a0)
        std::cout << c << " ";
    std::cout << std::endl;

    auto a1 = std::to_array({1, 2, 3, 4, 5});
    for (auto i : a1)
        std::cout << i << " ";
    std::cout << std::endl;

    auto a2 = std::to_array<double>({0, 1, 3});
    for (auto d : a2)
        std::cout << d << " ";
    std::cout << std::endl;
    std::cout << "typeid(a2[0]).name(): " << typeid(a2[0]).name() << std::endl;

    auto a3 = std::to_array<std::pair<int, double>>({{1, 1.1}, {2, 4.4}, {3, 9.9}});
    for (auto p : a3)
        std::cout << "(" << p.first << ", " << p.second << ") ";
    std::cout << std::endl;
}

void test3()
{
    //auto s0 = std::make_shared<double[]>(1024);
    //auto s1 = std::make_shared<double[]>(1024, 1.0);
}

void print(auto vec)
{
    for (auto v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void test4()
{
    auto vec = std::vector{-2, 3, -5, 10, 3, 0, -5};
    print(vec);

    auto end = std::remove_if(vec.begin(), vec.end(), [](int elem){
        return elem < 0;
    });
    vec.erase(end, vec.end());
    print(vec);
}

template <typename C>
void eraseValue(C &c, int value)
{
    std::erase(c, value);
}

template <typename C, typename Pred>
void erasePred(C &c, Pred pred)
{
    std::erase_if(c, pred);
}

void testContainer(auto c)
{
    print(c);
    eraseValue(c, 5);
    print(c);
    erasePred(c, [](auto i){ return i >= 3; });
    print(c);
}

void test5()
{
    auto v0 = std::vector{-2, 3, -5, 10, 3, 0, -5};
    print(v0);

    auto removeMinus = [](int i) {
        return i < 0;
    };

    auto v1 = v0;
    erasePred(v1, removeMinus);
    print(v1);

    auto v2 = v0;
    eraseValue(v2, -5);
    print(v2);

    auto str = std::string{"Hello, Qt6"};
    std::cout << "str: " << str << std::endl;

    auto s0 = str;
    std::erase(s0, 'l');
    std::cout << "s0: " << s0 << std::endl;

    auto removeUpper = [](char c) { return std::isupper(c); };
    auto s1 = str;
    std::erase_if(s1, removeUpper);
    std::cout << "s1: " << s1 << std::endl;

    std::cout << std::endl;
    std::cout << "test vector" << std::endl;
    auto v3 = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9};
    testContainer(v3);

    std::cout << std::endl;
    std::cout << "test list" << std::endl;
    auto l0 = std::list{1, 2, 3, 4, 5, 6, 7, 8, 9};
    testContainer(l0);

    std::cout << std::endl;
    std::cout << "test deque" << std::endl;
    auto d0 = std::deque{1, 2, 3, 4, 5, 6, 7, 8, 9};
    testContainer(d0);
}

void test6()
{
    auto s0 = std::set{3, 2, 1};
    print(s0);

    if (s0.find(2) != s0.end()) {
        std::cout << "s0.find(2) !!!" << std::endl;
    }

    auto s1 = std::multiset{3, 2, 1, 2};
    print(s1);
    if (s1.count(2)) {
        std::cout << "s1.found(2) !!!" << std::endl;
    }
}

template <typename C>
bool contains5(const C& c)
{
    return c.contains(5);
}

void test7()
{
    auto s0 = std::set{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << "s0.contains(5): " << contains5(s0) << std::endl;

    auto s1 = std::unordered_set{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << "s1.contains(5): " << contains5(s0) << std::endl;

    auto m0 = std::map<int, std::string>{{1, "red"}, {2, "green"}, {3, "blue"}};
    std::cout << "m0.contains(5): " << contains5(m0) << std::endl;

    auto m1 = std::unordered_map<int, std::string>{{1, "red"}, {2, "green"}, {3, "blue"}};
    std::cout << "m0.contains(5): " << contains5(m1) << std::endl;
}

template <typename Prefix>
void startsWith(const std::string &s, Prefix prefix)
{
    std::cout << "starts with " << prefix << ": " << s.starts_with(prefix) << std::endl;
}

template <typename Prefix>
void endsWith(const std::string &s, Prefix prefix)
{
    std::cout << "  ends with " << prefix << ": " << s.ends_with(prefix) << std::endl;
}

void test8()
{
    auto s0 = "Hello, Qt6";
    startsWith(s0, 'H');

    startsWith(s0, std::string_view("Hello"));

    endsWith(s0, '5');
    endsWith(s0, '6');
    endsWith(s0, "Qt6");
}

} // namespace ================================================================

void test_ch4_3()
{
    std::cout << std::endl;
    std::cout << std::boolalpha;

    //test2();
    //test4();
    //test5();
    //test6();
    //test7();
    test8();
}
