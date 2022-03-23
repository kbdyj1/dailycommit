#include <type_traits>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <typeinfo>

namespace detail {

void* voidfy(const volatile void* ptr) noexcept { return const_cast<void*>(ptr); }

}
namespace { //=================================================================

void print_separator()
{
    std::cout << "-----" << std::endl;
}

void test0()
{
    std::cout << "std::is_same<int, std::int32_t> : " << std::is_same<int, std::int32_t>::value << std::endl;
    std::cout << "std::is_same<int, std::int64_t> : " << std::is_same<int, std::int64_t>::value << std::endl;

    print_separator();

    long double num1 = 1.0;
    long double num2 = 2.0;
    std::cout << std::is_same_v<decltype(num1), decltype(num2)> << std::endl;

    print_separator();

    std::cout << "std::is_same_v<float, std::int32_t> : " << std::is_same_v<float, std::int32_t> << std::endl;

    print_separator();

    std::cout << "std::is_same_v<int, unsigned int> : " << std::is_same_v<int, unsigned int> << std::endl;
    std::cout << "std::is_same_v<int, signed int> : " << std::is_same_v<int, signed int> << std::endl;

    print_separator();

    // unlike other types, 'char' is nether 'unsigned' nor 'signed'
    std::cout << "std::is_same_v<char, unsigned char> : " << std::is_same_v<char, unsigned char> << std::endl;
    std::cout << "std::is_same_v<char, signed char> : " << std::is_same_v<char, signed char> << std::endl;

    print_separator();

    std::cout << "std::is_same_v<int, const int> : " << std::is_same_v<int, const int> << std::endl;
    std::cout << "std::is_same_v<int, std::decay<const int>::type> : " << std::is_same_v<int, std::decay<const int>::type> << std::endl;
}

void test1()
{
    typedef std::integral_constant<int, 2> two_t;
    typedef std::integral_constant<int, 4> four_t;

    std::cout << "two_t::value * 2 == four_t::value : " << (two_t::value * 2 == four_t::value) << std::endl;

    enum E {
        e0,
        e1
    };
    typedef std::integral_constant<E, E::e0> E0;
    typedef std::integral_constant<E, E::e1> E1;

    std::cout << "E0() == E1::value : " << (E0() == E1::value) << std::endl;
}

struct T
{
    enum { int_t, float_t } type;

    template <typename Integer, std::enable_if<std::is_integral<Integer>::value, bool> = true
    >
    T(Integer) : type(int_t)
    {}

    template <typename Floating, std::enable_if<std::is_floating_point<Floating>::value, bool> = true
    >
    T(Floating) : type(float_t)
    {}
};

template <typename T>
typename std::enable_if<std::is_trivially_default_constructible<T>::value>::type
construct(T*)
{
    std::cout << "default constructing trivially default constructible T" << std::endl;
}

template <typename T>
typename std::enable_if<!std::is_trivially_default_constructible<T>::value>::type
construct(T* p)
{
    std::cout << "default constructing non-trivially default constructible T" << std::endl;
    ::new(detail::voidfy(p)) T;
}

template <typename T, class... Args>
std::enable_if_t<std::is_constructible<T, Args&&...>::value>
construct(T* p, Args&&... args)
{
    std::cout << "constructing T with operation" << std::endl;
    ::new(detail::voidfy(p)) T(static_cast<Args&&>(args)...);
}

template <class T>
void destroy(
    T*,
    typename std::enable_if<std::is_trivially_destructible<T>::value>::type* = 0)
{
    std::cout << "destroying trivially destrutible T" << std::endl;
}

template <class T,
          typename std::enable_if<
              !std::is_trivially_destructible<T>{} &&
              (std::is_class<T>{} || std::is_union<T>{}),
              bool>::type = true>
void destroy(T* t)
{
    std::cout << "destroying non-trivally destructible T" << std::endl;
    t->~T();
}

template <class T, typename = typename std::enable_if<std::is_array<T>::value>::type>
void destroy(T* t)
{
    for (std::size_t i = 0; i < std::extent<T>::value; ++i) {
        destroy((*t)[i]);
    }
}

template <class T, class Enable = void>
class A{};

template <class T>
class A<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{};

void test2()
{
    std::aligned_union_t<0, int, std::string> u;

    construct(reinterpret_cast<int*>(&u));
    destroy(reinterpret_cast<int*>(&u));

    construct(reinterpret_cast<std::string*>(&u), "Hello, Qt");
    destroy(reinterpret_cast<std::string*>(&u));

    A<int>{};
    A<double>{};
}

template <class T>
struct my_decay
{
private:
    typedef typename std::remove_reference<T>::type U;

public:
    typedef typename std::conditional<
        std::is_array<U>::value,
        typename std::remove_extent<U>::type*,
        typename std::conditional<
            std::is_function<U>::value,
            typename std::add_pointer<U>::type,
            typename std::remove_cv<U>::type
        >::type
    >::type type;
};

template <class T, class U>
struct decay_equiv : std::is_same<typename my_decay<T>::type, U>::type
{};

void test3()
{
    std::cout << "decay_equiv<int, int> : " << decay_equiv<int, int>::value << std::endl;
    std::cout << "decay_equiv<int&, int> : " << decay_equiv<int&, int>::value << std::endl;
    std::cout << "decay_equiv<int&&, int> : " << decay_equiv<int&&, int>::value << std::endl;
    std::cout << "decay_equiv<const int&, int> : " << decay_equiv<const int&, int>::value << std::endl;
    std::cout << "decay_equiv<int[2], int*> : " << decay_equiv<int[2], int*>::value << std::endl;
    std::cout << "decay_equiv<int(int), int(*)(int)> : " << decay_equiv<int(int), int(*)(int)>::value << std::endl;
}

//-------------------------------------------------------------------
//  conditional
//-------------------------------------------------------------------

template <bool B, class T, class F>
struct my_conditional { using type = T; };

template <class T, class F>
struct my_conditional<false, T, F> { using type = F; };

void test4()
{
    typedef my_conditional<true, int, double>::type Type1;
    typedef my_conditional<false, int, double>::type Type2;
    typedef my_conditional<sizeof(int) >= sizeof(double), int, double>::type Type3;

    std::cout << typeid(Type1).name() << std::endl; // i
    std::cout << typeid(Type2).name() << std::endl; // d
    std::cout << typeid(Type3).name() << std::endl; // d
}

//-------------------------------------------------------------------
//  is_function
//-------------------------------------------------------------------

struct B {
    int fun() const&;
};
template <typename>
struct PM_traits
{};

template <class T, class U>
struct PM_traits<U T::*> {
    using member_type = U;
};

int func();

void test5()
{
    std::cout << "std::is_function<B> : " << std::is_function<B>::value << std::endl;
    std::cout << "std::is_function<int(int)> : " << std::is_function<int(int)>::value << std::endl;
    std::cout << "std::is_function<decltype (func)> : " << std::is_function<decltype (func)>::value << std::endl;
    std::cout << "std::is_function<int> : " << std::is_function<int>::value << std::endl;

    using T = PM_traits<decltype (&B::fun)>::member_type;
    std::cout << "std::is_function<T> : " << std::is_function<T>::value << std::endl;
}

//-------------------------------------------------------------------
//  add_pointer
//-------------------------------------------------------------------

void test6()
{

}

//-------------------------------------------------------------------
//  remove_extent
//-------------------------------------------------------------------
template <class T>
struct my_remove_extent { typedef T type; };

template <class T>
struct my_remove_extent<T[]> { typedef T type; };

template <class T, std::size_t N>
struct my_remove_extent<T[N]> { typedef T type; };

template <class A>
typename std::enable_if< std::rank<A>::value == 1 >::type
print_1d(const A& a)
{
#if (0)
    std::copy(a, a+std::extent<A>::value, std::ostream_iterator<typename my_remove_extent<A>::type>(std::cout, " "));
#else
    std::copy(a, a+3, std::ostream_iterator<int>(std::cout, " "));
#endif
    std::cout << std::endl;
}

void test7()
{
    int a[][3] = {{1, 2, 3}, {4, 5, 6}};
    //print_1d(a);
    print_1d(a[1]); //4 5 6
}

//-------------------------------------------------------------------
//  rank
//-------------------------------------------------------------------

void test8()
{
    std::cout << std::rank<int>::value << std::endl;
    std::cout << std::rank<int[4]>::value << std::endl;
    std::cout << std::rank<int[2][3]>::value << std::endl;
    std::cout << std::rank<int[][3][4]>::value << std::endl;
    std::cout << std::rank<int[]>::value << std::endl;
}

//-------------------------------------------------------------------
//  is_object
//-------------------------------------------------------------------

void test9()
{

}

//-------------------------------------------------------------------
//  extent
//-------------------------------------------------------------------

void test10()
{
    std::cout << std::extent<int[3]>::value << std::endl;
    std::cout << std::extent<int[3][4][5], 0>::value << std::endl;
    std::cout << std::extent<int[3][4][5], 1>::value << std::endl;
    std::cout << std::extent<int[3][4][5], 2>::value << std::endl;
    std::cout << std::extent<int[]>::value << std::endl;

    const auto ext = std::extent<int[9]>{};
    std::cout << ext << std::endl;

    const int array[] = {1, 2, 3, 4};
    std::cout << std::extent<decltype (array)>::value << std::endl;
}

} // namespace ================================================================

void test_typeTraits()
{
    std::cout << std::boolalpha << std::endl;

    //test0();
    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test7();
    test8();
    //test10();
}
