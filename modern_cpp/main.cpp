//=============================================================================
//  Modern C++ Programming Cookbook
//
//  Start:  22/08/14
//  End:
//=============================================================================

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void test_ch_01();
void test_ch_02();
void test_ch_03();
void test_ch_04();
void test_ch_05();
void test_ch_06();
void test_ch_07();
void test_ch_08();  // threading & concurrency
void test_ch_09();  // Robustness & Performance

int main()
{
    test_ch_09();

    std::this_thread::sleep_for(10ms);

    return 0;
}
