#include <iostream>
#include <optional>
#include <vector>

namespace { //=================================================================

namespace _1 {

template <typename T1, typename F>
auto transform(const std::optional<T1>& opt, F f) -> decltype(std::make_optional(f(opt.value())))
{
    if (opt) {
        return std::make_optional(f(opt.value()));
    } else {
        return F();
    }
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_ch_10()
{

}
