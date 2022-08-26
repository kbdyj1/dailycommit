//=============================================================================
//  Modern C++ Programming Cookbook
//  Ch.5 container, algorithm and library
//
//  22/08/26
//=============================================================================

#include <iostream>
#include <vector>
#include <stack>
#include <bitset>
#include <iterator>
#include <string>
#include <algorithm>

namespace { //=================================================================

using namespace std::literals;

struct A {
    A() : a(0), b(0.0)
    {
        std::cout << "A()\n";
    }
    A(int a, double b, const std::string& c) : a(a), b(b), c(c)
    {
        std::cout << "A(" << a << ", " << b << ", " << c << ")\n";
    }
    A(const A& other) {
        std::cout << "A(const A&)\n";
        a = other.a;
        b = other.b;
        c = other.c;
    }
    A& operator=(const A& other) {
        std::cout << "A& operator=(const A&)\n";
        if (this != &other) {
            a = other.a;
            b = other.b;
            c = other.c;
        }
        return *this;
    }
    ~A()
    {
        std::cout << "~A()\n";
    }
    int a;
    double b;
    std::string c;
};
std::ostream& operator<<(std::ostream& os, const A& a)
{
    os << "A(a:" << a.a << ", b:" << a.b << ", c:" << a.c << ")";
    return os;
}

void test_vector()
{
    int arr[] = {1, 2, 3, 4, 5};
#if (0)
    auto v0 = std::vector<int>{arr, arr+5};
#else
    auto v0 = std::vector<int>{};

    v0.assign(arr, arr+5);
#endif

    std::cout << "v0.size(): " << v0.size() << "\n";
    std::copy(v0.begin(), v0.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    auto v1 = std::vector<int>{v0.begin(), v0.end()};

    auto v2 = std::vector<int>(5, 1);
    auto v3 = std::vector<int>{0, 10};

    v2.insert(v2.begin(), v3.begin(), v3.end());

    std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

#if (0)
    auto v4 = std::vector<A>{{1, 1.0, "one"}, {2, 2.0, "two"}, {3, 3.0, "three"}};
#else
    auto v4 = std::vector<A>{};

    std::cout << "v4.capacity(): " << v4.capacity() << "\n";

    v4.reserve(8);
    v4.emplace_back(1, 1.0, "one");
    v4.emplace_back(2, 2.0, "two");
    v4.emplace_back(3, 3.0, "three");
#endif
    //std::copy(v4.begin(), v4.end(), std::ostream_iterator<A>(std::cout, "\n"));
}

void test_stack()
{
    auto s = std::stack<A>();

#if (0)
    s.push({1, 1.5, "one"});    //A(1, 1.5, one), A(const A&), ~A()
    s.push({2, 2.5, "two"});
    s.push({3, 3.5, "three"});
#else
    s.emplace(1, 1.5, "one");   //A(1, 1.5, one)
    s.emplace(2, 2.5, "two");
    s.emplace(3, 3.5, "three");
#endif

    std::cout << "s.size(): " << s.size() << "\n";
    while (!s.empty()) {
        auto& a = s.top();
        std::cout << a << "\n";
        s.pop();
    }
}

void test_bitset()
{
    enum Variant {
        Rear = 0,
        RearUSB,
        Cam0,
        Cam1,
        Radio0,
        Radio1,
        Radio2,
        Radio3
    };

    auto b0 = std::bitset<8>();
    std::cout << b0 << ", any? " << b0.any() << ", none? " << b0.none() << "\n";

    auto b1 = std::bitset<8>(10);
    std::cout << b1 << ", any? " << b1.any() << ", none? " << b1.none() << "\n";

    auto b2 = std::bitset<8>("11110000"s);
    std::cout << b2 << ", 1s: " << b2.count() << "\n";
    std::cout << "Rear: " << b2.test(Variant::Rear) << "\n";
    std::cout << "Radio0: " << b2.test(Variant::Radio0) << "\n";
    std::cout << "Cam0: " << b2[Variant::Cam0] << "\n";

    b2.flip();
    std::cout << "b2.flip(): " << b2 << "\n";

    b2.reset(0);

    auto b2str = b2.to_string('O', 'X');
    std::cout << b2str << "\n";

    std::cout << "b2.ulong(): " << b2.to_ulong() << "\n";
}

void test_bool_vector()
{
    auto v = std::vector<bool>();
    v.push_back(true);  //0
    v.push_back(false); //1
    v.push_back(false); //2
    v.push_back(true);  //3
    v.push_back(false); //4
    v.push_back(true);  //5

    v[3] = false;

    auto trues = std::count(v.begin(), v.end(), true);
    std::cout << "true count: " << trues << "\n";
}

} //namespace =================================================================

void test_ch_05_algorithm();

void test_ch_05()
{
#if (0) // done
    test_vector();
    test_stack();
    test_bitset();
    test_bool_vector();
#endif

    test_ch_05_algorithm();
}
