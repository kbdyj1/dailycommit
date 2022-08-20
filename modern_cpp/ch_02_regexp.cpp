#include <iostream>
#include <regex>
#include <string>
#include <codecvt>

using namespace std::literals;

namespace { //=================================================================

#define CHAR_TYPE   1

#if (CHAR_TYPE == 0)
#   define STRINGFY(S...) S##s      // string
#   define CharType       char
#elif (CHAR_TYPE == 1)
#   define STRINGFY(S...) L##S##s   // wstring
#   define CharType       wchar_t
#elif (CHAR_TYPE == 2)
#   define STRINGFY(S...) u##S##s   // u16string
#   define CharType       char16_t
#elif (CHAR_TYPE == 3)
#   define STRINGFY(S...) U##S##s   // u32string
#   define CharType       char32_t
#endif

template <typename Char>
bool isValidEmail(const std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>>& email)
{
    auto pattern = STRINGFY("^[A-Z0-9.%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,}$");
    auto rx = std::basic_regex<Char>{pattern, std::regex::icase};

    return std::regex_match(email, rx);
}

template <typename Char>
inline std::string toString(const std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>>& str)
{
#if CHAR_TYPE == 0
    return str;
#else
    std::wstring_convert<std::codecvt_utf8<CharType>, CharType> converter;
    return converter.to_bytes(str);
#endif
}

void test_email()
{
    std::cout << std::boolalpha;

    auto e0 = STRINGFY("JOHN.DOE@DOMAIN.COM");
    std::cout << toString(e0) << ": " << isValidEmail(e0) << "\n";

    auto e1 = STRINGFY("JOHNDOE@DOMAIN.CO.KR");
    std::cout << toString(e1) << ": " << isValidEmail(e1) << "\n";

    auto e2 = STRINGFY("JOHNDOE@DOMAIN.INFO");
    std::cout << toString(e2) << ": " << isValidEmail(e2) << "\n";

    auto e3 = STRINGFY("J.O.H.N.D.O.E@DOMAIN.COM");
    std::cout << toString(e3) << ": " << isValidEmail(e3) << "\n";

    auto e4 = STRINGFY("ROOT@LOCALHOST");
    std::cout << toString(e4) << ": " << isValidEmail(e4) << "\n";

    auto e5 = STRINGFY("john.doe@domain.com");
    std::cout << toString(e5) << ": " << isValidEmail(e5) << "\n";
}

void test_smatch()
{
    auto pattern = R"(^([A-Z0-9.%+-]+)@([A-Z0-9.-]+)\.([A-Z]{2,})$)";
    auto rx = std::regex{pattern, std::regex::icase};
    auto s = "john.doe@domain.com"s;
    auto result = std::smatch();

    auto success = std::regex_match(s, result, rx);
    if (success) {
        std::cout << "result.size: " << result.size() << "\n";
        for (auto iter=result.begin(); iter!=result.end(); iter++) {
            std::cout << "\t" << iter->str() << "\n";
        }
    } else {
        std::cout << "regex: " << pattern << "\n";
        std::cout << "dest: " << s << "\n";
        std::cout << "match fail.\n";
    }
}

void test_search()
{
    auto s = R"(
timeout=120
server=127.0.0.1

#retrycount=3)"s;

    auto pattern = R"(^(?!#)(\w+)\s*=\s*([\w\d]+[\w\d._,\-:]*)$)"s;
    auto rx = std::regex{pattern};
    auto result = std::smatch{};
    auto count = 0;
#if (1)
    auto e = std::sregex_iterator{};

    for (auto iter=std::sregex_iterator{std::begin(s), std::end(s), rx}; iter!=e; iter++) {
        std::cout << (*iter)[1] << "=" << (*iter)[2] << "\n";
        count++;
    }
#else
    auto e = std::sregex_token_iterator{};
    for (auto iter=std::sregex_token_iterator{std::begin(s), std::end(s), rx}; iter!=e; iter++) {
        std::cout << *iter << "\n";
        count++;
    }
#endif
    std::cout << "count: " << count << "\n";
}

} //===========================================================================

void test_ch_02_regexp()
{
    //test_email();
    //test_smatch();
    test_search();
}
