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

std::string_view getFilename(const std::string_view str)
{
    const auto pos0 = str.find_last_of('\\');
    const auto pos1 = str.find_first_of('.');
    return str.substr(pos0+1, pos1-pos0-1);
}

std::string_view trim_string_view(std::string_view str)
{
    const auto pos0 = str.find_first_not_of(' ');
    const auto pos1 = str.find_last_not_of(' ');
    return str.substr(pos0, pos1-pos0+1);
}

void test_string_view()
{
#if (0)
    auto file0 = R"(c:\windows\example.doc)"s;
    auto filename0 = getFilename(file0);
#else
    auto file0 = R"(c:\windows\example.doc)";
    auto filename0 = getFilename(std::string_view{file0, 22});
#endif
    std::cout << filename0 << "\n";

    auto str = "   Hello, Qt6  ";
    auto result = trim_string_view(str);

    std::cout << "trim: " << result << "\n";
}

} //namespace =================================================================

void test_ch_02_string()
{
    //test_tolower();
    //test_trim();
    //test_reverse();
    //test_remove_if();
    test_string_view();
}
