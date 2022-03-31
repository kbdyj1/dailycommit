#include <iostream>
#include <vector>
#include <span>
#include <memory>
#include <string>
#include <cstring>

namespace { //=================================================================

// P9. Don't waste time or space

struct A
{
    char c0;
    int i;
    std::string s;
    char c1;
};

A waste(const char* p)
{
    if (!p)
        throw std::exception();

    auto n = strlen(p);
    auto buf = new char[n];
    if (!buf)
        throw std::bad_alloc();

    for (auto i=0; i<n; i++)
        buf[i] = p[i];

    A a;
    a.c0 = 'a';
    a.s = std::string(n, ' ');
    for (auto i=0; i<a.s.size(); i++)
        a.s[i] = buf[i];

    delete [] buf;
    return a;
}

int slen(const char *p)
{
    static int count = 0;

    std::cout << "slen() : " << count << std::endl;
    ++count;

    return strlen(p);
}

void lower(char *p)
{
    for (auto i = 0; i<slen(p); ++i)
        p[i] = (char)tolower(p[i]);
}

// P10. Prefer immutable data to mutable data

// P11. Encapsulate messy constructs, rather than spreading throught the code

void p11_bad()
{
    auto size = 100;
    auto* p = (int*)malloc(sizeof(int) * size);
    auto count = 0;
    auto x = 0;

    for ( ;; ) {
        if (count == size)
            p = (int*)realloc(p, sizeof(int)*size*2);

        p[count++] = x++;
    }
}

void p11_good()
{
    auto v = std::vector<int>{};
    auto x = 0;

    for (auto i=0; i<100; i++) {
        v.push_back(x++);
    }
}

// P12. Use supporting tools as appropriate
//      Static analysis tools
//      Concurrency tools
//      Testing tools

// P13. Use support libraries as appropriate

} // namespace ================================================================

void test9()
{
    auto a = waste("Hello, Qt6");

    std::cout << a.s << std::endl;

    char title[] = "Hello, Qt6";
    lower(title);

    std::cout << title << std::endl;
}
