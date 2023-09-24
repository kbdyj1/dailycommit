#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

static void test_array(string& s)
{
    try {
        s[6] = '\0';
    } catch (const out_of_range& e) {
        cerr << e.what() << "\n";
    }
}

static void test_at(string& s)
{
    try {
        s.at(6) = '\0'; //basic_string::at: __n (which is 6) >= this->size() (which is 6)
    } catch (const out_of_range& e) {
        cerr << e.what() << "\n";
    }
}

void test_string_out_of_range()
{
    string s{"123456"};

#if (1)
    test_array(s);
#else
    test_at(s);
#endif
}
