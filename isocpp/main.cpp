#include <iostream>

void test_using_final_in_c_to_improve_performance();
void test_deferred_argument_evaluation();
void test_data_member_initialization();
void test_regular_types();
void test_modern_c_in_depth_lambdas();
void test_numbers_are_not_easy();
void test_initialize_list();
void test_movable_primitive_types();
void test_static_memory();

namespace { //=================================================================
} //===========================================================================

#define CHECK_CXX_COMPILER_VERSION

int main()
{
#ifdef CHECK_CXX_COMPILER_VERSION
    std::cout << "__cplusplus: " << __cplusplus << '\n';
#endif

#if (0) //done
    test_using_final_in_c_to_improve_performance();
    test_deferred_argument_evaluation();
    test_data_member_initialization();
    test_regular_types();
    test_modern_c_in_depth_lambdas();
    test_numbers_are_not_easy();
    test_initialize_list();
    test_movable_primitive_types();
#endif

    test_static_memory();

    return 0;
}
