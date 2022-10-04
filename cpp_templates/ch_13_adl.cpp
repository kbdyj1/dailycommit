#include <iostream>

namespace { //=================================================================

namespace X {

template <typename T>
void f(T) {
    std::cout << "X::f(T)\n";
}

} //X ---------------------------------------------------------------

namespace N {

using namespace X;

enum E { e0 };

void f(E) {
    std::cout << "N::f(E)\n";
}

} //N ---------------------------------------------------------------

void f(int) {
    std::cout << "f(int)\n";
}

void test_f()
{
    ::f(N::e0); //qualified name (not ADL)

    f(N::e0);
}

template <typename T>
class C {
    friend void f() {
        std::cout << "friend void f()\n";
    }
    friend void f(const C<T>&) {
        std::cout << "friend void f(const C<T>&)\n";
    }
};

void g(C<int>* p)
{
#if (0) //compile error
    f();
#endif
    f(*p);
}

namespace injection {

int C;

class C {
    int i[2];

public:
    static int h() {
        return sizeof(C);
    }
};

} // injection ------------------------------------------------------

template <template <typename> class TT>
class Y {
};

template <typename T>
class Z {
    Z* z0;
    Z<void>& z1;
    Y<Z> z2;
    Y<::Z> z3;
};

template <int I, typename... T>
class V {
    V* a;   // V<I, T...>*
    //V<0, void> b;
};

template <typename T>
class Node {
    using Type = T;

    // current instantiation
    Node* next;
    Node<Type>* prev;

    // unknown specialization
    Node<T*>* parent;
};

template <typename T>
class D {
    using Type = T;

    struct I {
        D* d0;
        D<Type>* d1;
        I* i;
    };

    struct J {
        D* d0;
        D<Type>* d1;
        I* i;
        J* j;
    };
};

} //namespace =================================================================

int h() {
    return sizeof(injection::C);
}

void injection_test()
{
    std::cout << "injection::C::h(): " << injection::C::h() << ", ::h(): " << ::h() << "\n";
}

void test_ch_13_adl()
{
#if (0) //done
    test_f();
#endif

    injection_test();
}
