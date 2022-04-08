#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <span>

namespace { //=================================================================

// F21. To return multiple "out" values, prefer returning a struct or tuple

void test_cxx98()
{
    auto s = std::set<std::string>{};
    auto res = s.insert("Hello, Qt6");
    if (res.second) {
        std::cout << "res.first: " << *res.first << std::endl;
    }
}

void test_cxx11()
{
    auto s = std::set<std::string>{};
    bool ok;
    std::set<std::string>::iterator iter;

    std::tie(iter, ok) = s.insert("Hello, Qt6");
    std::string str;
    if (ok) {
        str = *iter;
    }
    std::cout << "tie(iter, ok) : " << str << ", " << ok << std::endl;
}

void test_cxx17()
{
    auto s = std::set<std::string>{};
    if (auto [iter, ok] = s.insert("Hello, Qt6"); ok) {
        std::cout << "[iter, ok] : " << *iter << ", " << ok << std::endl;
    }
}

struct Distance {
    int value;
    int unit;
};
Distance getDistance()
{
    return {10, 1};
}

// F22. Use T* or owner<T> to designate a single object

// F23. Use a not_null<T> to indicate that "null" is not a valid value

// F24. Use a span<T> or a span_p<T> to designate a half-open sequence

void f(std::span<int> s)
{
    for (int x : s) {
        std::cout << x << std::endl;
    }
    std::cout << "s[5] : " << s[5] << std::endl;
}

// F25. Use a zstring or a not_null<zstring> to designate a C-style string

// F26. Use a unique_ptr<T> to transfer ownership where a pointer is needed

// F27. Use a shared_ptr<T> to share ownership

} // namespace ================================================================

void test_f_21()
{
    test_cxx98();
    test_cxx11();
    test_cxx17();

    auto d0 = getDistance();
    auto [value, unit] = getDistance();
    auto [x, y] = getDistance(); // Don't. confusing.

    std::cout << "getDistance : " << x << ", " << y << std::endl;
}

void test_f_24()
{
    int array[] = { 1, 2, 3, 4, 5 };
    auto s = std::span<int>(array);
    f(s);
}
