#include <iostream>
#include <vector>

using namespace std::literals;

namespace { //=================================================================

// SL.str.1 Use std::string to own character sequences

#if (0)
std::vector<std::string> readUntil(const std::string& terminator)
#else
std::vector<std::string> readUntil(std::string_view terminator) //C++17 readonly
#endif
{
    std::vector<std::string> ret;
    for (std::string s; std::cin >> s && s != terminator; )
        ret.push_back(s);
    return ret;
}

// SL.str.2 Use std::string_view or gsl::span<char> to refer to char sequence

// SL.str.3 Use zstring or czstring to refer to a C-style, zero-terminated, sequence of characters

// SL.str.4 Use char* to refer to a single character
void print(const char* p)
{
    std::cout << p << std::endl;
}

void sl_str_4()
{
    char arr[] = {'Q', 't', '6'};

    print(arr);
}

// SL.str.5 Use std::byte to refer to byte values that do not necessarily represent characters

// SL.str.10 Use std::string when you need to perform locale-sensitive string operations
//  std::string supports standard-library locale facilities

// SL.str.11 Use gsl::span<char> rather than std::string_view when you need to mutate a string

// SL.str.12 Use the s suffix for string literals meant to be standard-library strings

void sl_str_12()
{
    auto p0 = std::make_pair("Tokyo", 9.0); // C-style
    std::pair<std::string, double> p1 = {"Tokyo", 0.0};
    auto p2 = std::make_pair("Tokyo"s, 9.0);//
    std::pair p3 = {"Tokyo"s, 9.0};
}

} //===========================================================================
void test_sl_str()
{
    sl_str_4();
}
