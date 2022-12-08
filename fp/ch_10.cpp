#include <iostream>
#include <optional>
#include <vector>

namespace { //=================================================================

template <typename T1, typename F>
auto transform(const std::optional<T1>& opt, F f) -> decltype(std::make_optional(f(opt.value())))
{
    if (opt) {
        return std::make_optional(f(opt.value()));
    } else {
        return {};
    }
}

namespace _1 {

void test()
{
    auto f = [](int value) { return value * 2; };
    auto param = std::optional<int>();
    auto r = transform(param, f);
    if (r) {
        std::cout << r.value() << "\n";
    } else {
        std::cout << "-\n";
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

std::string user_name(const std::string& login_name)
{
    auto at = login_name.find("@");
    auto len = at == std::string::npos ? login_name.length() : at;

    return login_name.substr(0, len);
}

std::string to_html(const std::string& user_name)
{
    auto ret = std::string{"<b>"};
    ret += user_name;
    ret += "</b>";
    return ret;
}

void test()
{
    auto login_name = std::optional<std::string>{"kbdyj1@github.com"};
    auto ret = transform(transform(login_name, user_name), to_html);

    std::cout << " -> " << ret.value() << "\n";
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_ch_10()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
