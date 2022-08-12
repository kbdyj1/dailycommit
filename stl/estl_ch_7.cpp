#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <functional>

namespace { //=================================================================

namespace item43 {

using namespace std::placeholders;

class Widget {
    int value;
public:
    Widget(int value) : value(value)
    {}

    void draw() {
        std::cout << "Widget::draw(" << value << ")\n";
    }
};

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

void test()
{

}

} // item45 -----------------------------------------------

} //namespace =================================================================

void test_ch_7()
{
    item43::test();
}
