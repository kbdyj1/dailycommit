#include <iostream>

namespace { //=================================================================

namespace _1 { //full specialization

template <typename T>
class A {
public:
    void info() { std::cout << "A<T>()\n"; }
};

template <>
class A<void> {
public:
    void info() { std::cout << "A<void>()\n"; }
};

template <typename T>
class Types {
public:
    using I = int;
};

template <typename T, typename U = typename Types<T>::I>
class B
{};

template <>
class B<void> {
public:
    void f()
    {}
};

template <>
class B<char, char>
{};

#if (0)
template <>
class B<char, 0>    //template argument for template type parameter must be a type
{};
#endif

void test()
{
    B<int>* pi;
    B<int> b0;
    B<void>* pv;
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
class Out {
public:
    template<typename U>
    class In {
    };
};

template <>
class Out<void> {
    template<typename U>
    class In {
        static int count;
    };
};

template <typename U>
int Out<void>::In<U>::count = 1;

template <typename T>
class Invalid
{};

Invalid<double> x0;

#if (0) //has already been instantiated
template <>
class Invalid<double>;
#endif

} //_2 --------------------------------------------------------------

namespace _3 { //function

template <typename T>
std::string f(T)
{
    return "f(T)";
}

template <typename T>
std::string f(T*)
{
    return "f(T*)";
}

template <>
#if (0)
std::string f(int x = 0) //default argument not permitted on an explicit specialization of function 'f'
#else
std::string f(int x)
#endif
{
    return "f(int)";
}

template <>
std::string f(int*)
{
    return "f(int*)";
}

} //_3 --------------------------------------------------------------

namespace _4 { //variable template specialization

template <typename T>
constexpr std::size_t SZ = sizeof(T);

template <>
constexpr std::size_t SZ<void> = 0;

} //_4 --------------------------------------------------------------

namespace _5 { //full member specialization

template <typename T>
class Outer {
    static int code;
public:
    template<typename U>
    class Inner {
        static int count;
    };

    void print() const {
        std::cout << "Outer<T>::print()\n";
    }
};

template <typename T>
int Outer<T>::code = 0;

template <typename T>
template <typename U>
int Outer<T>::Inner<U>::count = 0;

template <>
class Outer<bool> {
public:
    template<typename U>
    class Inner {
        static int count;
    };

    void print() const {
        std::cout << "Outer<bool>::print()\n";
    }
};

template <>
template <typename X>
class Outer<wchar_t>::Inner {
public:
    static long count;
};

template <>
template <typename X>
long Outer<wchar_t>::Inner<X>::count = 10;

template <>
template <>
class Outer<char>::Inner<wchar_t> {
public:
    enum {
        count = 5
    };
};

void test()
{
    auto o0 = Outer<int>{};
    o0.print();

    auto o1 = Outer<bool>{};
    o1.print();
}

class DefaultInitOnly {
public:
    DefaultInitOnly() = default;
    DefaultInitOnly(const DefaultInitOnly&) = delete;
};

template <typename T>
class Statics {
    static T value;
};

template <>
DefaultInitOnly Statics<DefaultInitOnly>::value{};

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T>
class List {
public:
    void append(const T&);
    inline std::size_t size() const;
};

template <>
class List<void*> {
public:
    void append(const void*)
    {}
    inline std::size_t size() const {
        return 0;
    }
};

template <typename T>
class List<T*> {
    List<void*> impl;
public:
    inline void append(T* p) {
        impl.append(p);
    }
    inline std::size_t size() const {
        return impl.size();
    }
};

template <typename T, int I = 4>
class A;

//template <typename T>
//class A<int, T>;

//template <typename T=int>
//class A<T, 10>;

//template <int I>
//class A<int, I*2>;

//template <typename T, int U>
//class A<U, T>;

template <typename... Ts>
class Tuple;

//template <typename Tail, typename... Ts>
//class Tuple<Ts..., Tail>;

template <typename Tail, typename... Ts>
class Tuple<Tuple<Ts...>, Tail>;

} //_6 --------------------------------------------------------------

} //namespace =================================================================

#include "g.hpp"

namespace g {

template <>
int func(int, int init)
{
    return init/2;
}

void test()
{
    std::cout << func(1.0, 2.0) << "\n";
    std::cout << func(1, 2) << "\n";
}

}

void test_ch_16_specialization()
{
#if (0) //done
    g::test();
#endif

    _5::test();
}
