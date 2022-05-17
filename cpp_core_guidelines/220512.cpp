#include <iostream>
#include <vector>

namespace { //=================================================================

// ES.77 Minimize the use of break and continue in loops

void f0()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    int value;
    int condition;
    for (const auto i : v) {
        if (condition) {
            value = i;
            break;
        }
    }
}

int f1()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    int condition;
    for (const auto i : v) {
        if (condition) {
            return i;
        }
    }
    return int{};
}

// ES.78 Don't rely on implicit fallthrough in switch statement

enum LogLevel
{
    Information = 0,
    Warning,
    Error
};

void test_es_78()
{
    auto level = Warning;
    auto message = std::string{"Hello, Qt6"};

    switch (level)
    {
    case Information:
        std::cout << "[Info] " << message << std::endl;
        break;

    case Warning:
        std::cout << "[warning] " << message << std::endl;
        //[[fallthrough]]

    case Error:
        std::cout << "[Error] " << message << std::endl;
        break;
    }
}

// ES.79 Use default to handle common cases (only)

// ES.84 Don't try to declare a local variable with no name

// ES.85 Make empty statement visible

int f(int i)
{
    return i*2;
}
void test_es_85()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    int i;
#if (0) // bad
    for (i=0; i<5; i++);
    v[i] = f(v[i]);
#else
    i = 0;
    for (auto i : v) {
        // nothing
    }
    v[i] = f(v[i]);
#endif
}

// ES.86 Avoid modifying loop control variables inside the body of raw for-loops

// ES.87 Don't add redundant == or != to conditions

void test_es_86()
{
    std::vector<std::string> v;
    for (std::string s; std::cin >> s; ) {
        v.push_back(s);
    }
}

} // namespace ================================================================
