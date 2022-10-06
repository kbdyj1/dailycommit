#include <iostream>

namespace { //=================================================================

template <typename T>
T f(T p)
{
    return p*2;
}

void test_decltype()
{
    decltype(f(2)) x = 2;   // partial instantiation

    std::cout << typeid(x).name() << "\n";
}

//-------------------------------------------------------------------

template <typename T>
class A {
    using Type = typename T::Type;
};

template <typename T>
T v = T::default_value();

void test_partial_instantiation()
{
    A<int>* p = nullptr;

    auto s = decltype(v<int>){};

    std::cout << typeid(s).name() << "\n";
}

//-------------------------------------------------------------------

template <typename T>
class Safe
{};

template <int N>
class Danger {
    int array[N];
};

template <typename T, int N>
class Tricky {
public:
    void noBodyHere(Safe<T> = 3);

    void inClass() {
        auto d = Danger<N>{};
    }

    struct Nested {
        Danger<N> danger;
    };

    union {
        Danger<N> anonymouse;
        int align;
    };

    void unsafe(T(*p)[N]);

    void error() {
        Danger<-1> damm;
    }
};

void test_error()
{
#if (0)
    auto inst = Tricky<int, -1>{};
#endif
}

//-------------------------------------------------------------------

template <typename T>
class V {
public:
    virtual ~V(){}
    virtual T vmem();
};

void test_virtual()
{
    auto inst = V<int>{};
}

template <typename T>
T V<T>::vmem()
{
    return T{};
}

//-------------------------------------------------------------------

template <typename T>
class B {
public:
    T operator->();
};

void test_operator()
{
    auto inst = B<int>{};
}

} //namespace =================================================================

void test_ch_14_lazy()
{
#if (0) //done
    test_decltype();
    test_partial_instantiation();
    test_virtual();
#endif

    test_operator();
}
