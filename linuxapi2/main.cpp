#include <iostream>

namespace { //=================================================================
} //namespace =================================================================

// Book 1 -----------------------------------------------------------
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
void test_ch_37();

// Book 2 -----------------------------------------------------------
void exec_ch_01();
void exec_ch_02();
void exec_ch_03();
void exec_ch_04(int argc, const char** argv);
void exec_ch_05();
void exec_ch_07();
void exec_ch_08();
void exec_ch_09();
void exec_ch_10(int argc, const char* argv[]);
void exec_ch_11(int argc, const char* argv[]);
void exec_ch_12(int argc, const char* argv[]);
void exec_ch_13(int argc, const char* argv[]);
void exec_ch_15(int argc, const char* argv[]);
void exec_ch_16(int argc, const char* argv[]);
void exec_ch_17(int argc, const char* argv[]);
void exec_ch_18(int argc, const char* argv[]);
void exec_ch_19(int argc, const char* argv[]);
void exec_ch_20(int argc, const char* argv[]);
void exec_ch_22(int argc, const char* argv[]);
void exec_ch_23(int argc, const char* argv[]);
void exec_ch_24(int argc, const char* argv[]);
void exec_ch_25(int argc, const char* argv[]);
void exec_ch_26(int argc, const char* argv[]);
void exec_ch_27(int argc, const char* argv[]);

int main(int argc, const char** argv)
{
    (void)argc;
    (void)argv;

    std::cout << std::boolalpha;

#if (0) //done (Book 1)
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
    test_ch_35(argc, argv);
    test_ch_37();
#endif

#if (0) //done (Book 2)
    exec_ch_01();
    exec_ch_02();
    exec_ch_03();
    exec_ch_04(argc, argv);
    exec_ch_05();
    exec_ch_07();
    exec_ch_08();
    exec_ch_09();
    exec_ch_10(argc, argv);
    exec_ch_11(argc, argv);
    exec_ch_12(argc, argv);
    exec_ch_13(argc, argv);
    exec_ch_15(argc, argv);
    exec_ch_16(argc, argv);
    exec_ch_17(argc, argv);
    exec_ch_18(argc, argv);
    exec_ch_19(argc, argv);
    exec_ch_20(argc, argv);
    exec_ch_22(argc, argv);
    exec_ch_23(argc, argv);
    exec_ch_24(argc, argv);
    exec_ch_25(argc, argv);
    exec_ch_26(argc, argv);
#endif

    exec_ch_27(argc, argv);

    return 0;
}
