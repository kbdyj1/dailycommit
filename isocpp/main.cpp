void test_using_final_in_c_to_improve_performance();
void test_deferred_argument_evaluation();
void test_data_member_initialization();
void test_regular_types();

namespace { //=================================================================
} //===========================================================================

int main()
{
#if (0) //done
    test_using_final_in_c_to_improve_performance();
    test_deferred_argument_evaluation();
    test_data_member_initialization();
#endif

    test_regular_types();

    return 0;
}
