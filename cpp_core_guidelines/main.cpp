#include <iostream>

using namespace std;

// Philosophy -----------------------------------------------------------------

void test4();
void test5();
void test7();
void test9();

void test_philosophy()
{
    //test5();
    //test7();
    test9();
}

// Interface ------------------------------------------------------------------

void test_i_3();
void test_i_4();
void test_i_7();
void test_i_13();
void test_i27();

void test_interface()
{
    //test_i_3();
    //test_i_4();
    //test_i_7();
    //test_i_13();
    test_i27();
}

// Function -------------------------------------------------------------------

void test_f_1();
void test_f_4();
void test_f_7();
void test_f_17();
void test_f_20();
void test_f_21();
void test_f_24();
void test_f_54();

void test_function()
{
    //test_f_1();
    //test_f_4();
    //test_f_7();
    //test_f_17();
    //test_f_20();
    //test_f_21();
    //test_f_24();
    test_f_54();
}

// Class ----------------------------------------------------------------------

void test_c_7();
void test_c_9();
void test_c_10();
void test_c_11();
void test_c_21();
void test_c_30();
void test_c_35();
void test_c_40();
void test_c_41();
void test_c_43();
void test_c_45();

void test_class()
{
    //test_c_7();
    //test_c_9();
    //test_c_10();
    //test_c_11();
    //test_c_21();
    //test_c_30();
    //test_c_35();
    //test_c_40();
    //test_c_41();
    //test_c_43();
    test_c_45();
}

// Main =======================================================================

int main()
{
    std::cout << std::boolalpha << std::endl;

    //test_philosophy();
    //test_interface();
    //test_function();
    test_class();

    std::cout << std::endl;

    return 0;
}
