#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <memory>

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
    ~A()
    {
        std::cout << "~A(" << value << ")\n";
    }
    int getValue() const {
        return value;
    }
};

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

void test_insert()
{
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    std::vector<int> v1 = { 6, 7, 8, 9, 10};
    v1.insert(v1.begin()+v1.size()/2, v0.begin(), v0.end());

    print_elements(v1, "v1 : ");

    std::set<int> s0 = { 2, 4, 1, 3, 5 };
    std::set<int> s1 = { 8, 7, 9, 10, 6};
    s1.insert(s0.begin(), s0.end());

    print_elements(s1, "s1 : ");
}

void test_erase()
{
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    v0.erase(v0.begin()+v0.size()/2, v0.end());

    print_elements(v0, "v0 : ");
    {
        std::set<int> s0 = { 2, 4, 1, 3, 5 };
        auto iter = s0.find(3);
        iter = s0.erase(iter, s0.end());

        print_elements(s0, "s0 : ");
    }
    {
        std::map<int, std::string> m = {{0, "zero"}, {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};
        auto iter = m.find(3);
        m.erase(iter, m.end());

        for (iter = m.begin(); iter != m.end(); iter++) {
            std::cout << "{" << iter->first << ", " << iter->second << "} ";
        }
        std::cout << "\n";
    }
}

} // item5 ----------------------------------------------------------

namespace item6 {

void make_test_file()
{
    auto os = std::ofstream("data.bin");
    if (os.is_open()) {
        int value;
        for (auto i=0; i<5; i++) {
            value = i + '0';
            os.write((const char*)&value, sizeof(int));
        }
        os.close();
    }
}

void test_istream_iterator()
{
    auto is = std::ifstream("data.bin");
    if (is.is_open()) {
#if (0)
        std::list<int> data(std::istream_iterator<int>(is), std::istream_iterator<int>());
#else
        auto begin = std::istream_iterator<int>(is);
        auto end = std::istream_iterator<int>();
        std::list<int> data(begin, end);
#endif
        is.close();
        std::cout << "data.size() : " << data.size() << "\n";
        for (auto iter = data.begin(); iter != data.end(); iter++) {
            std::cout << *iter << " ";
        }
    }
}

void test()
{
    //make_test_file();
    test_istream_iterator();
}

} // item6 ----------------------------------------------------------

namespace item7 {

#if (0)
template <typename T>
struct Deleter : public std::unary_function<const T*, void>
{
    void operator()(const T* p) const {
        delete p;
    }
};
#else
struct Deleter
{
    template <typename T>
    void operator()(const T* p) const {
        delete p;
    }
};
#endif

void test_pointer_cotainer()
{
    std::vector<A*> v;
    for (auto i=0; i<4; i++) {
        v.push_back(new A(i));
    }

    // clean up
#if (0)
    std::for_each(v.begin(), v.end(), [](A* a){
        delete a;
    });
#else
    std::for_each(v.begin(), v.end(), Deleter());
#endif
}

void test_shared_ptr_container()
{
    auto v = std::vector<std::shared_ptr<A>>();
    for (auto i=0; i<4; i++) {
        v.push_back(std::shared_ptr<A>(new A(i)));
    }
}

void test()
{
    test_shared_ptr_container();
}

} // item6 ----------------------------------------------------------

} // namespace ================================================================

void test_estl_ch_1()
{
    //item5::test_erase();
    //item6::test();
    item7::test();
}
