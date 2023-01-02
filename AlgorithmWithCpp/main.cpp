#include <iostream>
#include <unistd.h>

using namespace std;

void test_ch_01();

int main()
{
    auto pid = getpid();

    cout << "\nData structure & algorithm with C++\npid: " << pid << endl;

    test_ch_01();

    return 0;
}
