#include <iostream>

namespace { //=================================================================

template <typename F, typename... Params>
void forwardCopy(F f, const Params&... params)
{
    f(Params(params)...);
}

template <typename... Types>
class Nested {
    template <typename... InnerTypes>
    void f(const InnerTypes&... innerValues) {
        g(Types(InnerTypes(innerValues)...)...);
    }
};

} //namespace =================================================================

void test_ch_12_template_parameter_pack()
{

}
