#include <iostream>
#include <string>
#include <algorithm>

using namespace std::literals;

namespace { //=================================================================

void test_tolower()
{
    auto s = "Hello, Qt6"s;
#if (1)
    std::transform(std::begin(s), std::end(s), std::begin(s), tolower);
#else
    std::transform(std::begin(s), std::end(s), std::begin(s), toupper);
#endif

    std::cout << s << "\n";
}

void test_trim()
{
    auto s = "   Hello, Qt 6  "s;
    auto b = s.find_first_not_of(" ");
    auto e = s.find_last_not_of(" ");

    std::cout << s.substr(b, e-b+1) << "\n";
}

void test_reverse()
{
    auto s = "Hello, Qt6"s;
    std::reverse(std::begin(s), std::end(s));
    std::cout << s << "\n";
}

void test_remove_if()
{
    auto s = "Hello, Qt6"s;
    auto b = std::remove_if(std::begin(s), std::end(s), [](char c){
        return c == 'l';
    });
    s.erase(b, s.end());

    std::cout << s << "\n";
}

} //namespace =================================================================

void test_ch_02_string()
{
    //test_tolower();
    //test_trim();
    //test_reverse();
    test_remove_if();
}
