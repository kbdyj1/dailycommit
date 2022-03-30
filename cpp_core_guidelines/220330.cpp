#include <iostream>
#include <vector>
#include <span>
#include <memory>
#include <fstream>

namespace { //=================================================================

// P6. What cannot be checked at compile time should be checkable at run time

void f(int *p)
{}

void g(int n)
{
    f(new int[n]);
}

std::vector<int> f0(int n)
{
    auto v = std::vector{n};

    return v;
}

std::unique_ptr<int[]> f1(int n)    // bad. loose n
{
    auto p = std::make_unique<int[]>(n);

    return p;
}

// P7. Catch run-time error early

void inc0(int*p, int n)
{
    for (auto i=0; i<n; i++) {
        ++p[i];
        std::cout << p[i] << " ";
    }
    std::cout << std::endl;
}

void inc1(std::span<int> p)
{
    for (int& x : p) {
        ++x;
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

// P8. Don't leak any resources

void g0(const char* name)
{
    FILE* input = fopen(name, "r");

    int condition = 0;

    if (condition == 0)
        return;

    fclose(input);
}

void g1(const char* name)
{
    std::ifstream in{name};

    int condition = 0;

    if (condition == 0)
        return;
}

} // namespace ================================================================

void test7()
{
    const int n = 10;
    const int m = 15;
    int a[n] = {};

    //inc0(a, m);

    // 1 1 1 1 1 1 1 1 1 1 -268775679 -1088870821 -8431 32768 1431670661
    // *** stack smashing detected ***: terminated
    //inc1({a, m});

    inc1(a);
}
