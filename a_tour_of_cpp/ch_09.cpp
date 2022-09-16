//=============================================================================
//  A tour of C++
//  Ch.06  String & Regular expression
//
//  22/09/17
//=============================================================================

#include <string>
#include <iostream>
#include <algorithm>

namespace { //=================================================================

using namespace std::string_literals;

std::string cat(std::string_view v0, std::string_view v1)
{
    auto len = v0.length() + v1.length();
    auto res = std::string(len, 0);
    res.reserve(len);

    std::copy(std::begin(v0), std::end(v0), &res[0]);
    std::copy(std::begin(v1), std::end(v1), &res[v0.length()]);

    return res;
}

void test_cat()
{
    auto res = cat("Hello, "s, "Qt 6.2!!!"s);

    std::cout << "res: " << res << std::endl;
}

} //namespace =================================================================

void test_ch_09()
{
    test_cat();
}
