#include <iostream>

namespace { //=================================================================

// Template definitions

// T.60 Minimize a template's context dependencies

template <typename Container>
void sort(Container& c)
{
    std::sort(std::begin(c), std::end(c));
}

template <typename Iter>
Iter algo(Iter first, Iter last)
{
    for ( ; first != last; ++first) {
        auto x = sqrt(*first);
        helper(first, x);
    }
}

// T.61 Do not over-parameterize members (SCARY)

namespace t61 {
namespace bad {

template <typename T, typename A=std::allocator<T>>
class List {
    struct Link* head;

public:
    struct Link {
        T elem;
        Link* pre;
        Link* suc;
    };
    using Iterator = Link;

    Iterator first() const { return head; }
};

} //bad

namespace good {

template <typename T>
struct Link {
    T elem;
    Link* pre;
    Link* suc;
};

template <typename T, typename A=std::allocator<T>>
class List {
    Link<T>* head;

public:
    using Iterator = Link<T>*;

    Iterator first() { return head; }
};

} //good

template <typename T>
struct MyAllocator
{};

} //t61

// T.62 Place non-dependent class template members in a non-templated base class

namespace t62 {

namespace bad {

template <typename T>
class Foo {
public:
    enum {
        V0 = 0,
        V1
    };
};

} //bad

namespace good {

struct FooBase {
    enum {
        V0 = 0,
        V1
    };
};

template <typename T>
class Foo : public FooBase {
public:
    // ...
};

} //good

} //t62

// T.64 Use specialization to provide alternative implementations of class template

// T.65 Use tag dispatch to provide alternative implementations of a function

namespace t65 {

struct tagPod {};
struct tagNonPod {};

template <typename T>
struct CopyTraits { using tag = tagNonPod; };

template <>
struct CopyTraits<int> { using tag = tagPod; };

template <typename Iter, typename Out>
Out copyHelper(Iter first, Iter last, Iter out, tagPod)
{
}

template <typename Iter, typename Out>
Out copyHelper(Iter first, Iter last, Iter out)
{
}

template <typename Iter, typename Out>
Out copy(Iter first, Iter last, Iter out)
{
    return copyHelper(first, last, out, typename CopyTraits<Iter>::tag{});
}

} //t65

// T.67 Use specialization to provide alternative implementation for irregular types

// T.68 Use {} rather than () within templates to avoid ambiguties

// T.69 Inside a template, don't make an unqualified non-member function call
//      unless you intend it to be a customization point
//  trait
//      a type alias to compute a type
//      a constexpr function to compute a value
//      a traditional traits template to be specialized on the user's type

namespace t69 {

template <typename T>
void callMemberF(T t)
{
    t.f();
}

template <typename T>
void callF(T t)
{
    f(t);
}

template <typename T>
struct TestTraits
{
    static void f(T t)
    {
        std::cout << "TestTraits::f(" << t << ")" << std::endl;
    }
};

template <typename T>
void invokeTraitsF(T t)
{
    TestTraits<T>::f(t);
}

} //t69

} //===========================================================================

void test_t_61()
{
    using namespace t61;

    bad::List<int> l0;
    bad::List<int, MyAllocator<bad::Link>> l1;

    good::List<int> l2;
    good::List<int, MyAllocator<good::Link<int>>> l3;
}
