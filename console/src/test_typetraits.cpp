#include <type_traits>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <typeinfo>

#define SHOW(item) \
    std::cout << #item << " : " << item << "\n"

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
//
//  If T is a reference type, then provides the member typedef type which is a pointer to the referred type.
//  Otherwise, if T names an object type, a function type that is not cv- or ref-qualified,
//  or a (possibly cv-qualified) void type, provides the member typedef type which is the type T*.
//  Otherwise (if T is a cv- or ref-qualified function type), provides the member typedef type which is the type T.
//-------------------------------------------------------------------

template <class T>
struct type_identity { using type = T; };

template <class T>
auto try_add_pointer(int) -> type_identity<typename std::remove_reference<T>::type*>;
template <class T>
auto try_add_pointer(...) -> type_identity<T>;

template <class T>
struct my_add_pointer : decltype(try_add_pointer<T>(0)) {};

template <typename F, typename Class>
void ptr_to_member_func_cvref_test(F Class::*)
{
    using FF = std::add_pointer_t<F>;
    static_assert(std::is_same_v<F, FF>, "FF should be precisely F");
}

struct C
{
    void f_ref() & {}
    void f_const() const {}
};

void test6()
{
    int i = 123;
    int& ri = i;
    typedef std::add_pointer<decltype (i)>::type IntPtr;
    typedef std::add_pointer<decltype (ri)>::type IntRefPtr;
    IntPtr pi = &i;
    std::cout << "i = " << i << std::endl;
    std::cout << "*pi = " << *pi << std::endl;

    std::cout << "std::is_pointer<IntPtr> : " << std::is_pointer<IntPtr>::value << std::endl;
    std::cout << "std::is_same<IntPtr, int*> : " << std::is_same<IntPtr, int*>::value << std::endl;
    std::cout << "std::is_same<IntRefPtr, IntPtr> : " << std::is_same<IntRefPtr, IntPtr>::value << std::endl;

    typedef std::remove_pointer<IntPtr>::type Int;
    Int j = i;
    std::cout << "j = " << j << std::endl;

    std::cout << "std::is_pointer<Int> : " << std::is_pointer<Int>::value << std::endl;
    std::cout << "std::is_same<Int, int> : " << std::is_same<Int, int>::value << std::endl;

    ptr_to_member_func_cvref_test(&C::f_ref);
    ptr_to_member_func_cvref_test(&C::f_const);
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

template <class T>
struct my_is_object : std::integral_constant<bool,
        std::is_scalar<T>::value ||
        std::is_array<T>::value ||
        std::is_union<T>::value ||
        std::is_class<T>::value>
{};

class D
{
};

void test9()
{
    std::cout << "std::is_object<int> : " << std::is_object<int>::value << std::endl;
    std::cout << "std::is_object<int&> : " << std::is_object<int&>::value << std::endl;
    std::cout << "std::is_object<D> : " << std::is_object<D>::value << std::endl;
    std::cout << "std::is_object<D&> : " << std::is_object<D&>::value << std::endl;
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

//-------------------------------------------------------------------
//  is_scalar
//-------------------------------------------------------------------

template <class T>
struct my_is_scalar : std::integral_constant<bool,
        std::is_arithmetic<T>::value ||
        std::is_enum<T>::value ||
        std::is_pointer<T>::value ||
        std::is_member_pointer<T>::value ||
        std::is_null_pointer<T>::value>
{};

template <typename Head, typename... Tail>
void are_scalars(Head&& head, Tail&&... tail)
{
    using T = std::decay_t<decltype (head)>;
    std::cout << typeid(T).name() << " is " << (std::is_scalar<T>::value ? "" : "not ") << "a scalar" << std::endl;
    if constexpr (sizeof... (Tail)) {
        are_scalars(std::forward<decltype (tail)>(tail)...);
    }
}

struct S { int m; };

void test11()
{
    S s;
    int S::* p = &S::m;
    enum class E { e };

    //error: constexpr if condition evaluates to 6, which cannot be narrowed to type 'bool' [-Wc++11-narrowing]
    //    if constexpr (sizeof... (Tail)) {
    //                  ^
#if (0)
    are_scalars(42, 3.14, E::e, "str", p, nullptr, s);
#endif
}

void test_is_void()
{
    std::cout << std::is_void<void>::value << "\n";
    std::cout << std::is_void<int>::value << "\n";
}

// std::is_pointer is false for std::nullptr_t because it is not a built-in pointer type
// typedef decltype (nullptr) nullptr_t;
void test_is_nullpointer()
{
    std::cout << "std::is_null_pointer<decltype(nullptr)>::value(" << std::is_null_pointer<decltype(nullptr)>::value << "), "
              << "std::is_null_pointer<int*>::value(" << std::is_null_pointer<int*>::value << ")\n"
              << "std::is_pointer<decltype(nullptr)>::value(" << std::is_pointer<decltype(nullptr)>::value << "), "
              << "std::is_pointer<int*>::value(" << std::is_pointer<int*>::value << ")\n";
}

void f(int*)
{
    std::cout << "f(int*)\n";
}
void f(double*)
{
    std::cout << "f(double*)\n";
}
void f(std::nullptr_t)
{
    std::cout << "f(nullptr_t)\n";
}

void test_nullptr_t()
{
    int* pi{};
    double *pd{};

    f(pi);
    f(pd);
    f(nullptr);
}

// bool
// char, char8_t, char16_t, char32_t
// wchar_t,
// short
// int
// long, long long
//
// Note : the behavior of a program that adds specializations for is_integral or is_integral_v is undefined
//
// template <class T>
// inline constexpr bool is_integral_v = std::is_integral<T>::value;

namespace isIntegral { //------------------------------------------------------

class A{};
enum E : int {};

template <class T>
T f(T i)
{
    static_assert(std::is_integral<T>::value, "Integral required");
    return i;
}

void test_is_integral()
{
    SHOW(std::is_integral<A>::value);
    SHOW(std::is_integral_v<E>);
    SHOW(std::is_integral_v<float>);
    SHOW(std::is_integral_v<int>);
    SHOW(std::is_integral_v<const int>);
    SHOW(std::is_integral_v<bool>);
    SHOW(f(123));
}

} // isIntegral ---------------------------------------------------------------

// float, double, long double

//template <class T>
//inline constexpr bool is_floating_point_v = std::is_floating_point<T>::value;

//template <class T>
//struct is_floating_point : std::integral_constant<
//        bool,
//        std::is_same_v<float, typename std::remove_cv<T>::type> ||
//        std::is_same_v<double, typename std::remove_cv<T>::type> ||
//        std::is_same_v<long double, typename std::remove_cv<T>::type>
//>
//{};

void test_is_floating_point()
{
    std::cout << "std::is_floating_point_v<float> : " << std::is_floating_point_v<float> << "\n";
    std::cout << "std::is_floating_point_v<const float> : " << std::is_floating_point_v<const float> << "\n";
    std::cout << "std::is_floating_point_v<volatile float> : " << std::is_floating_point_v<volatile float> << "\n";
    std::cout << "std::is_floating_point_v<float&>: " << std::is_floating_point_v<float&> << "\n";
    std::cout << "std::is_floating_point_v<double>: " << std::is_floating_point_v<double> << "\n";
    std::cout << "std::is_floating_point_v<double&>:" << std::is_floating_point_v<double&> << "\n";
    std::cout << "std::is_floating_point_v<int> : " << std::is_floating_point_v<int> << "\n";
}

class E
{};
void test_is_array()
{
    std::cout << "std::is_array_v<E> : " << std::is_array_v<E> << "\n";
    std::cout << "std::is_array_v<E[]> : " << std::is_array_v<E[]> << "\n";
    std::cout << "std::is_array_v<E[2]> : " << std::is_array_v<E[2]> << "\n";
    std::cout << "std::is_array_v<int[]> : " << std::is_array_v<int[]> << "\n";
    std::cout << "std::is_array_v<std::array<int, 3>> : " << std::is_array_v<std::array<int, 3>> << "\n";
}

namespace Enum { //------------------------------------------------------------

struct A { enum E{}; };
enum E{};
enum class EC : int {};

void test_is_enum()
{
    std::cout << "std::is_enum_v<A> : " << std::is_enum_v<A> << "\n";
    std::cout << "std::is_enum_v<E> : " << std::is_enum_v<E> << "\n";
    std::cout << "std::is_enum_v<A::E> : " << std::is_enum_v<A::E> << "\n";
    std::cout << "std::is_enum_v<int> : " << std::is_enum_v<int> << "\n";
    std::cout << "std::is_enum_v<EC> : " << std::is_enum_v<EC> << "\n";
}

} // Enum ---------------------------------------------------------------------

namespace Union { //-----------------------------------------------------------

struct A{};
typedef union {
    int a;
    float b;
} B;
struct C {
    B d;
};

void test_is_union()
{
    std::cout << "std::is_union_v<A> : " << std::is_union_v<A> << "\n";
    std::cout << "std::is_union_v<B> : " << std::is_union_v<B> << "\n";
    std::cout << "std::is_union_v<C> : " << std::is_union_v<C> << "\n";
    std::cout << "std::is_union_v<int> : " << std::is_union_v<int> << "\n";
}

} // Union --------------------------------------------------------------------

namespace Class { //-----------------------------------------------------------

namespace detail {
template <class T>
std::integral_constant<bool, !std::is_union_v<T>> test(int T::*);

template <class T>
std::false_type test(...);
} // namespace detail

template <class T>
struct is_class : decltype(detail::test<T>(nullptr))
{};

struct A{};
class B{};
enum class E{};
union U { class UC{}; };

void test_is_class()
{
    std::cout << "is_class<A> : " << is_class<A>::value << "\n";
    std::cout << "is_class<B> : " << is_class<B>::value << "\n";
    std::cout << "is_class<E> : " << is_class<E>::value << "\n";
    std::cout << "is_class<U> : " << is_class<U>::value << "\n";
    std::cout << "is_class<U::UC> : " << is_class<U::UC>::value << "\n";
}

} // Class --------------------------------------------------------------------

namespace Function { //--------------------------------------------------------

struct A {
    int fun() const&;
};
int f();

void test_is_function()
{
    std::cout << std::is_function_v<A> << "\n";
    std::cout << std::is_function_v<int(int)> << "\n";
    std::cout << std::is_function_v<decltype(f)> << "\n";
    std::cout << std::is_function_v<int> << "\n";
}

} // Function -----------------------------------------------------------------

} // namespace ================================================================

void test_typeTraits()
{
    std::cout << std::boolalpha << std::endl;

    //isIntegral::test_is_integral();
    //test_is_floating_point();
    //test_is_array();
    //Enum::test_is_enum();
    //Union::test_is_union();
    //Class::test_is_class();
    Function::test_is_function();
}
