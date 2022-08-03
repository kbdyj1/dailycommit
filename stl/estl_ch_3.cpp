#include <set>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "estl_util.h"

namespace { //=================================================================

struct Employee {
    int id;
    std::string name;
    std::string title;
};

std::ostream& operator<<(std::ostream& os, const Employee& e)
{
    os << "employee(" << e.id << ", name: " << e.name << ", title: " << e.title << ")";
    return os;
}

namespace item19 {

class A {
    int value;
public:
    explicit A(int value) : value(value)
    {}
    friend bool operator<(const A& l, const A& r);
};

bool operator<(const A& l, const A& r)
{
    return l.value < r.value;
}

bool ciCompStringLess(const std::string& s0, const std::string& s1)
{
    auto len0 = s0.length();
    auto len1 = s1.length();
    auto len = std::min(len0, len1);
    for (auto i=0; i<len; i++) {
        auto c0 = std::toupper(s0[i]);
        auto c1 = std::toupper(s1[i]);
        if (c0 < c1)
            return true;
        else if (c1 < c0)
            return false;
    }
    return len0 < len1;
}

struct CompString : public std::binary_function<std::string, std::string, bool> {
    bool operator()(const std::string& s0, const std::string& s1) const {
        return ciCompStringLess(s0, s1);
    }
};

void test()
{
    std::set<std::string, CompString> s;

    s.insert("qt6");
    s.insert("Qt6");

    std::cout << "s.size(): " << s.size() << "\n";

    for (auto iter=s.begin(); iter!=s.end(); iter++)
        std::cout << *iter << "\n";

    auto key = "qT6";
    {
        auto iter = s.find(key);
        if (iter != s.end()) {
            std::cout << "s.find(" << key << "): " << *iter << "\n";
        }
    }
    {
        auto iter = std::find(s.begin(), s.end(), key);
        if (iter != s.end()) {
            std::cout << "std::find(): " << *iter << "\n";
        }
    }
}

} // item19 -----------------------------------------------

namespace item20 {

struct StrPtrComp : public std::binary_function<const std::string*, const std::string*, bool>
{
    bool operator()(const std::string* p0, const std::string* p1) const {
        return *p0 < *p1;
    }
};

void printStrPtr(const std::string* p)
{
    std::cout << *p << "\n";
}

struct Dereference {
    template <typename T>
    const T& operator()(const T* ptr) const {
        return *ptr;
    }
};
struct DereferenceLess {
    template <typename T>
    bool operator()(T p0, T p1) const {
        return *p0 < *p1;
    }
};

void test()
{
    std::set<std::string*, DereferenceLess> ps;
    ps.insert(new std::string{"ddd"});
    ps.insert(new std::string{"ccc"});
    ps.insert(new std::string{"aaa"});
    ps.insert(new std::string{"bbb"});

    std::transform(ps.begin(), ps.end(), std::ostream_iterator<std::string>(std::cout, "\n"), Dereference());
}

} // item20 -----------------------------------------------

namespace item21 {

// The associative container comparison function should return false for the same value.

void test()
{
    std::set<int, std::less_equal<int>> s;
    s.insert(10);
    s.insert(10);
    std::cout << "s.size(): " << s.size() << "\n";
}

} // item21 -----------------------------------------------

namespace item22 {


struct IdCmpLess : public std::binary_function<const Employee&, const Employee&, bool>
{
    bool operator()(const Employee& e0, const Employee& e1) const {
        return e0.id < e1.id;
    }
};

std::set<Employee, IdCmpLess> s;

void init()
{
    s.insert({2, "ccc", "worker"});
    s.insert({1, "bbb", "manager"});
    s.insert({3, "aaa", "vip"});
}

void print()
{
    std::cout << "***** list of set ******\n";
    for (auto iter=s.begin(); iter!=s.end(); iter++) {
        std::cout << "id: " << iter->id << ", name: " << iter->name << ", title: " << iter->title << "\n";
    }
}

void test_modify_using_cast()
{
    auto key = 3;
    auto iter = std::find_if(s.begin(), s.end(), [key](const Employee& e) {
        return key == e.id;
    });
    if (iter != s.end()) {
    #if (1)
        const_cast<Employee&>(*iter).title = "programmer";
    #else
        static_cast<Employee>(*iter).title = "programmer";
    #endif
        std::cout << "id: " << iter->id << ", name: " << iter->name << ", title: " << iter->title << "\n";
    }
    print();
}

void test_modify_using_erase()
{
    auto key = 3;
    auto iter = std::find_if(s.begin(), s.end(), [key](const Employee& e) {
        return key == e.id;
    });
    auto employee = *iter;
    s.erase(iter);

    employee.title = "programmer";
    s.insert(employee);

    print();
}

void test()
{
    init();

    //test_modify_using_cast();
    test_modify_using_erase();
}

} // item22 -----------------------------------------------

namespace item23 {

std::vector<Employee> v;

void init()
{
    v.push_back({4, "hhh", "title-18"});
    v.push_back({2, "ccc", "title-14"});
    v.push_back({6, "bbb", "title-07"});
    v.push_back({1, "zzz", "title-10"});
    v.push_back({5, "aaa", "title-03"});

    std::sort(v.begin(), v.end(), [](const Employee& e0, const Employee& e1){
        return e0.id < e1.id;
    });
}

void test_binary_search(int key)
{
    auto e = Employee{key, "", ""};
    auto less = [](const Employee& e0, const Employee& e1){
        return e0.id < e1.id;
    };
    auto result = std::binary_search(v.begin(), v.end(), e, less);
    std::cout << "std::binary_search(" << key << "): " << result << "\n";
}

void test_lower_bound(int key)
{
    auto e = Employee{key, "", ""};
    auto less = [](const Employee& e0, const Employee& e1){
        return e0.id < e1.id;
    };
    auto iter = std::lower_bound(v.begin(), v.end(), e, less);
    if (iter != v.end()) {
        std::cout << "std::lower_bound(" << key << "): " << *iter << "\n";
    } else {
        std::cout << "std::lower_bound(" << key << "): " << "false\n";
    }
}

void test_equal_range()
{
    std::vector<int> v0{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto pair = std::equal_range(v0.begin(), v0.end(), 4, [](int val0, int val1){
        return val0 < val1;
    });
    for (auto iter=pair.first; iter!=pair.second; iter++) {
        std::cout << *iter << " ";
    }
    std::cout << "\n";
}

struct Pair {
    int key;
    Employee value;
};
struct PairCmp {
    bool keyLess(int l, int r) const
    {
        return l < r;
    }
    bool operator()(const Pair& p0, const Pair& p1) const {
        return keyLess(p0.key, p1.key);
    }
    bool operator()(const Pair& p, int key) const {
        return keyLess(p.key, key);
    }
    bool operator()(int key, const Pair& p) const {
        return keyLess(key, p.key);
    }
};
std::ostream& operator<<(std::ostream& os, const Pair& pair)
{
    os << "Pair(key: " << pair.key << ", value: " << pair.value << ")";
    return os;
}

void test_map_using_vector()
{
    std::vector<Pair> m;
    m.push_back({1, {1, "aaa", "title-1"}});
    m.push_back({5, {5, "eee", "title-5"}});
    m.push_back({2, {2, "bbb", "title-2"}});
    m.push_back({4, {4, "ddd", "title-4"}});
    m.push_back({3, {3, "ccc", "title-3"}});
    std::sort(m.begin(), m.end(), PairCmp());

    print_elements(m, "m:\n");

    auto p = Pair{};
    p.key = 6;
    auto result = std::binary_search(m.begin(), m.end(), p, PairCmp());
    std::cout << "std::binary_search(" << p.key << "): " << result << "\n";
}

void test()
{
    init();
    print_elements(v, "v:\n");

//    test_binary_search(0);
//    test_binary_search(4);

    //test_lower_bound(3);

    //test_equal_range();
    test_map_using_vector();
}

} // item23 -----------------------------------------------

} // namespace ================================================================

void test_ch_3()
{
    item23::test();
}
