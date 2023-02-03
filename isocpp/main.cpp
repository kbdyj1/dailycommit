void test_using_final_in_c_to_improve_performance();
void test_deferred_argument_evaluation();
void test_data_member_initialization();
void test_regular_types();
void test_modern_c_in_depth_lambdas();

namespace { //=================================================================
} //===========================================================================

int main()
{
#if (0) //done
    test_using_final_in_c_to_improve_performance();
    test_deferred_argument_evaluation();
    test_data_member_initialization();
    test_regular_types();
#endif

    test_modern_c_in_depth_lambdas();

    return 0;
}
