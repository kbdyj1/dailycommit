#include <iostream>
#include <unistd.h>

using namespace std;

namespace { //================================================================
} //==========================================================================

void test_ch_01();
void test_ch_02();

int main()
{
    auto pid = getpid();

    cout << "\nData structure & algorithm with C++\npid: " << pid << endl;

    test_ch_02();

    return 0;
}
