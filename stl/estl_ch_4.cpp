//=============================================================================
//
//  Iterators
//
//=============================================================================
#include <iostream>
#include <vector>
#include <list>
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

namespace item30 {

int f(int x) {
    return x * 2;
}

void test_front_back_inserter()
{
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    std::deque<int> result;

    // back_inserter call push_back
#if (0)
    std::transform(v.begin(), v.end(), std::back_inserter(result), f);
#else
    std::transform(v.rbegin(), v.rend(), std::front_inserter(result), f);
#endif

    std::copy(result.begin(), result.end(), std::ostream_iterator<int>(std::cout, " "));
}

void test_inserter()
{
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    std::vector<int> result{ 10, 11, 12, 13, 14, 15 };

    result.reserve(result.size() + v.size());

    std::transform(v.begin(), v.end(), std::inserter(result, result.begin()+result.size()/2), f);

    std::copy(result.begin(), result.end(), std::ostream_iterator<int>(std::cout, " "));
}

void test()
{
    //test_front_back_inserter();
    test_inserter();
}

} // item30 -----------------------------------------------

namespace item31 {

bool cmp(int l, int r) {
    return l < r;
}

void test_partial_sort()
{
    std::vector<int> v { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
    std::partial_sort(v.begin(), v.begin()+4, v.end(), cmp);

    // 1 2 3 4 9 7 5 6 8 10
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
}

void test_nth_element()
{
    std::vector<int> v { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
    std::nth_element(v.begin(), v.begin()+5, v.end(), cmp);

    // 3 1 2 5 4 6 7 9 8 10
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
}

void test_nth_element_2()
{
    std::vector<int> v { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
    typedef std::vector<int>::iterator IntIter;

    IntIter b(v.begin());
    IntIter e(v.end());
    IntIter c = b + v.size()/2;

    std::nth_element(b, c, e, cmp);

    std::copy(b, e, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\ncenter value: " << *c << "\n";
}

bool x3(int i)
{
    return i && (i % 3 == 0);
}

void test_partition()
{
    std::vector<int> v { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
    auto e = std::partition(v.begin(), v.end(), x3);

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
    std::copy(v.begin(), e, std::ostream_iterator<int>(std::cout, " "));
}

void test()
{
    //test_partial_sort();
    //test_nth_element();
    //test_nth_element_2();
    test_partition();
}

} // item31 -----------------------------------------------

namespace item32 {

void test_remove()
{
    std::vector<int> v{ 1, 2, 3, 4, 5, 4, 3, 2, 1 };
    std::cout << "before size: " << v.size() << "\n";
    auto e = std::remove(v.begin(), v.end(), 4);
    std::cout << "after size: " << v.size() << "\n";
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
    v.erase(e, v.end());
    std::cout << "after erase size: " << v.size() << "\n";
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void test_list_remove()
{
    std::list<int> l{ 1, 2, 99, 3, 99, 4, 5 };
    std::cout << "before remove: " << l.size() << "\n";
    l.remove(99);
    std::cout << "after remove: " << l.size() << "\n";
}

void test_unique()
{
    std::vector<int> l{ 1, 2, 3, 4, 5, 4, 3, 2, 1 };

    l.erase(std::unique(l.begin(), l.end()), l.end());
    std::cout << "l.size(): " << l.size() << "\n";
    std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    std::sort(l.begin(), l.end());
    l.erase(std::unique(l.begin(), l.end()), l.end());
    std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void test()
{
    //test_remove();
    //test_list_remove();
    test_unique();
}

} // item32 -----------------------------------------------

namespace item33 {

class W {
    int quality = 0;
public:
    W() : quality(0)
    {}
    W(int q) : quality(q)
    {}
    ~W()
    {
        std::cout << "~W(" << quality << ")\n";
    }

    bool isCertified() const {
        return 90 <= quality;
    }
    void print() const {
        std::cout << "W(" << quality << ") ";
    }
};

void test_pointer_container()
{
    std::vector<W*> v;
    v.push_back(new W(80));
    v.push_back(new W(90));
    v.push_back(new W(95));
    v.push_back(new W(85));
    v.push_back(new W(100));

    std::for_each(v.begin(), v.end(), [](W*& w){
        if (!w->isCertified()) {
            delete w;
            w = nullptr;
        }
    });
    auto e = std::remove(v.begin(), v.end(), static_cast<W*>(0));
    v.erase(e, v.end());

    std::for_each(v.begin(), v.end(), [](const W* w){
        w->print();
    });
    std::cout << "\n";
    std::cout << "\n********** clean up vector **********\n";
    std::for_each(v.begin(), v.end(), [](W*& w){
        delete w;
        w = nullptr;
    });
}

void test_sharedptr_container()
{
    std::vector<std::shared_ptr<W>> v;
    v.push_back(std::shared_ptr<W>(new W(80)));
    v.push_back(std::shared_ptr<W>(new W(90)));
    v.push_back(std::shared_ptr<W>(new W(95)));
    v.push_back(std::shared_ptr<W>(new W(85)));
    v.push_back(std::shared_ptr<W>(new W(100)));

    auto e = std::remove_if(v.begin(), v.end(), [](std::shared_ptr<W> w){
        return !w->isCertified();
    });
    v.erase(e, v.end());

    std::cout << "********** end of test_sharedptr_container() **********\n";
}

void test()
{
    //test_pointer_container();
    test_sharedptr_container();
}

} // item33 -----------------------------------------------

} // namespace ================================================================

void test_ch_4()
{
    item33::test();
}
