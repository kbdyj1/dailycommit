#include <iostream>

void test_ch_17();
void test_ch_20(int argc, const char** argv);
void test_ch_21(int argc, const char** argv);

int main(int argc, const char** argv)
{
    std::cout << std::boolalpha;

#if (0) //done
    test_ch_17();
    test_ch_20(argc, argv);
#endif

    test_ch_21(argc, argv);

    return 0;
}
