#include <iostream>
#include <memory>
#include <vector>

namespace { //=================================================================

// ES.60 Avoid new and delete outside resource management functions

struct X
{
    int value;
};

void fBad(int n)
{
    auto p = new X[n];
    delete p;
}
void fGood(int n)
{
    auto p = std::unique_ptr<X[]>(new X[n]);
}

// ES.61 Delete arrays using delete [] and non-arrays using delete

// ES.62 Don't compare pointers into different arrays

// ES.63 Don't slice

// ES.64 Use the T{e} notation for construction

// ES.65 Don't dereference an invalid pointer

// ES.70 Prefer a switch-statement to an if-statement when there is a choice

// ES.71 Prefer a range-for-statement to a for-statment when there is a choice

// ES.72 Prefer a for-statement to a while-statement when there is an obvious loop variable

// ES.73 Prefer a while-statement to a for-statement when there is no obvious loop variable

// ES.74 Prefer to declare a loop variable in the initializer part of a for-statement

// ES.75 Avoid do-statement

// ES.76 Avoid goto

} // namespace ================================================================

void test_es_60()
{
    fBad(5);
    fGood(5);
}

void test_es_71()
{
    auto sl = std::vector<std::string>{"Hello,", "Qt", "6.3"};

    for (auto s : sl) { // bad. expensive
        std::cout << s << " ";
    }
    std::cout << std::endl;

    for (auto &s : sl) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    for (const auto &s : sl) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
}
