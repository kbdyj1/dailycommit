#include <iostream>

void test_ch_01();
void test_ch_02();
void test_ch_03();
void test_ch_06();
void test_ch_07();
void test_ch_09();
void test_ch_10(const char*);
void test_ch_11();
void test_ch_12();
void test_ch_13();
void test_ch_15();

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

#if (0) // done
    test_ch_10(argv[0]);
#endif

    test_ch_15();

    return 0;
}
