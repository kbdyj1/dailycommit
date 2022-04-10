#include <iostream>

using namespace std;

// Philosophy
void test4();
void test5();
void test7();
void test9();

// Interface
void test_i_3();
void test_i_4();
void test_i_7();
void test_i_13();
void test_i27();

// Function
void test_f_1();
void test_f_4();
void test_f_7();
void test_f_17();
void test_f_20();
void test_f_21();
void test_f_24();

void test_f_54();

void test()
{
    //test5();
    //test7();
    //test9();

    //test_i_3();
    //test_i_4();
    //test_i_7();
    //test_i_13();
    //test_i27();

    //test_f_1();
    //test_f_4();
    //test_f_7();
    //test_f_17();
    //test_f_20();
    //test_f_21();
    //test_f_24();
    test_f_54();
}

int main()
{
    std::cout << std::boolalpha << std::endl;

    test();

    std::cout << std::endl;

    return 0;
}
