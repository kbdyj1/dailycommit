#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <iterator>

namespace { //=================================================================

class A
{
public:
    using Filter = bool(*)(int value);

    void addFilter() {
#if (1)
        mFilters.emplace_back([=](int value) {
            return value % mDivider;
        });
#else
        //19:28: error: ‘this’ was not captured for this lambda function
        //   19 |             return value % mDivider;
        //      |                            ^~~~~~~~
        mFilters.emplace_back([](int value) -> bool {
            return value % mDivider;
        });
#endif
    }
private:
    int mDivider = 1;
    std::vector<std::function<bool(int)>> mFilters;
};

void test0()
{
    auto a = A{};

    a.addFilter();
}

class B
{
public:
    bool isValid() { std::cout << "isValid" << std::endl; return true; }
    bool isRun() { std::cout << "isRun" << std::endl; return true; }
    bool isArchived() { std::cout << "isArchived" << std::endl; return true; }
};

void test1()
{
#if (0)
    auto b = std::make_unique<B>();
    auto f = [pb = std::move(b)]{
        return pb->isValid() && pb->isArchived();
    };
#else
    auto f = [pb = std::make_unique<B>()]{
        return pb->isValid() && pb->isArchived();
    };
#endif
    f();
}

class IsValidAndArchived
{
public:
    using DataType = std::unique_ptr<B>;

    explicit IsValidAndArchived(DataType&& ptr) : pw(std::move(ptr))
    {}

    bool operator()() const {
        return pw->isValid() && pw->isArchived();
    }
private:
    std::unique_ptr<B> pw;
};

void test2()
{
    auto func = IsValidAndArchived(std::make_unique<B>());

    func();
}

void test3()
{
    auto v = std::vector<double>{1.0, 2.0, 3.0};

    // C++14
    auto f = [v = std::move(v)]{
        std::copy(std::begin(v), std::end(v), std::ostream_iterator<double>(std::cout, " "));
    };

    f();
}

void test4()
{
    auto v = std::vector<double>{1.0, 2.0, 3.0};

    // C++11
    auto f = std::bind([](const std::vector<double>& v){
        std::copy(std::begin(v), std::end(v), std::ostream_iterator<double>(std::cout, " "));
    }, std::move(v));

    f();
}

void test5()
{
    auto v = std::vector<double>{1.0, 2.0, 3.0};

#if (0)
    auto func = [v = std::move(v)]{
        std::cout << "using lambda" << std::endl;
        std::copy(std::begin(v), std::end(v), std::ostream_iterator<double>(std::cout, " "));
    };
#else
    #define MAKE_MUTABLE

    // using c++ bind
    auto func = std::bind(
    #ifdef MAKE_MUTABLE
        [](std::vector<double>& v) mutable {
    #else
        [](const std::vector<double>& v){
    #endif
        std::cout << "using std::bind()" << std::endl;
        std::copy(std::begin(v), std::end(v), std::ostream_iterator<double>(std::cout, " "));
    },
    std::move(v));
#endif
    func();
}

template <typename T>
T normalize(T x) {
    std::cout << "normalize(" << x << ")" << std::endl;
    return x;
}
void test6()
{
    auto f = [](auto&& x) {
        return normalize(std::forward<decltype(x)>(x));
    };
    f(3);
    f(std::string("Qt6"));
    f(3.141592);
}

using Time = std::chrono::steady_clock::time_point;
enum class Sound { Beep, Silen, Whistle };
using Duration = std::chrono::steady_clock::duration;

void setAlarm(Time t, Sound s, Duration d)
{
}

void test7()
{

}

} // namespace ================================================================

void test_lambda()
{
    //test0();
    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    test6();
}
