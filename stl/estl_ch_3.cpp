#include <set>
#include <iostream>
#include <algorithm>
#include <iterator>

namespace { //=================================================================

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

struct Employee {
    int id;
    std::string name;
    std::string title;
};

struct IdCmpLess : public std::binary_function<const Employee&, const Employee&, bool>
{
    bool operator()(const Employee& e0, const Employee& e1) const {
        return e0.id < e1.id;
    }
};

void test()
{
    std::set<Employee, IdCmpLess> s;

    s.insert({2, "ccc", "worker"});
    s.insert({1, "bbb", "manager"});
    s.insert({3, "aaa", "vip"});

    for (auto iter=s.begin(); iter!=s.end(); iter++) {
        std::cout << "id: " << iter->id << ", name: " << iter->name << ", title: " << iter->title << "\n";
    }

    auto key = 3;
    auto iter = std::find_if(s.begin(), s.end(), [key](const Employee& e) {
        return key == e.id;
    });
    if (iter != s.end()) {
        const_cast<Employee&>(*iter).title = "programmer";
        std::cout << "id: " << iter->id << ", name: " << iter->name << ", title: " << iter->title << "\n";
    }
}

} // item22 -----------------------------------------------

} // namespace ================================================================

void test_ch_3()
{
    item22::test();
}
