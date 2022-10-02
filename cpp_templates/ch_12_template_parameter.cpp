#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <array>

namespace { //=================================================================

template <typename, int>
class A {};

void test_unnamed_parameter()
{
    auto a = A<int, 8>{};
}

template <typename T, typename T::Allocator* Allocator>
class List
{};

struct B {
    static struct Allocator {
    } allocator;
};

List<B, &B::allocator> bList;

template <int& Counter>
struct C {
    C() { Counter = Counter + 1; }
    ~C() { Counter = Counter - 1; }
};

int count0 = int{};
int count1 = int{};

void test_reference_parameter()
{
    auto c0 = C<count0>{};
    auto c1 = C<count0>{};
    auto c2 = C<count1>{};

    std::cout << "count0: " << count0 << "\n";
    std::cout << "count1: " << count1 << "\n";
}

struct MyAllocator {
};

template <template <typename T, typename A = MyAllocator> class Container>
class Adaptor {
    Container<int> storage;
};

template <typename... Types>
class Tuple;

template <typename T, unsigned... Dimensions>
class MultiArray;

using TransformMatrix = MultiArray<double, 3, 3>;

template <typename T, template<typename, typename> class ... Containers>
void test_containers()
{

}

void test()
{
    test_containers<int, std::vector, std::list>();
}

template <unsigned...>
struct Tensor
{};

template <unsigned... Dim0, unsigned... Dim1>
auto compose(Tensor<Dim0...>, Tensor<Dim1>...)
{
    std::cout << "compose(" << sizeof...(Dim0) << ", " << sizeof...(Dim1) << ")\n";
    return 0;
}

//void test_compose()
//{
//    auto ret = compose(Tensor<1, 2>{}, Tensor<3, 4>{});
//}

template <typename...>
struct TypeList;

template <typename T, typename U>
struct Zip;

template <typename... Xs, typename... Ys>
struct Zip<TypeList<Xs...>, TypeList<Ys...>>
{};

template <typename... Ts>
struct Args {
    template<Ts... args>
    struct Vals
    {};
};

void test_overlapped_parameter_pack()
{
    auto vals = Args<int, char, char>::Vals<3, 'x', 'y'>{};
}

template <typename T0, typename T1, typename T2, typename T3 = int>
class D;

template <typename T0, typename T1, typename T2 = char, typename T3>
class D;

#if (0) //error T1 is empty
template <typename T0 = int, typename T1, typename T2, typename T3>
class D;
#endif

template <typename R = void, typename T>
R* addressOf(T& value);

template <typename T>
class E;

template <typename T>
class E<T*>;

template <typename T>
T max(T a, T b)
{
    return (a < b) ? b : a;
}

void test_max()
{
    std::cout << max<double>(1.5, 2.5) << "\n";
    std::cout << max(1.5, 2.5) << "\n";
    std::cout << max<int>(1.5, 2.5) << "\n";
}

template <typename Ret, typename T>
Ret implicitCast(const T& src)
{
    return src;
}

void test_implicitCast()
{
    std::cout << implicitCast<int>(3.4) << "\n";
}

template <typename Func, typename T>
void apply(Func func, T arg)
{
    func(arg);
}

template <typename T>
void single(T t)
{
    std::cout << "single(" << t << ")\n";
}

template <typename T>
void multi(T t)
{
    std::cout << "multi(" << t << ")\n";
}

template <typename T>
void multi(T* t)
{
    std::cout << "multi(*" << *t << ")\n";
}

void test_apply()
{
    apply(&single<int>, 3);
#if (0) //overloaded function
    apply(&multi<int>, 3);
#endif
}

namespace overload {
template <typename T>
auto test(const typename T::X& param) -> decltype(param)
{
    std::cout << "test(const typename T::X& param)\n";
    return param;
}
template <typename T>
auto test(...)
{
    std::cout << "test(...)\n";
}
} //overload

struct F {
    struct X {
        int value;
    } internal;
};

void test_overload_function_template()
{
    overload::test<int>(1, 2);

    auto f = F{};
    overload::test<F>(f.internal);
}

template <typename T, T param>
class G
{};

void gfunc()
{}
void gfunc(int)
{}

template <typename T>
void gtempfunc()
{}

struct X {
    static bool flag;
    int iValue;
    constexpr operator int() const {
        return 62;
    }
};

int iValue = 16;
void test_non_type_parameter()
{
    auto g0 = G<int, 8>{};

    auto g1 = G<int*, &iValue>{};

    auto g2 = G<void(*)(int), gfunc>{};

    auto g3 = G<void(), &gtempfunc<int>>{};

    auto g4 = G<bool&, X::flag>{};

    auto g5 = G<int X::*, &X::iValue>{};

    auto g6 = G<long, X{}>{};
}

template <typename T0, typename T1, template<typename...> class Container>
class Rel
{};

template <template <typename...> class TT>
class AlmostAnyImpl
{};

void test_template_template_parameter()
{
    auto rel = Rel<int, double, std::list>{};

    auto v = AlmostAnyImpl<std::vector>{};
    auto m = AlmostAnyImpl<std::map>{};
#if (0)
    auto a = AlmostAnyImpl<std::array>{};   //error: template template argument has different template parameters than its corresponding template template parameter
#endif
}

template <typename T, int I>
class Mix
{};

using Int = int;

void test_equality()
{
    auto m0 = Mix<Int, 3*3>{};
    auto m1 = Mix<int, 3+3>{};

    std::cout << "std::is_same_v<Mix<Int, 3*3>, Mix<int, 3+3>>: " << std::is_same_v<Mix<Int, 3*3>, Mix<int, 3+3>> << "\n";
}

template <typename... Types>
class Tuple
{};

template <typename... Types>
class MyTuple : public Tuple<Types...>
{};

template <typename... Types>
class PtrTuple : public Tuple<Types*...>
{};

void test_parameter_pack()
{
    auto t0 = MyTuple<int, float, double>{};
}

template <typename... Mixins>
class Point : public Mixins... {
    double x, y, z;

public:
    Point() : Mixins()...
    {}
    Point(Mixins... mixins) : Mixins(mixins)...
    {}

    template<typename Visitor>
    void visit(Visitor visitor) {
        visitor(static_cast<Mixins&>(*this)...);
    }
};

struct Color {
    unsigned char r, g, b;
};
struct Label {
    std::string name;
};

void test_mixin()
{
    auto p = Point<Color, Label>{};

    p.r = 255;
    p.g = 255;
    p.g = 255;
    p.name = "white";

    auto p1 = Point<Color, Label>{{255, 255, 0}, {"white"}};
}

template <typename... Ts>
struct Values {
    template<Ts... Vs>
    struct Holder {
    };
};

void test_holder()
{
    auto v = Values<int, char, int*>::Holder<1, 'a', &count0>{};
}

} //namespace =================================================================

void test_ch_12_template_parameter()
{
    std::cout << std::boolalpha;

#if (0) //done
    test_unnamed_parameter();
    test_reference_parameter();
    test_max();
    test_implicitCast();
    test_overload_function_template();
#endif

    test_equality();
}
