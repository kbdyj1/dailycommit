#include <iostream>
#include <vector>

namespace { //=================================================================

class A
{
public:
    A(const int n) : mN(n)
    {
        std::cout << "A(const int)" << std::endl;
    }
    A(const A& rhs) : mN(rhs.mN)
    {
        std::cout << "A(const A&)" << std::endl;
    }
    A& operator=(const A& rhs)
    {
        std::cout << "operator=(const A&)" << std::endl;
        if (this != &rhs) {
            mN = rhs.mN;
        }
        return *this;
    }
    A(A&& rhs) : mN(std::move(rhs.mN))
    {
        std::cout << "A(A&&)" << std::endl;
    }
    ~A()
    {
        std::cout << "~A()" << std::endl;
    }

private:
    int mN;
};

void test0()
{
    auto v = std::vector<A>{};

    std::cout << "call push_back() ------------------------------------------" << std::endl;
    auto a = A{1};
    v.push_back(a);

    std::cout << std::endl;

    std::cout << "call emplace_back() ---------------------------------------" << std::endl;
    v.emplace_back(3);
    std::cout << std::endl;
}

} // namespace ================================================================

void test_emplace_back()
{
    test0();
}
