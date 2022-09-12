//=============================================================================
//  Secure Coding
//  Ch.2 String
//
//  22/09/12
//=============================================================================

#include <iostream>
#include <algorithm>
#include <iterator>
#include <array>

#include "util.h"

namespace { //=================================================================

namespace bad {
    void clear(int array[]) {
        auto length = sizeof(array)/sizeof(array[0]);   // warning: sizeof on array function parameter will return size of 'int *' instead of 'int []'

        PRINT_ATTR(sizeof(array)/sizeof(array[0]));

        for (auto i=0; i<length; i++) {
            array[i] = 0;
        }
    }
} //bad

namespace good {

template <typename T, size_t N>
void clear(std::array<T, N>& array) {
    std::fill(std::begin(array), std::end(array), 0);
}

} //good

void test_clear_array() {
#if (0)
    int array[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    bad::clear(array);
    std::copy(array, array+8, std::ostream_iterator<int>(std::cout, ","));
#else
    auto array = std::array<int,8>{ 1, 2, 3, 4, 5, 6, 7, 8 };
    good::clear(array);
    std::copy(std::begin(array), std::end(array), std::ostream_iterator<int>(std::cout, ","));
#endif

    std::cout << std::endl;
};

void test_char_type()
{
    char cstr[] = "char string";
    signed char scstr[] = "singed char string";
    unsigned char uscstr[] = "unsigned char string";

    auto len = strlen(cstr);

#if (0)
    //note: candidate function not viable: no known conversion from 'unsigned char [21]' to 'const char *' for 1st argument
    //size_t   strlen(const char *__s);
    //         ^
    len = strlen(uscstr);
#endif

    PRINT_ATTR(sizeof('x'));    // C:4, C++:1
    PRINT_ATTR(sizeof(L'x'));   // C,C++: 4

}

void test_wchar_type()
{
    PRINT_ATTR(sizeof(wchar_t));// Linux(UTF-32): 4, Win32(UTF-16): 2

    wchar_t wstr0[] = L"0123456789";
    PRINT_FUNC(wcslen(wstr0));
}

void test_cin()
{
    std::cout << "\ninput string: ";
    char buf[12];
    std::cin.width(12);
    std::cin >> buf;
    std::cout << "echo: " << buf << std::endl;
}

bool test_gets_password()
{
    std::cout << "input password: ";

    char password[12];

    gets(password);

    auto result = strcmp(password, "goodpass");

    return result == 0;
}

} //namespace =================================================================

void test_ch_02()
{
#if (0) // done
    test_clear_array();
    test_char_type();
    test_cin();
#endif

    test_gets_password();
}
