//=============================================================================
//  Modern C++ Programming Cookbook
//  Ch.9 Robustness & Performance
//
//  22/09/07
//=============================================================================

void test_ch_09_exception();
void test_ch_09_const_correctness();
void test_ch_09_cast();
void test_ch_09_resource();

void test_ch_09()
{
#if (0) // done
    test_ch_09_exception();
    test_ch_09_const_correctness();
    test_ch_09_cast();
#endif

    test_ch_09_resource();
}
