#include <iostream>
#include <type_traits>

namespace { //=================================================================

namespace _1 {

void test()
{
    int i = 10;
    std::add_const<int>::type ci = i;
    static_assert (std::is_const<decltype(ci)>::value, "not const !!!");

    std::cout << std::is_same<decltype(ci), const int>::value << "\n";
    std::cout << std::is_same<decltype(ci), const int>{} << "\n";

    typedef std::integral_constant<int, 10> ten;

    std::cout << ten::value << "\n";
    std::cout << ten{} << "\n";
    std::cout << typeid(ten::value_type).name() << "\n";

    using Type = int;
    auto ic = std::is_const<Type>();
    std::cout << "ic: " << ic() << "\n";

    static constexpr auto isConst = std::is_const<Type>();
    if constexpr (isConst) {
        std::cout << "isConst\n";
    } else {
        std::cout << "isConst: false\n";
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

void func(std::string&& s)
{
    std::cout << std::is_lvalue_reference<decltype(s)>::value << "\n";
    std::cout << std::is_rvalue_reference<decltype(s)>::value << "\n";

    std::cout << std::is_lvalue_reference<decltype((s))>::value << "\n";
    std::cout << std::is_rvalue_reference<decltype((s))>::value << "\n";
}

void test()
{
    func("Hello, Qt");
}

} //_2 --------------------------------------------------------------

namespace _3 {

void func(int a[], int b[8], int* c)
{
    std::cout << "std::is_array -------------------------------------\n";
    std::cout << "int a[]: " << std::is_array<decltype(a)>::value << "\n";
    std::cout << "int b[8]: " << std::is_array<decltype(b)>::value << "\n";
    std::cout << "int* c: " << std::is_array<decltype(c)>::value << "\n\n";

    std::cout << "std::is_pointer -----------------------------------\n";
    std::cout << "int a[]: " << std::is_pointer<decltype(a)>::value << "\n";
    std::cout << "int b[8]: " << std::is_pointer<decltype(b)>::value << "\n";
    std::cout << "int* c: " << std::is_pointer<decltype(c)>::value << "\n";
}

void test()
{
    std::cout << "std::is_array<int[]>: " << std::is_array<int[]>::value << "\n";
    std::cout << "std::is_array<int[4]>: " << std::is_array<int[4]>::value << "\n";

    int a[8];
    int b[8];
    int c[8];

    func(a, b, c);
}

} //_3 --------------------------------------------------------------

namespace _4 {

class A {
public:
    void member();
};

void test()
{
    std::cout << std::is_member_object_pointer<int(A::*)>::value << "\n";
    std::cout << std::is_member_function_pointer<decltype(&A::member)>::value << "\n";

    auto l = []{};
    std::cout << "std::is_class<[]{}>: " << std::is_class<decltype(l)>::value << "\n";
}

} //_4 --------------------------------------------------------------

// is_arithmetic = is_integral + is_floating_point
// is_fundamental = is_arithmetic + is_void + is_null_pointer or !is_compound
// is_scalar = is_fundamental + is_enum + is_pointer + is_member_pointer
// is_object = is_scalar + is_array + is_class + is_union
// is_compound = is_enum + is_array + is_class + is_union + is_reference + is_pointer + is_member_pointer + is_function

namespace _5 {

class A
{
public:
    static const int value = 0;
};
class B
{
public:
    virtual ~B()
    {}
};

enum Error {
    NoError = 0,
    InvalidParam
};

class C {
public:
#if (0)
    virtual ~C()
    {}
#endif

    int a;
    std::string b;
    char c;
};

class D : public C
{
public:
    int d[8];
};
class E
{
public:
    bool operator()(int) {
        return true;
    }
};

void test()
{
    std::cout << "std::is_signed<char>: " << std::is_signed<char>::value << "\n";
    std::cout << "std::is_empty<A>: " << std::is_empty<A>::value << "\n";
    std::cout << "std::is_pod<A>: " << std::is_pod<A>::value << "\n";
    std::cout << "std::is_polymorphic<B>: " << std::is_polymorphic<B>::value << "\n";
    std::cout << "std::is_abstract<B>: " << std::is_abstract<B>::value << "\n";
    std::cout << "std::has_virtual_destructor<B>: " << std::has_virtual_destructor<B>::value << "\n";
    std::cout << "std::rank<int[][8][8]>: " << std::rank<int[][8][8]>::value << "\n";

    std::cout << "std::extent<int[3][4], 0>: " << std::extent<int[3][4], 0>::value << "\n";
    std::cout << "std::extent<int[3][4], 1>: " << std::extent<int[3][4], 1>::value << "\n";

    const auto ext = std::extent<int[9]>{};
    std::cout << "ext: " << ext << "\n";

    std::underlying_type<Error>::type t0;
    std::cout << typeid(t0).name() << "\n"; //j : unsigned int

    std::cout << "std::is_aggregate<C>: " << std::is_aggregate<C>::value << "\n";
    std::cout << "std::is_aggregate<D>: " << std::is_aggregate<D>::value << "\n";

    std::cout << "std::is_trivial<C>: " << std::is_trivial<C>::value << "\n";
    std::cout << "std::is_standard_layout<C>: " << std::is_standard_layout<C>::value << "\n";
    std::cout << "std::is_pod<C>: " << std::is_pod<C>::value << "\n";

    std::cout << "std::is_invocable<E, int>: " << std::is_invocable<E, int>::value << "\n";
    std::cout << "std::is_invocable<int(*)(int), int>: " << std::is_invocable<int(*)(int), int>::value << "\n";
    std::cout << "std::is_invocable_r<bool, E, int>: " << std::is_invocable_r<bool, E, int>::value << "\n";
}

} //_5 --------------------------------------------------------------

namespace _6 {

class A {
public:
    A()
    {}
    virtual ~A() = default;
};

void test()
{
    std::cout << "std::is_default_constructible<A>: " << std::is_default_constructible<A>::value << "\n";
    std::cout << "std::is_trivially_default_constructible<A>: " << std::is_trivially_default_constructible<A>::value << "\n";
    std::cout << "std::is_nothrow_default_constructible<A>: " << std::is_nothrow_default_constructible<A>::value << "\n";

    std::cout << "std::is_copy_constructible<A>: " << std::is_copy_constructible<A>::value << "\n";
    std::cout << "std::is_trivially_copy_constructible<A>: " << std::is_trivially_copy_constructible<A>::value << "\n";
    std::cout << "std::is_nothrow_copy_constructible<A>: " << std::is_nothrow_copy_constructible<A>::value << "\n";

    std::cout << "std::is_destructible<A>: " << std::is_destructible<A>::value << "\n";
    std::cout << "std::is_trivially_destructible<A>: " << std::is_trivially_destructible<A>::value << "\n";
    std::cout << "std::is_nothrow_destructible<A>: " << std::is_nothrow_destructible<A>::value << "\n";
}

} //_6 --------------------------------------------------------------

namespace _7 {

void test()
{
    auto a = nullptr;
    auto b = nullptr;

    std::cout << "std::is_same<decltype(a), decltype(b)>: " << std::is_same<decltype(a), decltype(b)>::value << "\n";

    auto la = [](int){};
    auto lb = [](int){};
    auto lc = lb;

    std::cout << "std::is_same<decltype(la), decltype(lb)>: " << std::is_same<decltype(la), decltype(lb)>::value << "\n";
    std::cout << "std::is_same<decltype(lc), decltype(lb)>: " << std::is_same<decltype(lc), decltype(lb)>::value << "\n";
}

} //_7 --------------------------------------------------------------

namespace _8 {

class A
{};
class B0 : A
{};
class B1 : A
{};
class C : private B0, private B1
{};

class D
{
public:
    explicit D(const D&)
    {}
};

void test()
{
    std::cout << "std::is_base_of<A, B0>: " << std::is_base_of<A, B0>::value << "\n";
    std::cout << "std::is_base_of<A, C>: " << std::is_base_of<A, C>::value << "\n";
    std::cout << "std::is_base_of<A&, C&>: " << std::is_base_of<A&, C&>::value << "\n";

    std::cout << "std::is_constructible<D, D>: " << std::is_constructible<D, D>::value << "\n";
    std::cout << "std::is_convertible<D, D>: " << std::is_convertible<D, D>::value << "\n";

    std::cout << typeid(std::remove_const_t<int const&>).name() << "\n";

    std::cout << typeid(std::add_cv_t<int>).name() << "\n";
}

} //_8 --------------------------------------------------------------

namespace _9 {

template <typename T>
struct Number
{
    T n;
};

template <typename T, typename U>
Number<typename std::common_type<T, U>::type> operator+(const Number<T>& l, const Number<U>& r)
{
    return {l.n + r.n};
}

#if (0)
using POD = std::aligned_union_t<0, char, std::pair<std::string, std::string>>;
#else
using POD = std::aligned_storage_t<5>;
#endif

void test()
{
    Number<int> i0 = {1}, i1 = {2};
    Number<double> d0 = {2.3}, d1 = {3.4};

    std::cout << "(i0 + i1).n: " << (i0 + i1).n << "\n";
    std::cout << "(i0 + d1).n: " << (i0 + d1).n << "\n";
    std::cout << "(d0 + i1).n: " << (d0 + i1).n << "\n";
    std::cout << "(d0 + d1).n: " << (d0 + d1).n << "\n";

    std::cout << "sizeof(POD): " << sizeof(POD) << '\n';
    std::cout << "alignment_value: " << std::aligned_union<0, char, std::pair<std::string, std::string>>::alignment_value << '\n';
}

} //_9 --------------------------------------------------------------

namespace _10 {

struct X {
    X(int);
};
struct Y;

template <typename T>
struct IsNoPtr : std::negation<std::disjunction<std::is_null_pointer<T>, std::is_member_pointer<T>, std::is_pointer<T>>>
{};

//c++17 conjunction, disjunction, negation

void test()
{
#if (0)
    static_assert(std::is_constructible<X, int>{} || std::is_constructible<Y, int>{}, "can't construct X{} or Y{}");
#endif

    static_assert(std::disjunction<std::is_constructible<X, int>, std::is_constructible<Y, int>>{}, "can't construct X{} or Y{}");

    std::cout << "IsNoPtr<int>::value: " << IsNoPtr<int>::value << '\n';
    std::cout << "IsNoPtr<int*>::value: " << IsNoPtr<int*>::value << '\n';
}

} //_10 -------------------------------------------------------------

} //namespace =================================================================

void test_appendix_std()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
    _5::test();
    _6::test();
    _7::test();
    _8::test();
    _9::test();
#endif

    _10::test();
}
