#include <iostream>
#include <vector>
#include <list>
#include <typeinfo>
#include <type_traits>

namespace { //=================================================================

namespace _1 {

template <typename T>
struct TypeSize {
    static const std::size_t value = sizeof(T);
};

template <typename T>
inline constexpr std::size_t TypeSizeV = TypeSize<T>::value;

void test()
{
    std::cout << "TypeSize<long>::value: " << TypeSize<long>::value << "\n";
    std::cout << "TypeSizeV<long>: " << TypeSizeV<long> << "\n";
}
} //_1 --------------------------------------------------------------

namespace _2 { //element's type

template <typename T>
struct Element;

#if (0)
template <typename T>
struct Element<std::vector<T>> {
    using Type = T;
};

template <typename T>
struct Element<std::list<T>> {
    using Type = T;
};
#else
template <typename C>
struct Element {
    using Type = typename C::value_type;
};
#endif

template <typename T, std::size_t N>
struct Element<T[N]> {
    using Type = T;
};

template <typename T>
struct Element<T[]> {
    using Type = T;
};

template <typename T>
void printElementType(const T& t)
{
    std::cout << typeid(typename Element<T>::Type).name() << "\n";
}

template <typename T>
using ElementType = typename Element<T>::Type;

template <typename C>
typename Element<C>::Type sum(const C& container) {
    ElementType<C> total = ElementType<C>{};

    return total;
}

void test()
{
    auto v = std::vector<bool>{};
    printElementType(v);

    int array[32];
    printElementType(array);
}

} //_2 --------------------------------------------------------------

namespace _3 { //transform traits

template <typename T>
struct RemoveReference {
    using Type = T;
};

template <typename T>
struct RemoveReference<T&> {
    using Type = T;
};

template <typename T>
struct RemoveReference<T&&> {
    using Type = T;
};

template <typename T>
using RemoveReferenceT = typename RemoveReference<T>::Type;

template <typename T>
struct AddLValueReference {
    using Type = T&;
};

template <typename T>
using AddLValueReferenceT = typename AddLValueReference<T>::Type;

template <typename T>
struct AddRValueReference {
    using Type = T&&;
};

template <typename T>
using AddRValueReferenceT = typename AddRValueReference<T>::Type;

template <>
struct AddLValueReference<void> {
    using Type = void;
};

template <>
struct AddLValueReference<const void> {
    using Type = const void;
};

template <>
struct AddLValueReference<volatile void> {
    using Type = volatile void;
};

template <>
struct AddLValueReference<const volatile void> {
    using Type = const volatile void;
};

// remove const
template <typename T>
struct RemoveConst {
    using Type = T;
};

template <typename T>
struct RemoveConst<const T> {
    using Type = T;
};

template <typename T>
using RemoveConstT = typename RemoveConst<T>::Type;

} //_3 --------------------------------------------------------------

namespace _4 {

template <typename T>
void f(T)
{}

template <typename A>
void printParameterType(void(*)(A)) {
    std::cout << "parameter type: " << typeid(A).name() << "\n";
    std::cout << "is int? " << std::is_same_v<A, int> << "\n";
    std::cout << "is const? " << std::is_const_v<A> << "\n";
    std::cout << "is volatile? " << std::is_volatile_v<A> << "\n";
    std::cout << "is pointer? " << std::is_pointer_v<A> << "\n";
}

void test()
{
    printParameterType(&f<int>);
    printParameterType(&f<const int*>);
    printParameterType(&f<int[7]>);
    printParameterType(&f<int(int)>);
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <bool b>
struct Bool {
    using Type = Bool<b>;
    static constexpr bool value = b;
};

using TrueType = Bool<true>;
using FalseType = Bool<false>;

template <typename T0, typename T1>
struct IsSame : FalseType {
};

template <typename T>
struct IsSame<T, T> : TrueType {
};

template <typename T0, typename T1>
constexpr bool isSame = IsSame<T0, T1>::value;



} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_19_type_function()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _4::test();
}
