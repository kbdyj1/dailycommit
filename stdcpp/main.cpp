#include <iostream>

using namespace std;

void test_constexpr();
void test_weak_ptr();
void test_exception();

int main()
{
#if (0) //done
    test_constexpr();
    test_weak_ptr();
#endif

    test_exception();

    return 0;
}
