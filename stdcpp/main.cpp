#include <iostream>

using namespace std;

void test_constexpr();
void test_weak_ptr();

int main()
{
#if (0) //done
    test_constexpr();
#endif

    test_weak_ptr();

    return 0;
}
