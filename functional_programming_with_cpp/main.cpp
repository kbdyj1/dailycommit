#include <iostream>
#include <unistd.h>

namespace { //=================================================================

} //namespace =================================================================

void test_ch_01();
void test_ch_02();
void test_ch_03();
void test_ch_04();
void test_ch_05();
void test_ch_06();
void test_ch_08();
void test_ch_09();
void test_ch_10();
void test_ch_11();
void test_ch_12();
void test_ch_13();
void test_ch_14();
void test_ch_15();

int main()
{
    std::cout << std::boolalpha;
    std::cout << "pid: " << getpid() << "\n" << std::endl;

    test_ch_15();

    return 0;
}
