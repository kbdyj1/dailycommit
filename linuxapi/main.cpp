#include <iostream>
#include <unistd.h>

using namespace std;

void test_ch_5();
void test_ch_6();

int main()
{
    test_ch_6();

    std::cout.flush();
    usleep(10);

    return 0;
}
