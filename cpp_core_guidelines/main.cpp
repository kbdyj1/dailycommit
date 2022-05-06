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
void test_c_49();
void test_c_50();
void test_c_52();
void test_c_60();
void test_c_64();
void test_c_67();
void test_c_80();
void test_c_82();
void test_c_83();
void test_c_101();
void test_c_102();
void test_c_104();
void test_c_127();
void test_c_130();
void test_c_137();
void test_c_138();
void test_c_140();
void test_c_146();
void test_c_147();
void test_c_152();
void test_c_164();
void test_c_165();
void test_c_166();
void test_c_180();
void test_c_181();
void test_c_183();

void test_class()
{
    test_c_183();
}

// Resource -------------------------------------------------------------------

void test_r_1();
void test_r_3();
void test_r_4();
void test_r_10();
void test_r_14();
void test_r_24();
void test_r_32();
void test_r_34();
void test_weak_ptr();
void test_r_35();
void test_r_36();
void test_r_37();

void test_resource()
{
    test_r_37();
    //test_weak_ptr();
}

// Expressions and Statements -------------------------------------------------
void test_es_6();
void test_es_7();

void test_expressions_and_statements()
{
    test_es_7();
}

// Main =======================================================================

int main()
{
    std::cout << std::boolalpha << std::endl;

    //test_philosophy();
    //test_interface();
    //test_function();
    //test_class();
    //test_resource();
    test_expressions_and_statements();

    std::cout << std::endl;
    std::flush(std::cout);

    return 0;
}
