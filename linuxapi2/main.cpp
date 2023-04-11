#include <iostream>

namespace { //=================================================================
} //namespace =================================================================

void test_ch_17();
void test_ch_20(int argc, const char** argv);
void test_ch_21(int argc, const char** argv);
void test_ch_22(int argc, const char* argv[]);
void test_ch_23(int argc, const char* argv[]);
void test_ch_24(int argc, const char* argv[]);
void test_ch_25();
void test_ch_26();
void test_ch_27(int argc, const char* argv[]);
void test_ch_28();
void test_ch_29(int argc, const char* argv[]);
void test_ch_30(int argc, const char* argv[]);
void test_ch_31(int argc, const char* argv[]);
void test_ch_32();
void test_ch_33(int argc, const char* argv[]);
void test_ch_34();
void test_ch_35(int argc, const char* argv[]);

int main(int argc, const char** argv)
{
    (void)argc;
    (void)argv;

    std::cout << std::boolalpha;

#if (0) //done
    test_ch_17();
    test_ch_20(argc, argv);
    test_ch_21(argc, argv);
    test_ch_22(argc, argv);
    test_ch_23(argc, argv);
    test_ch_24(argc, argv);
    test_ch_25();
    test_ch_26();
    test_ch_27(argc, argv);
    test_ch_28();
    test_ch_29(argc, argv);
    test_ch_30(argc, argv);
    test_ch_31(argc, argv);
    test_ch_32();
    test_ch_33(argc, argv);
    test_ch_34();
#endif

    test_ch_35(argc, argv);

    return 0;
}
