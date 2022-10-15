#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

namespace { //=================================================================

namespace _1 {

template <typename Iter>
Iter findNegative(Iter b, Iter e)
{
#if __cplusplus < 201402L
    return std::find_if(b, e, [](typename std::iterator_traits<Iter>::value_type value){
        return value < 0;
    });
#else
    return std::find_if(b, e, [](auto value) {
        return value < 0;
    });
#endif
}

void test()
{
    auto v = std::vector<int>{ 1, 2, -3, 4, 5 };
    auto iter = findNegative(v.begin(), v.end());
    if (iter != v.end()) {
        std::cout << "-? " << *iter;
    }

    auto lambda = []{ return 0; };
    std::cout << "std::is_class_v<decltype(lambda)>: " << std::is_class_v<decltype(lambda)> << "\n";
}

//Mock lambda class to capture local variables
class CompilerGenerateLambda {
    int x;
    int y;

public:
    CompilerGenerateLambda(int _0, int _1)
        : x(_0)
        , y(_1)
    {}
    bool operator()(int value) const {
        return x<value && value<y;
    }
};

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename Func, typename... Args>
void invoke(Func func, Args... args)
{
    func(args...);
}

void test()
{
    invoke([](auto x, auto y){
        std::cout << "x + y: " << x+y << "\n";
    }, 1, 2);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_lambda()
{
#if (0) //done
    _1::test();
#endif
    _2::test();
}
