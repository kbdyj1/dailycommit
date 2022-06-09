#include <iostream>
#include <unistd.h>

using namespace std;

void test_ch_5();

int main()
{
    test_ch_5();

    std::cout.flush();
    usleep(1000);

    return 0;
}
