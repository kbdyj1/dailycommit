//=============================================================================
//  A tour of C++
//  Ch.06  String & Regular expression
//
//  22/09/17
//=============================================================================

#include <string>
#include <iostream>
#include <algorithm>
#include <regex>

namespace { //=================================================================

using namespace std::string_literals;
using namespace std::string_view_literals;

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
    auto res = cat("Hello, "sv, "Qt 6.2!!!"sv);

    std::cout << "res: " << res << std::endl;
}

void test_regex()
{
    {
        auto rx = std::regex{R"(\w{2}\s*\d{5}(-\d{4})?)"};
        auto s = std::string{"address: seoul aa12345 korea"};
        auto matches = std::smatch{};
        if (std::regex_search(s, matches, rx)) {
            std::cout << "regex_search(): " << matches[0] << "\n";
        }
    }
    {
        auto rx = std::regex{R"((ab)+?)"};
        auto s = std::string{R"(abababbabbbbaaaaabba)"s};
        auto matches = std::smatch{};
        auto index = 0;
        if (std::regex_search(s, matches, rx)) {
            for (auto iter = std::begin(matches); iter != std::end(matches); iter++) {
                std::cout << "[" << index++ << "]: " << *iter << "\n";
            }
        }
    }
    std::cout << std::endl;
}

bool is_identifier(const std::string& s)
{
    auto rx = std::regex{R"([_[:alpha:]][_[:alnum:]]*)"};

    return std::regex_match(s, rx);
}

void test_is_identifier()
{
    auto var0 = std::string("mValue2-2");
    auto var1 = std::string("is_identifier");

    std::cout << std::boolalpha;
    std::cout << "is_identifier(" << var0 << "): " << is_identifier(var0) << "\n";
    std::cout << "is_identifier(" << var1 << "): " << is_identifier(var1) << "\n";
}

void test_sub_match()
{
    auto s = std::string{",,,12121    :23,4567890  "};
    {
#if (0)
        auto rx = std::regex{R"((\s|:|,)*(\d*))"};
#else
        auto rx = std::regex{R"((?:\s|:|,)*(\d*))"};
#endif
        auto results = std::smatch{};
        if (std::regex_search(s, results, rx)) {
            std::cout << "results.size(): " << results.size() << "\n";
            for (const auto& result : results) {
                std::cout << result << "\n";
            }
        }
    }
    std::cout << std::endl;
}

void test_xml_pattern()
{
    auto s = std::string{R"(Always look on the <b>bright</b> side of <b>life</b>.)"};
    auto rx = std::regex{R"(<(.*?)>(.*?)</\1>)"};

#if (0)
    auto results = std::smatch{};

    if (std::regex_search(s, results, rx)) {
        for (const auto& result : results) {
            std::cout << result << "\n";
        }
    }
#else
    auto iter = std::sregex_iterator(s.begin(), s.end(), rx);
    for ( ; iter != std::sregex_iterator{}; iter++) {
        std::cout << (*iter)[0] << "\n";
    }
#endif
}

} //namespace =================================================================

void test_ch_09()
{
#if (0) // done
    test_cat();
    test_regex();
    test_is_identifier();
    test_sub_match();
#endif

    test_xml_pattern();
}
