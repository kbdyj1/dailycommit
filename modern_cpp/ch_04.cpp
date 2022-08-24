//=============================================================================
//  Modern C++ Programming Cookbook
//  Ch.4 preprocessor & compile
//
//  22/08/25
//=============================================================================

#include <iostream>

namespace { //=================================================================

void test_compiler()
{
#if defined _MSC_VER
    std::cout << "MSVC\n";
#elif defined __clang__
    std::cout << "Clang\n";
#elif defined __GNUC__
    std::cout << "GCC\n";
#else
    std::cout << "etc compiler\n";
#endif
}

void test_architecture()
{
#if defined __clang__ || __GNUC__
    std::cout <<
#   if defined __amd64__
        "AMD64"
#   elif defined __i386__
        "Intel x86"
#   elif defined __arm__
        "ARM"
#   else
        "Unknown"
#   endif
        << "\n";
#endif
}

void test_dev_configuration()
{
#if defined _DEBUG || QT_QML_DEBUG
    std::cout << "Debug\n";
#else
    std::cout << "Release\n";
#endif
}

} //namespace =================================================================

void test_ch_04_stringfy();

void test_ch_04()
{
    test_compiler();
    test_architecture();
    test_dev_configuration();
    test_ch_04_stringfy();
}
