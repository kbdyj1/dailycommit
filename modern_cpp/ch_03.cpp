//=============================================================================
//  Modern C++ Programming Cookbook
//  Ch.3 Function
//
//  22/08/21
//=============================================================================

#include <iostream>

namespace { //=================================================================

struct Data {};

class A {
    Data* pData;

public:
    A(Data* data) : pData(data)
    {
        std::cout << "A(pData)\n";
    }
    virtual ~A()
    {
        std::cout << "~A()\n";
        delete pData;
    }

    // disable copy
    A(const A&) = delete;
    A& operator=(const A&) = delete;

    // movable
    A(A&& a) : pData(a.pData) {
        std::cout << "A(A&&)\n";
        a.pData = nullptr;
    }
    A& operator=(A&& a) {
        std::cout << "A& operator=(A&&)\n";
        if (this != &a) {
            pData = a.pData;
            a.pData = nullptr;
        }
        return *this;
    }
};

void test_default_and_delete()
{
    A a0(new Data{});
#if (0)
    A a1(a0);
#else
    A a1(A(new Data));
    a1 = A(new Data);
#endif
}

template <typename T>
void run(T value) = delete;

void run(long value)
{
    std::cout << "run(" << value << ": [" << typeid(value).name() << "])\n";
}

void test_run()
{
    long lValue = 10;
    run(lValue);

#if (0) //error: use of deleted function ‘void {anonymous}::run(T) [with T = int]’
    unsigned long ulValue = 20;
    run(ulValue);

    auto aValue = 30;
    run(aValue);
#endif
}

} //namespace =================================================================

void test_ch_03_lambda();
void test_ch_03_vargs();

void test_ch_03()
{
    //test_default_and_delete();
    //test_run();
    //test_ch_03_lambda();
    test_ch_03_vargs();
}
