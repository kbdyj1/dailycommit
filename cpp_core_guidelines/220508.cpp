#include <iostream>
#include <array>
#include <stack>
#include <cstdarg>

namespace { //=================================================================

// ES.26 Don't use a variable for two unrelated purpose

// ES.27 Use std::array or stack_array for arrays on the stack

const int N = 7;
int M = 9;

void f()
{
#if (0)
    int a0[N];
    int a1[M];  // not ISO C++
#else
    auto a0 = std::array<int, N>();
#endif
}

// ES.28 Use lambdas for complex initialization, especially of const variables

class Widget
{
    int mSum{0};

public:
    void operator+=(int value)
    {
        mSum += value;
    }
    int sum() const
    {
        return mSum;
    }
};

void test_es_28_bad()
{
    auto x = Widget{};
    for (auto i=0; i<N; i++)
        x += i;
}

void test_es_28_good()
{
    const Widget x = [&]{
        auto ret = Widget{};
        for (auto i=0; i<N; i++)
            ret += i;
        return ret;
    }();

    std::cout << "Widget.sum: " << x.sum() << std::endl;
}

// ES.30 Don't use macros for program text manipulation

enum E
{
    E_Ok = 0,
    E_Error
};

template <int X>
constexpr const char* stringfy()
{
    switch (X)
    {
    case E_Ok: return "OK";
    case E_Error: return "Error";
    }
}

// ES.31 Don't use macros for constants or "functions"

// ES.32 Use ALL CAPS for all macros names

// ES.33 If you must use macros, give them unique names

// ES.34 Don't define a (C-Sytle) variadic function

#if (0)
void error(int serverity ...)
{
    va_list ap;
    va_start(ap, serverity);

    for (;;) {
        char* p = va_arg(ap, char*);
        if (!p)
            break;
        std::cerr << p << ' ';
    }

    va_end(ap);
    std::cerr << std::endl;

    if (serverity)
        exit(serverity);
}
#else
void error(int serverity)
{
    std::cerr << std::endl;
    std::exit(serverity);
}

template <typename T, typename... TS>
constexpr void error(int serverity, T head, TS... tail)
{
    std::cout << head << " ";
    error(serverity, tail...);
}

void use()
{
    //error(7);
    error(5, "This", "is", "not", "an", "error");
    std::string an = "an";
    error(7, "This", "is", "not", an, "error");
    error(5, "oh", "no", nullptr);
}
#endif

} // namespace ================================================================

void test_es_28()
{
    test_es_28_good();
}

void test_es_30()
{
    std::cout << stringfy<0>() << std::endl;
}

void test_es_34()
{
    use();
}
