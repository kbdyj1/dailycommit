#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <set>
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
    std::for_each(v.begin(), v.end(), std::function(testWidget));
#else
    std::for_each(v.begin(), v.end(), std::mem_fun_ref(&Widget::test));
#endif
}

void test_mem_fun()
{
    std::list<Widget*> wl;
    setupWidgetPtrList(wl);

    std::for_each(wl.begin(), wl.end(), std::mem_fn(&Widget::test));
}

void test()
{
    test_mem_fun_ref();
    //test_mem_fun();
}

} // item41 -----------------------------------------------

namespace item42 {

class Bullet {
    size_t mWeight;
    size_t mSpeed;
public:
    Bullet(size_t weight, size_t speed) : mWeight(weight), mSpeed(speed)
    {}
    size_t weight() const {
        return mWeight;
    }
    size_t speed() const {
        return mSpeed;
    }
    friend bool operator<(const Bullet& l, const Bullet& r) {
        return l.weight() < r.weight();
    }
    friend std::ostream& operator<<(std::ostream& os, const Bullet& bullet)
    {
        os << "Bullet(" << bullet.weight() << ", " << bullet.speed() << ")";
        return os;
    }
};

//#define INHERIT_BINARY_FUNCTION

#if (0)
//error: declaration of ‘struct std::less<{anonymous}::item42::Bullet>’ in namespace ‘{anonymous}::item42’ which does not enclose ‘std’
template <>
struct std::less<Bullet> : public std::binary_function<Bullet, Bullet, bool>
#else
struct BulletSpeedCmp
    #ifdef INHERIT_BINARY_FUNCTION
        : public std::binary_function<Bullet, Bullet, bool>
    #endif
#endif
{
    bool operator()(const Bullet& l, const Bullet& r) const {
        return l.speed() < r.speed();
    }

#ifndef INHERIT_BINARY_FUNCTION
    typedef Bullet first_argument_type;
    typedef Bullet second_argument_type;
    typedef bool result_type;
#endif
};

//#define INHERIT_UNARY_FUNCTION

struct BulletValid
#ifdef INHERIT_UNARY_FUNCTION
    : public std::unary_function<Bullet, bool>
#endif
{
    bool operator()(const Bullet& bullet) const {
        return 10 <= bullet.speed();
    }

#ifndef INHERIT_UNARY_FUNCTION
    typedef Bullet argument_type;
    typedef bool result_type;
#endif
};

#if (1)
typedef std::set<Bullet> BulletSet;
#else
typedef std::set<Bullet, BulletSpeedCmp> BulletSet;
#endif

void setupBulletSet(BulletSet& bs)
{
    BulletSet s{
        {1, 10},
        {2, 5},
        {3, 8},
        {4, 15},
        {5, 6}
    };
    bs.swap(s);
}

void test()
{
    BulletSet bs;
    setupBulletSet(bs);

    std::copy(bs.begin(), bs.end(), std::ostream_iterator<Bullet>(std::cout, ""));
    std::cout << "\n";

    auto iter = std::find_if(bs.begin(), bs.end(), std::not1(BulletValid()));
    if (iter != bs.end()) {
        std::cout << *iter << "\n";
    }
}

} // item42 -----------------------------------------------

} //namespace =================================================================

void test_ch_5()
{
    item42::test();
}
