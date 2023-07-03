#include <iostream>

using namespace std;

void test_constexpr();
void test_weak_ptr();
void test_exception();
void test_lambda();

int main()
{
#if (0) //done
    test_constexpr();
    test_weak_ptr();
    test_exception();
#endif

    test_lambda();

    return 0;
}
