#include <type_traits>
#include <iostream>

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

} // namespace ================================================================

void test_typeTraits()
{
    std::cout << std::boolalpha << std::endl;

    //test0();
    //test1();
    test2();
}
