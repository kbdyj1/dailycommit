#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
#include <iterator>

namespace { //=================================================================

class Widget {
    int value;
public:
    Widget(int value) : value(value)
    {}
    int getValue() const {
        return value;
    }
    void test() {
        if (3 < value) {
            std::cout << "Widget(" << value << ")\n";
        }
    }
};
std::ostream& operator<<(std::ostream& os, const Widget& w)
{
    os << "Widget(" << w.getValue() << ")";
    return os;
}
void setupWidgetVector(std::vector<Widget>& v)
{
    std::vector<Widget> vw{ 1, 2, 3, 4, 5 };
    v.swap(vw);
}
void setupWidgetPtrList(std::list<Widget*>& l)
{
    std::list<Widget*> wl{
        new Widget(1),
        new Widget(2),
        new Widget(3),
        new Widget(4),
        new Widget(5)
    };
    l.swap(wl);
}

namespace item38 {

template <typename T>
class Functor;

template <typename T>
class Impl {
public:
    virtual ~Impl()
    {}
    virtual void operator()(const T& val) const = 0;
};

template <typename T>
class Functor : public std::unary_function<T, void> {
    Impl<T>* impl;
public:
    Functor(Impl<T>* p) : impl(p)
    {}

    void operator()(const T& value) const {
        impl->operator()(value);
    }
};

template <typename T>
class SpaceImpl : public Impl<T> {
public:
    virtual ~SpaceImpl()
    {}
    virtual void operator()(const T& value) const {
        std::cout << value << " ";
    }
};
template <typename T>
class NewLineImpl : public Impl<T> {
public:
    virtual ~NewLineImpl()
    {}
    virtual void operator()(const T& value) const {
        std::cout << value << "\n";
    }
};

void test()
{
#if (0)
    auto impl = std::shared_ptr<SpaceImpl<int>>(new SpaceImpl<int>);
#else
    auto impl = std::shared_ptr<NewLineImpl<int>>(new NewLineImpl<int>);
#endif
    Functor<int> functor(impl.get());
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    std::for_each(v.begin(), v.end(), functor);
}

} // item38 -----------------------------------------------

namespace item39 {

class Bad : public std::unary_function<Widget, bool> {
    int count = 0;
public:
    bool operator()(const Widget& w)
    {
        return ++count % 3 == 0;
    }
};
class Good : public std::unary_function<Widget, bool> {
public:
    bool operator()(const Widget& w) const {
        return w.getValue() % 3 == 0;
    }
};

void test()
{
    std::vector<Widget> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
#if (0)
    v.erase(std::remove_if(v.begin(), v.end(), Bad()), v.end());
#else
    v.erase(std::remove_if(v.begin(), v.end(), Good()), v.end());
#endif
    std::copy(v.begin(), v.end(), std::ostream_iterator<Widget>(std::cout, " "));
}

} // item39 -----------------------------------------------

namespace item40 {

// argument_type
// first_argument_type
// second_argument_type
// result type

bool isPass(const Widget* w)
{
    return 3 < w->getValue();
}

void test_not1()
{
    std::list<Widget*> wptrlist;
    setupWidgetPtrList(wptrlist);

#if (1)
    auto iter = std::find_if(wptrlist.begin(), wptrlist.end(), isPass);
#else
    auto iter = std::find_if(wptrlist.begin(), wptrlist.end(), std::not1(std::ptr_fun(isPass)));
#endif
    if (iter != wptrlist.end()) {
        std::cout << (*iter)->getValue() << "\n";
    }

    std::for_each(wptrlist.begin(), wptrlist.end(), [](Widget* p){
        delete p;
    });
}

template <typename T>
class MeetsThreshold : public std::unary_function<Widget*, bool>
{
    const T threshold;
public:
    MeetsThreshold(const T& t) : threshold(t)
    {}
    bool operator()(const Widget* w) const
    {
        return threshold < w->getValue();
    }
};

struct WidgetNameCompare : public std::binary_function<Widget, Widget, bool>
{
    bool operator()(const Widget& l, const Widget& r) const
    {
        return true;
    }
};

void test_unary_function()
{
    std::list<Widget*> wl;
    setupWidgetPtrList(wl);

#if (0)
    auto iter = std::find_if(wl.begin(), wl.end(), MeetsThreshold<int>(3));
#else
    auto iter = std::find_if(wl.begin(), wl.end(), std::not1(MeetsThreshold<int>(3)));
#endif
    if (iter != wl.end()) {
        std::cout << "find_if(MeetsThreshold<int>(3): " << (*iter)->getValue() << "\n";
    }
}

void test()
{
    //test_not1();
    test_unary_function();
}

} // item40 -----------------------------------------------

namespace item41 {

void testWidget(const Widget& w)
{
    if (3 < w.getValue()) {
        std::cout << "Widget(" << w.getValue() << ")\n";
    }
}



/*
template <typename R, typename C>
mem_fun_t<R, C>
mem_fun(R (C::*func)());
*/

void test_mem_fun_ref()
{
    std::vector<Widget> v;
    setupWidgetVector(v);
#if (1)
    std::for_each(v.begin(), v.end(), std::ptr_fun(testWidget));
#else
    std::for_each(v.begin(), v.end(), std::mem_fun_ref(&Widget::test));
#endif
}

void test_mem_fun()
{
    std::list<Widget*> wl;
    setupWidgetPtrList(wl);

    std::for_each(wl.begin(), wl.end(), std::mem_fun(&Widget::test));
}

void test()
{
    test_mem_fun_ref();
    //test_mem_fun();
}

} // item41 -----------------------------------------------

} //namespace =================================================================

void test_ch_5()
{
    item41::test();
}
