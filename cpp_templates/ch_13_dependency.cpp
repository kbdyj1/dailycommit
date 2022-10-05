#include <iostream>

namespace { //=================================================================

template <typename T>
class Trab {
public:
    enum { x };
};

template <typename T>
class Victim {
public:
    int y;
    void poof() {
//        Trab<T>::x * y;
    }
};

template <>
class Trab<void> {
public:
    using x = int;
};

void boom(Victim<void>& bomb)
{
    bomb.poof();
}

void test_victim()
{
    auto v = Victim<void>{};

    boom(v);
}

template <typename T>
class Shell {
public:
    template<int N>
    class In {
    public:
        template <int M>
        class Deep {
        public:
            virtual void f() {
                std::cout << "Deep<M>::f()\n";
            }
        };
    };
};

template <typename T, int N>
class Weired {
public:
    void case0(typename Shell<T>::template In<N>::template Deep<N>* p) {
        std::cout << "case0()\n";
        p->f();
    }
    void case1(typename Shell<T>::template In<N>::template Deep<N>& p) {
        std::cout << "case1()\n";
        p.f();
    }
};

void test_deep_template()
{
    auto w = Weired<int, 3>{};
    auto shell = Shell<int>::In<3>::Deep<3>{};

    w.case0(&shell);
    w.case1(shell);
}


//-------------------------------------------------------------------
//  Using
//-------------------------------------------------------------------

class BX {
public:
    void f(int) {
        std::cout << "f(int)\n";
    }
    void f(const char*) {
        std::cout << "f(const char*>\n";
    }
    void g();
};

class DX : BX {
public:
    using BX::f;
};

void test_using()
{
    auto d = DX{};

    d.f(0);
}

template <typename T>
class BT {
public:
    using Mystery = T;

    template <typename U>
    struct Magic;
};

template <typename T>
class DT : private BT<T> {
public:
    using typename BT<T>::Mystery;

    Mystery* p;

    template <typename U>
    using Magic = typename BT<T>::template Magic<T>;

    Magic<T>* plink;
};

namespace N { //-----------------------------------------------------

class X {};

template <int I>
void sel(X*);

} //N ---------------------------------------------------------------

#define ENABLE_ADL

#if defined(ENABLE_ADL)
template <typename T> void sel();
#endif

void h(N::X* x)
{
    sel<3>(x);
}

namespace N {
template <int I>
void sel(X* p) {
    std::cout << "N::X::sel(N::X*)\n";
}
}

void test_adl()
{
    auto x = N::X{};
    h(&x);
}

namespace error { //-------------------------------------------------

void f() {}

template <int I>
void nonDependentCall()
{
#if (0)
    f(I);
#endif
}

template <int N>
void instantiationDependentBound()
{
    constexpr int x = sizeof(N);
    constexpr int y = sizeof(N) + 1;
    // 'array' declared as an array with a negative size
#if (0)
    int array[x - y];
#endif
}

} //error -----------------------------------------------------------

} //namespace =================================================================

void test_ch_13_dependency()
{
#if (0) //done
    test_victim();
    test_deep_template();
    test_using();
#endif

    test_adl();
}
