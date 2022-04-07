#include <iostream>
#include <vector>
#include <iterator>

namespace { //=================================================================

// F15. Prefer simple and conventional ways of passing information

// F16. For "in" parameters, pass cheaply-copied types by value and other by reference to const

// F17. For "in-out" parameters, pass by reference to non-const

void f(std::string& s)
{
    s = "Hello, Qt6 !!!";
}

void g()
{
    std::string buf = ".......................................................";
    f(buf);

    std::cout << buf << std::endl;
}

// F18. For "will-move-from" parameters, pass by X&& and std::move the parameter

// F19. For "forward" parameters, pass by TP&& and only std::forward the parameter

// F20. For "out" output variables, prefer return values to output parameters

std::vector<int> findGood(const std::vector<int>&v, int x)
{
    std::vector<int> ret;

    for (auto iter=std::begin(v); iter != std::end(v); iter++) {
        if (*iter == x) {
            ret.push_back(x);
        }
    }

    return ret;
}

void findBad(const std::vector<int>& v, int x, std::vector<int>& ret)
{
    for (auto iter=std::begin(v); iter != std::end(v); iter++) {
        if (*iter == x) {
            ret.push_back(x);
        }
    }
}

const std::vector<int> a()
{
    return std::vector<int>{ 1, 2, 3, 4, 5 };
}

void b(std::vector<int>& out)
{
    //a() = out;    // prevented by the "const"

    out = a();  // expensive copy. move semantics suppressed by the "const"

    std::copy(std::begin(out), std::end(out), std::ostream_iterator<int>(std::cout, " "));
}

} // namespace ================================================================

void test_f_17()
{
    g();
}

void test_f_20()
{
    auto in = std::vector<int>{ 1, 2, 1, 2, 1, 3 };

#if (0)
    auto out = findGood(in, 1);
#else
    auto out = std::vector<int>{};
    findBad(in, 2, out);
#endif
    std::copy(std::begin(out), std::end(out), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    out.clear();

    b(out);

    std::cout << std::endl;
}
