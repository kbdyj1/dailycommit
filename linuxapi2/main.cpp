#include <iostream>

void test_ch_17();
void test_ch_20();

int main()
{
    std::cout << std::boolalpha;

#if (0) //done
    test_ch_17();
#endif

    test_ch_20();

    return 0;
}
