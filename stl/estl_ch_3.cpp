#include <set>
#include <iostream>

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

} // namespace ================================================================

void test_ch_3()
{
    item19::test();
}
