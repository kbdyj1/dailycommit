#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

namespace { //=================================================================

namespace _1 {

void test_array(string& s)
{
    try {
        s[6] = '\0';
    } catch (const out_of_range& e) {
        cerr << e.what() << "\n";
    }
}

void test_at(string& s)
{
    try {
        s.at(6) = '\0'; //basic_string::at: __n (which is 6) >= this->size() (which is 6)
    } catch (const out_of_range& e) {
        cerr << e.what() << "\n";
    }
}

void test_c_str(string& s)
{
    const char* p = s.c_str();

    delete [] p;
}

void test()
{
    string s{"123456"};

    //test_array(s);
    test_at(s);
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_string_out_of_range()
{
    _1::test();
}
