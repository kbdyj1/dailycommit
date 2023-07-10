#include <iostream>

using namespace std;

void test_constexpr();
void test_weak_ptr();
void test_exception();
void test_lambda();
void test_fileio();
void test_macro();
void test_move();
void test_vtemplate();

int main()
{
#if (0) //done
    test_constexpr();
    test_weak_ptr();
    test_exception();
    test_lambda();
    test_fileio();
    test_macro();
    test_move();
#endif

    test_vtemplate();

    return 0;
}
