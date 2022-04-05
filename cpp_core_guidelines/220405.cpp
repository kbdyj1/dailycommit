#include <iostream>
#include <span>
#include <memory>
#include <fstream>

namespace { //=================================================================

// I11. Never transfer ownership by a raw pointer or reference

// I12. Declare a pointer that must not be null as not_null

// I13. Do not pass an array as a single pointer

void procBad(int *p, int n)
{
    for (auto i=0; i<n; i++) {
        std::cout << "p[" << i << "] : " << p[i] << std::endl;
    }
}

void procGood(std::span<int> p)
{
    for (auto n : p) {
        std::cout << "span : " << n << std::endl;
    }
}

// I22. Avoid complex initialization of global objects

// I23. Keep the number of function arguments low

template <typename InIter0, typename InIter1, typename OutIter, typename Cmp>
OutIter merge(InIter0 begin0, InIter0 end0, InIter1 begin1, InIter1 end1, OutIter result, Cmp compare)
{
    return OutIter{};
}

template <typename InputRange0, typename InputRange1, typename OutIter>
OutIter mergeGood(InputRange0 input0, InputRange1 input1, OutIter result)
{
    return OutIter{};
}

// I24. Avoid adjacent parameters that can be invoked by the same arguments in either order with different meaning
template <typename T>
void copyN(T* from, T* to, int n)
{

}

template <typename T>
void copyNGood(const T* from, T* to, int n)
{

}

// I25. Prefer empty abstract classes as interfaces to class hierarchies

// I26. If you want a cross-compile ABI, use a C-style subset

// I27. for stable library ABI, consider the Pimpl idiom

// Widget.h
class Widget
{
    class Impl;
    std::unique_ptr<Impl> impl;
public:
    Widget(int);
    Widget(Widget&&);
    Widget(const Widget&) = delete;
   ~Widget();
    Widget& operator=(Widget&&);
    Widget& operator=(const Widget&) = delete;
    void draw();
};

// Widget.cpp
class Widget::Impl {
    int n;
public:
    Impl(int n) : n(n)
    {}
    void draw(const Widget& w) {
        std::cout << "Widget::Impl::draw(" << n << ")" << std::endl;
    }
};
Widget::Widget(int n) : impl(std::make_unique<Impl>(n))
{}
Widget::Widget(Widget&&) = default;
Widget& Widget::operator=(Widget&&) = default;
Widget::~Widget() = default;

void Widget::draw() { impl->draw(*this); }

// I30. Encapsulate rule violations

class Istream
{
public:
    enum Option {
        FromLine = 1
    };
    Istream()
    {}
    Istream(std::string s) : mOwn(true), mIn(new std::ifstream(s))
    {}
    Istream(std::string s, Option) : mOwn(true), mIn(new std::ifstream(s))
    {}
   ~Istream()
    { if (mOwn) delete mIn; }

    operator std::istream&(){ return *mIn; }

private:
    bool mOwn = false;
    std::istream* mIn = &std::cin;
};

} // namespace ================================================================

void test_i_13()
{
    int array[] = { 1, 2, 3, 4, 5 };

    std::cout << "Bad interface" << std::endl;
    procBad(array, 5);

    std::cout << "Good interface" << std::endl;
#if (0)
    procGood(std::span<int>{array});
#else
    procGood(array);
#endif
}

void test_i27()
{
    Widget w(5);

    w.draw();
}
