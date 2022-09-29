#include <iostream>
#include <type_traits>

namespace { //=================================================================

template <typename T>
class A {
    static_assert (!std::is_same_v<std::remove_cv_t<T>, void>, "!void");

public:
    template <typename V>
    void f(V&& v) {
        if constexpr(std::is_reference_v<V>) {
            std::cout << "f(V&)\n";
        }
        if constexpr(std::is_convertible_v<std::decay_t<V>, T>) {
            std::cout << "V = T\n";
        }
        if constexpr(std::has_virtual_destructor_v<V>) {
            std::cout << "~V()\n";
        }
    }
};

class B {
public:
    virtual ~B()
    {}
};

class C : public B {
public:
    virtual ~C()
    {}
};

class D {
public:
};

void test_type_traits()
{
    auto a = A<B>{};
    auto c = C{};
    auto d = D{};

    std::cout << "a.f(c)\n";
    a.f(c);

    std::cout << "\na.f(d)\n";
    a.f(d);
}

template <typename T, typename U>
constexpr bool same = std::is_same_v<T, U>;

void test_remove()
{
    using type0 = std::remove_cv<const int>::type;
    using type1 = std::remove_cv<volatile int>::type;
    using type2 = std::remove_cv<const volatile int>::type;
    using type3 = std::remove_cv<const volatile int*>::type;
    using type4 = std::remove_cv<int* const volatile>::type;

    std::cout << "same<type0, int>: " << same<type0, int> << "\n";
    std::cout << "same<type1, int>: " << same<type1, int> << "\n";
    std::cout << "same<type2, int>: " << same<type2, int> << "\n";
    std::cout << "same<type3, int*>: " << same<type3, int*> << "\n";
    std::cout << "same<type3, const volatile int*>: " << same<type3, const volatile int*> << "\n";
    std::cout << "same<type4, int*>: " << same<type4, int*> << "\n";
}

template <typename T>
void f(T&& a)
{
    auto p = &a;
    auto q = std::addressof(a);

    std::cout << "p: " << p << "\n";
    std::cout << "q: " << q << "\n";
}

void test_addressof()
{
    int a;
    f(a);

    f(int{});
}

template <typename T0, typename T1, typename RT = std::decay_t<decltype(true ? std::declval<T0>() : std::declval<T1>())>>
RT max(T0 a, T1 b)
{
    return a < b ? b : a;
}

void test_declval()
{
    auto val = max(1, 2.5);
    std::cout << "max(1, 2.5): " << val << ", typeid: " << typeid(val).name() << "\n";

    auto val2 = max(2, 1.5);
    std::cout << "max(2, 1.5): " << val2 << ", typeid: " << typeid(val2).name() << "\n";
}

} //namespace =================================================================

void test_ch_11_util()
{
    std::cout << std::boolalpha;

#if (0) //done
    test_type_traits();
    test_remove();
    test_addressof();
#endif

    test_declval();
}
