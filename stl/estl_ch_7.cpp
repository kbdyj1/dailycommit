#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <functional>

namespace { //=================================================================

using namespace std::placeholders;

class Widget {
    int value;
public:
    Widget(int value) : value(value)
    {}

    void draw() {
        std::cout << "Widget::draw(" << value << ")\n";
    }
    bool operator==(const Widget& w) const {
        return value == w.value;
    }
    bool operator<(const Widget& w) const {
#if (0)
        if (value < w.value) {
            return -1;
        } else if (value > w.value) {
            return 1;
        } else {
            return 0;
        }
#else
        return value < w.value;
#endif
    }
    friend std::ostream& operator<<(std::ostream& os, const Widget& w)
    {
        os << "Widget(" << w.value << ")";
    }
};

namespace item43 {

void setupWidgetVector(std::vector<Widget>& wv)
{
    std::vector<Widget> v { 1, 2, 3, 4, 5 };
    wv.swap(v);
}

void test_for_each()
{
    std::vector<Widget> vw;
    setupWidgetVector(vw);

    std::for_each(vw.begin(), vw.end(), std::mem_fn(&Widget::draw));
}

void test_transform()
{
    std::vector<int> v { 10, 11, 12, 13, 14 };
    std::vector<int> result;
    std::transform(v.begin(), v.end(), std::inserter(result, result.begin()), std::bind(std::plus<int>(), _1, 'a'));
    std::copy(result.begin(), result.end(), std::ostream_iterator<char>(std::cout, " "));
}

void test_find_if()
{
    std::vector<int> v { 10, 11, 12, 13, 14 };
    int x = 11;
    int y = 14;
    auto iter = std::find_if(v.begin(), v.end(), [=](int value){
        return x < value && value < y;
    });
    std::cout << "11 < ? << 14 : " << *iter << "\n";
}

void test()
{
    //test_for_each();
    test_transform();
    //test_find_if();
}

} // item43 -----------------------------------------------

namespace item44 {

void test()
{
    std::set<int> s{
        1, 2, 3, 4, 5, 6, 7, 8, 9
    };
    auto iter = s.find(5);
    if (iter != s.end()) {
        std::cout << "s.find(5): " << *iter << "\n";
    }
    iter = std::find(s.begin(), s.end(), 5);
    if (iter != s.end()) {
        std::cout << "std::find(5): " << *iter << "\n";
    }
}

} // item44 -----------------------------------------------

namespace item45 {

// count, count_if, find, find_if -> O(N)
// binary_search, lower_bound, upper_bound -> O(log N)

void test_unordered()
{
    std::list<Widget> wl{ 1, 2, 3, 2, 3, 3, 4, 5 };
    auto w = Widget(3);
    auto c = std::count(wl.begin(), wl.end(), w);
    std::cout << "std::count(Widget(3)): " << c << "\n\n";

    auto iter = std::find(wl.begin(), wl.end(), w);
    if (iter != wl.end()) {
        std::cout << "std::find(Widget(3)): " << *iter << "\n";
        std::cout << "distance: " << std::distance(wl.begin(), iter) << "\n";
    }
}

void test_ordered()
{
    std::list<Widget> wl{ 1, 1, 2, 3, 5, 5, 9, 10, 11, 12 };
    auto bsearch = [=](int value) {
        auto res = std::binary_search(wl.begin(), wl.end(), Widget(value));
        std::cout << "std::binary_search(Widget(" << value << "): " << res << "\n";
    };

    bsearch(4);
    bsearch(5);

    auto equalRange = [=](int value) {
        auto res = std::equal_range(wl.begin(), wl.end(), Widget(value));
        std::cout << "std::equal_range(Widget(" << value << ")): ";
        if (res.first != res.second) {
             std::cout << "("
                       << *res.first << "[" << std::distance(wl.begin(), res.first) << "], "
                       << *res.second << "[" << std::distance(wl.begin(), res.second) << "] )\n";
             std::cout << "count: " << std::distance(res.first, res.second) << "\n";
        } else {
            std::cout << "not found\n";
        }
    };

    equalRange(4);
    equalRange(5);

    auto lowerBound = [=](int value) {
        auto iter = std::lower_bound(wl.begin(), wl.end(), Widget(value));
        std::cout << "std::lower_bound(Widget(" << value << ")): ";
        if (iter != wl.end()) {
            std::cout << *iter << "[" << std::distance(wl.begin(), iter) << "]\n";
        } else {
            std::cout << "not found\n";
        }
    };

    lowerBound(4);
    lowerBound(5);

    auto upperBound = [=](int value) {
        auto iter = std::upper_bound(wl.begin(), wl.end(), Widget(value));
        std::cout << "std::upper_bound(Widget(" << value << ")): ";
        if (iter != wl.end()) {
            std::cout << *iter << "[" << std::distance(wl.begin(), iter) << "]\n";
        } else {
            std::cout << "not found\n";
        }
    };

    upperBound(4);
    upperBound(5);
}

void test()
{
    //test_unordered();
    test_ordered();
}

} // item45 -----------------------------------------------

namespace item46 {

void test_sort()
{
    std::vector<int> v{ 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

struct StringSize : public std::unary_function<std::string, int>
{
    std::string::size_type operator()(const std::string& s) const {
        return s.size();
    }
};

void test_string_size()
{
    std::set<std::string> strs {
        "Hello",
        "Extraordinary",
        "Attorney",
        "Woo",
        "Netflix",
        "Hanbada",
        "Taesan"
    };
    std::transform(strs.begin(), strs.end(),
                   std::ostream_iterator<std::string::size_type>(std::cout, " "),
               #if (0)
                   std::mem_fn(&std::string::size)
               #else
                   StringSize()
               #endif
    );
}

template <typename T>
T average(T value0, T value1)
{
    return (value0 + value1) / 2;
}

template <typename Iter0, typename Iter1>
void printAverage(Iter0 b0, Iter0 e0, Iter1 b1, std::ostream& os)
{
    std::transform(b0, e0, b1,
                   std::ostream_iterator<typename Iter0::value_type>(os, "\n"),
                   average<typename std::iterator_traits<Iter0>::value_type>);
}

void test_average()
{
    std::vector<int> v0{ 1, 2, 3, 4, 5 };
    std::vector<double> v1{ 6, 7, 8, 9, 10 };
    printAverage(v0.begin(), v0.end(), v1.begin(), std::cout);
}

void test()
{
    //test_sort();
    //test_string_size();
    test_average();
}

} // item46 -----------------------------------------------

namespace item47 {

void test()
{
    std::vector<int> v { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int x = 7;
    int y = 4;

    // code that is easy to write and hard to read
    v.erase(
        std::remove_if(
            std::find_if(
                v.rbegin(), v.rend(),
                std::bind(std::less<int>(), _1, y)
            ).base(),
            v.end(),
            std::bind(std::greater_equal<int>(), _1, x)
        ),
        v.end()
    );

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

} // item47 -----------------------------------------------

namespace item49 {

class EmailNickname {
    typedef  std::map<std::string, std::string> EmailNicknameMap;
    EmailNicknameMap emailNicknameMap;
public:
    void showEmailAddress(const std::string& nickname) const
    {
        EmailNicknameMap::const_iterator iter = emailNicknameMap.find(nickname);
        if (iter != emailNicknameMap.end()) {
            // TODO
        }
    }
};

void test()
{
#if (0)
    //error: no matching function for call to ‘std::__cxx11::basic_string<char>::basic_string(int)’
    //  287 |     std::string s(10);
    //      |                     ^
    std::string s(10);
#endif

#if (0)
    //error: no match for ‘operator-’ (operand types are ‘std::_List_iterator<int>’ and ‘std::_List_iterator<int>’)
    // 1955 |                                 std::__lg(__last - __first) * 2,
    //      |                                           ~~~~
    std::list<int> l{ 1, 2, 3, 4, 5, 9, 8, 7, 6 };
    std::sort(l.begin(), l.end());
#endif
}

}

} //namespace =================================================================

void test_ch_7()
{
    item47::test();
}
