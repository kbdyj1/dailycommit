#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>

#include "estl_util.h"

// 1. vector, string, deque
// 2. list (balanced tree)

namespace { //=================================================================

template <typename C>
void print_all(const C& c, const std::string& desc)
{
    std::cout << desc << "\n";

    for (auto i : c) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

void test_container_feature()
{
    constexpr int SIZE = 16;
    {
        auto v = std::vector<int>{0, 2, 1};
        print_all(v, "vector{0, 2, 1}");
        auto iter = v.begin();
        iter = v.erase(iter);
        std::cout << "v.erase(iter) : " << *iter << "\n";
        auto ref = v.front();
        std::cout << "v.front() : " << ref << "\n";

        v.reserve(SIZE);
        std::cout << "v.capacity() : " << v.capacity() << "\n";
    }

    {
        auto l = std::list<int>{0, 2, 1};
        print_all(l, "list{0, 2, 1}");
    }

    {
        auto s = std::set<int>{0, 2, 1};
        print_all(s, "set{0, 2, 1}");
        std::cout << "s.count(1): " << s.count(1) << "\n";
        auto iter = s.lower_bound(1);
        std::cout << "s.lower_bound(1) : " << *iter << "\n";
        iter = s.upper_bound(1);
        std::cout << "s.upper_bound(1) : " << *iter << "\n";
    }
    {
        auto m = std::map<int, std::string>{{0, "zero"}, {1, "one"}, {2, "two"}};
        // 0 or 1
        std::cout << "m.count(3) : " << m.count(3) << "\n";
        std::cout << m[0] << "\n";

        auto mm = std::multimap<int, std::string>{{0, "zero"}, {0, "void"}, {1, "one"}, {1, "solo"}, {2, "two"}, {2, "duo"}};
        // 0~
        std::cout << "m.count(0) : " << mm.count(0) << "\n";
        for (auto iter = mm.begin(); iter != mm.end(); iter++) {
            std::cout << "(" << iter->first << ", " << iter->second << ") ";
        }
    }
}

namespace item3 {

class A {
    int value;
public:
    A(int value = 0) : value(value)
    {
        std::cout << "A(" << value << ")\n";
    }
    A(const A& rhs) : value(rhs.value)
    {
        std::cout << "A(A) : value(" << value << ")\n";
    }
    A& operator=(const A&rhs)
    {
        if (this != &rhs) {
            value = rhs.value;
            std::cout << "A::operator=(" << rhs.value << ")\n";
        }
        return *this;
    }
    int getValue() const {
        return value;
    }
};

void test()
{
    std::vector<A> va;
    va.reserve(8);
    for (auto i=1; i<=6; i++) {
        va.push_back(A(i));
    }

    for (auto iter=va.begin(); iter!=va.end(); iter++) {
        if (iter->getValue() == 2) {
            iter = va.erase(iter);
        }
    }

    //A aa[4];
}

} // item3 ----------------------------------------------------------

namespace item4 {

void test()
{
    std::list<int> l0 = { 1, 2, 3, 4, 5 };
    std::list<int> l1 = { 6, 7, 8, 9, 10};

    print_elements(l0, "l0 : ");
    print_elements(l1, "l1 : ");

    std::cout << "l0.splice()\n";
    l0.splice(l0.end(), l1, std::find(l1.begin(), l1.end(), 6), std::find(l1.rbegin(), l1.rend(), 10).base());

    print_elements(l0, "l0 : ");
    print_elements(l1, "l1 : ");

    if (l1.size() == 0) {
        std::cout << "l1.size() == 0\n";
    }
    if (l1.empty()) {
        std::cout << "l1.empty() == true\n";
    }
}

} // item4 ----------------------------------------------------------

namespace item5 {

void test()
{
    std::vector<int> v0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> v1;

    //v1.assign(v0.begin() + v0.size()/2, v0.end());
    //std::copy(v0.begin() + v0.size()/2, v0.end(), std::back_inserter(v1));
    v1.insert(v1.end(), v0.begin() + v0.size()/2, v0.end());

    print_elements(v1, "v1 : ");
}

} // item5 ----------------------------------------------------------

} // namespace ================================================================

void test_estl_ch_1()
{
    item5::test();
}
