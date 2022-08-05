//=============================================================================
//
//  Iterators
//
//=============================================================================
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <iterator>
#include <fstream>

namespace { //=================================================================

namespace item26 {

void test()
{
    std::deque<int> v{ 0, 1, 2, 3, 4, 5 };
    auto iter = v.begin();
    auto cIter = v.cbegin();

    if (iter == cIter) {
        std::cout << "iter == cIter\n";
    }
    if (iter - cIter >= 3) {
        std::cout << "iter - cIter >= 3\n";
    }
}

} // item26 -----------------------------------------------

namespace item27 {

void test()
{
    {
        typedef std::vector<int>::iterator IntVecIter;
        typedef std::vector<int>::const_iterator IntVecConstIter;
        std::vector<int> v{ 0, 1, 2, 3, 4, 5 };
        auto cIter = v.cbegin();
#if (0)
        std::vector<int>::iterator iter(const_cast<IntVecIter>(cIter));
#else
        std::vector<int>::iterator iter = v.begin();
        // convert const iterator -> iterator
        std::advance(iter, std::distance<IntVecConstIter>(iter, cIter));
#endif
    }
    {
        typedef std::string::iterator StrIter;
        std::string str = "Qt 6.0";
        auto cIter = str.cbegin();
        //std::string::iterator iter = const_cast<StrIter>(cIter);
    }
}

} // item27 -----------------------------------------------

namespace item28 {

void test_reverse_iterator_insert()
{
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    std::vector<int>::reverse_iterator rIter = std::find(v.rbegin(), v.rend(), 3);
    auto iter = rIter.base();
    v.insert(iter, 100);
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
}

void test_reverse_iterator_erase()
{
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    std::vector<int>::reverse_iterator rIter = std::find(v.rbegin(), v.rend(), 3);
#if (0)
    v.erase(--rIter.base());
#else
    v.erase((++rIter).base());
#endif
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
}

void test()
{
    std::cout << "reverse_iterator insert\n";
    test_reverse_iterator_insert();

    std::cout << "\n\n";
    std::cout << "reverse_iterator erase\n";
    test_reverse_iterator_erase();
}

} // item28 -----------------------------------------------

namespace item29 {

void init()
{
    std::string filename{"qt.txt"};
    std::fstream s(filename, s.out);
    if (s.is_open()) {
        s << "Hello, Qt 6!";
        s.close();
    }
}

void test()
{
    init();
    std::ifstream in("qt.txt");
    in.unsetf(std::ios::skipws);
#if (0)
    // istream_iterator handles formatted input
    std::string str((std::istream_iterator<char>(in)), std::istream_iterator<char>());
#else
    // performance enhanced
    std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
#endif
    std::cout << "qt.txt\n";
    std::cout << str;
}

} // item29 -----------------------------------------------

} // namespace ================================================================

void test_ch_4()
{
    item29::test();
}
