#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iterator>

namespace { //=================================================================

namespace item38 {

class Widget
{};

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

class Widget {
    int value;
public:
    Widget(int value) : value(value)
    {}
    int getValue() const {
        return value;
    }
};
std::ostream& operator<<(std::ostream& os, const Widget& w)
{
    os << "Widget(" << w.getValue() << ")";
    return os;
}

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

} //namespace =================================================================

void test_ch_5()
{
    item39::test();
}
