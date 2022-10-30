//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.21 Template & Inheritance
//
//  22/10/29
//=============================================================================

#include <iostream>

namespace { //=================================================================

namespace _1 { //EBCO

class A {
    static const int value = 0;
};
class B
{
    using Type = int;
};
class C : public A, public B
{
    char c;
};

void test()
{
    std::cout << "sizeof(A): " << sizeof(A) << "\n";
    std::cout << "sizeof(B): " << sizeof(B) << "\n";
    std::cout << "sizeof(C): " << sizeof(C) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename Base, typename Member>
class BaseMemberPair : private Base {
    Member m;
public:
    BaseMemberPair(const Base& base, const Member& member) : Base(base), m(member)
    {}
    const Base& base() const {
        return static_cast<const Base&>(*this);
    }
    Base& base() {
        return static_cast<Base&>(*this);
    }
    const Member& member() const {
        return this->m;
    }
    Member& member() {
        return this->m;
    }
};

template <typename T>
class Optimizable {
    BaseMemberPair<T, void*> storage;
};

class A{
public:
    A() = default;
    A(const A&) = default;
};

void test()
{
    auto a = A{};
    std::cout << sizeof(a) << "\n";

//    auto o = Optimizable<A>{};
}

} //_2 --------------------------------------------------------------

namespace _3 { //CRTP

template <typename T>
class ObjectCounter {
    inline static std::size_t count = 0;

protected:
    ObjectCounter() {
        ++count;
    }
    ObjectCounter(const ObjectCounter<T>&) {
        ++count;
    }
    ObjectCounter(ObjectCounter<T>&&) {
        ++count;
    }
    ~ObjectCounter() {
        --count;
    }

public:
    static std::size_t live() {
        return count;
    }
};

template <typename Char>
class String : public ObjectCounter<String<Char>> {
public:
    ~String() = default;
};

void test()
{
    auto s0 = String<char>();
    auto s1 = String<char>();
    auto s2 = String<wchar_t>();

    std::cout << "String<char>::live(): " << String<char>::live() << "\n";
    std::cout << "String<wchar_t>::live(): " << String<wchar_t>::live() << "\n";
}

} //_3 --------------------------------------------------------------

namespace _4 { //Barton & Nackman trick

template <typename T>
class Array {
    static bool equal(const Array<T>& l, const Array<T>& r) {
        return true;
    }

public:
    friend bool operator==(const Array<T>& a, const Array<T>& b) {
        return equal(a, b);
    }
};

class A
{};

template <typename T>
class Wrapper {
    T obj;

public:
    Wrapper(T o) : obj(o)
    {}
    friend void foo(const Wrapper<T>&) {

    }
};

void test()
{
    auto a = A{};
    auto w = Wrapper<A>{a};

    foo(w);
#if (0)
    foo(a);
#endif
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <typename Derived>
class EqualityCompariable {
public:
    friend bool operator!=(const Derived& d0, const Derived& d1) {
        return !(d0 == d1);
    }
};

class A : public EqualityCompariable<A> {
public:
    friend bool operator==(const A& a0, const A& a1) {
        return true;
    }
};

void test()
{
    auto a0 = A{};
    auto a1 = A{};

    std::cout << (a0 != a1) << "\n";
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_21_facade();
void test_ch_21_named_template_arg();

void test_ch_21()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _3::test();
    _5::test();
    test_ch_21_facade();
#endif

    test_ch_21_named_template_arg();
}
