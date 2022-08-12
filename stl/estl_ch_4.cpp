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
#include <memory>
#include <numeric>
#include <functional>

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

namespace item34 {

void test_binary_search()
{
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

#if (0)
    // std::binary_search(5): false
    auto res = std::binary_search(v.begin(), v.end(), 5);
#else
    // std::binary_search(5): true
    auto res = std::binary_search(v.begin(), v.end(), 5, std::greater<int>());
#endif
    std::cout << "std::binary_search(5): " << (res ? "true" : "false") << "\n";
}
void test_set()
{
#if (1)
    std::vector<int> v0{ 1, 2, 3, 4, 5 };
    std::vector<int> v1{ 3, 4, 5, 6, 7 };
#else
    std::vector<int> v0{ 1, 2, 5, 5, 5, 9 };
    std::vector<int> v1{ 2, 5, 7 };
#endif
    std::vector<int> v2;

    //std::set_union(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v2));
    //std::set_intersection(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v2));
    //std::set_difference(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v2));
    //std::set_symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v2));
    std::merge(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v2));

    std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void test_inplace_merge()
{
    std::vector<int> v{ 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
    std::inplace_merge(v.begin(), v.begin()+5, v.end());

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

template <class Os, class Co>
Os& operator<<(Os& os, const Co& c) {
    os << "{ ";
    for (auto i : c) {
        os << i << ' ';
    }
    return os << "}\t";
}

void test_includes()
{
    const auto
            v0 = { 'a', 'b', 'c', 'f', 'h', 'x' },
            v1 = { 'a', 'b', 'c' },
            v2 = { 'a', 'c' },
            v3 = { 'a', 'a', 'b' },
            v4 = { 'g' },
            v5 = { 'a', 'c', 'g' },
            v6 = { 'A', 'B', 'C' };
    auto nocaseLess = [](char a, char b) {
        return std::tolower(a) < std::tolower(b);
    };

    std::cout << v0 << "\nincludes:\n" << std::boolalpha
              << v1 << ": " << std::includes(v0.begin(), v0.end(), v1.begin(), v1.end()) << "\n"
              << v2 << ": " << std::includes(v0.begin(), v0.end(), v2.begin(), v2.end()) << "\n"
              << v3 << ": " << std::includes(v0.begin(), v0.end(), v3.begin(), v3.end()) << "\n"
              << v4 << ": " << std::includes(v0.begin(), v0.end(), v4.begin(), v4.end()) << "\n"
              << v5 << ": " << std::includes(v0.begin(), v0.end(), v5.begin(), v5.end()) << "\n"
              << v6 << ": " << std::includes(v0.begin(), v0.end(), v6.begin(), v6.end(), nocaseLess) << " (case-insensitive)\n";
}

void test()
{
    //test_binary_search();
    //test_set();
    //test_inplace_merge();
    test_includes();
}

} // item34 -----------------------------------------------

namespace item35 {

int nocaseCharCmp(char c0, char c1)
{
    int l0 = std::tolower(c0);
    int l1 = std::tolower(c1);

    if (l0 < l1)
        return -1;
    else if (l0 > l1)
        return 1;
    else
        return 0;
}

int nocaseStringCmpImpl(const std::string& s0, const std::string& s1)
{
    auto p = std::mismatch(s0.begin(), s0.end(), s1.begin(), std::not2(std::function(nocaseCharCmp)));
    if (p.first == s0.end()) {
        if (p.second == s1.end())
            return 0;
        else
            return -1;
    }
    return nocaseCharCmp(*p.first, *p.second);
}

int nocaseStrCmp(const std::string& s0, const std::string& s1)
{
    if (s0.size() < s1.size())
        return nocaseStringCmpImpl(s0, s1);
    else
        return -nocaseStringCmpImpl(s1, s0);
}

bool nocaseCharLess(char c0, char c1)
{
    int l0 = std::tolower(c0);
    int l1 = std::tolower(c1);

    return l0 < l1;
}

bool nocaseStrCmp2(const std::string& s0, const std::string& s1)
{
    return std::lexicographical_compare(s0.begin(), s0.end(), s1.begin(), s1.end(), nocaseCharLess);
}

void test_nocase_string_cmp(const std::string& s0, const std::string& s1)
{
    std::cout << "nocaseStrCmp(" << s0 << ", " << s1 << "): " << nocaseStrCmp(s0, s1) << "\n";
}
void test_nocase_string_cmp2(const std::string& s0, const std::string& s1)
{
    std::cout << "nocaseStrCmp2(" << s0 << ", " << s1 << "): "
              << (nocaseStrCmp2(s0, s1) ? "<" : ">=")
              << "\n";
}

void test()
{
    auto s0 = std::string{"Hello, Qt6.0"};
    auto s1 = std::string{"hello, qt6.0"};
    auto s2 = std::string{"hello, qt6"};
    auto s3 = std::string{"hEllo, qt6.2"};
    auto s4 = std::string{"fello, qT6.0"};
    auto s5 = std::string{"hello, Rt6.2.1"};

    void (*func)(const std::string& s0, const std::string& s1);

#if (0)
    func = test_nocase_string_cmp;
#else
    func = test_nocase_string_cmp2;
#endif

    func(s0, s1);
    func(s0, s2);
    func(s0, s3);
    func(s0, s4);
    func(s0, s5);
}

} // item35 -----------------------------------------------

namespace item36 {

template <typename InputIterator, typename OutputIterator, typename Predicate>
OutputIterator copy_if(InputIterator begin, InputIterator end, OutputIterator outBegin, Predicate func)
{
    while (begin != end) {
        if (func(*begin))
            *outBegin++ = *begin;
        ++begin;
    }
    return outBegin;
}

void test()
{
    std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
#if (0)
    std::copy_if(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "), [](int value){
        return value % 3 == 0;
    });
#else
    item36::copy_if(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "), [](int value){
            return value % 3 == 0;
    });
#endif
}

} // item36 -----------------------------------------------

namespace item37 {

int x2(int ret, int value)
{
    return ret + value * 2;
}

struct Point {
    float x;
    float y;

    Point() : x(0.0), y(0.0)
    {}
    Point(const Point& rhs) : x(rhs.x), y(rhs.y)
    {}
    Point(float x, float y) : x(x), y(y)
    {}

    Point& operator+=(const Point& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Point& operator=(const Point& rhs) {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }
};

class PointAverage {
    int count;
    float x;
    float y;
public:
    PointAverage() : count(0), x(0.0), y(0.0)
    {}
    const Point operator()(const Point& base, const Point& value) {
        ++count;
        x += value.x;
        y += value.y;
        return Point(x/count, y/count);
    }
};

void setupVector(std::vector<Point>& v)
{
    std::vector<Point> pts{{1, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}};
    v.swap(pts);
}

void test_accumulate()
{
    std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::cout << "std::count(5): " << std::count(v.begin(), v.end(), 5) << "\n";
    std::cout << std::count_if(v.begin(), v.end(), [](int value){
        return value % 3 == 0;
    })
    << "\n";
    auto iter = std::min_element(v.begin(), v.end());
    std::cout << "min_element: " << *iter << "\n";
    iter = std::max_element(v.begin(), v.end());
    std::cout << "max_element: " << *iter << "\n";

    auto res = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "accumulate: " << res << "\n";

    res = std::accumulate(v.begin(), v.end(), 0, x2);
    std::cout << "accumulate(x2): " << res << "\n";

    std::vector<Point> pts;
    setupVector(pts);

    auto result = std::accumulate(pts.begin(), pts.end(), Point(0, 0), PointAverage());
    std::cout << "accumulate -> average point(" << result.x << ", " << result.y << ")\n";
}

class PointAverage2 : public std::unary_function<Point, void>{
    int count = 0;
    float x = 0.0;
    float y = 0.0;
public:
    void operator()(const Point& p) {
        x += p.x;
        y += p.y;
        ++count;
    }
    Point result() const {
        return Point(x/count, y/count);
    }
};

void test_for_each()
{
    std::vector<Point> pts;
    setupVector(pts);

    auto res = std::for_each(pts.begin(), pts.end(), PointAverage2());
    auto point = res.result();
    std::cout << "for_each -> average point(" << point.x << ", " << point.y << ")\n";
}

void test()
{
    //test_accumulate();
    test_for_each();
}

} // item37 -----------------------------------------------

} // namespace ================================================================

void test_ch_4()
{
    item37::test();
}
