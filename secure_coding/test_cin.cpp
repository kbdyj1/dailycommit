#include <iostream>

using namespace std;

namespace { //=================================================================

namespace _1 {

void test()
{
    string s;

    cin >> s;
    cout << "s(" << s << ").size(): " << s.size() << endl;
    cin >> s;
    cout << "s(" << s << ").size(): " << s.size() << endl;
    cin >> s;
    cout << "s(" << s << ").size(): " << s.size() << endl;
}

} //-----------------------------------------------------------------

} //===========================================================================

void test_cin()
{
    _1::test();
}
