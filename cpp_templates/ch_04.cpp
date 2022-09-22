//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.04 Variadic template
//
//  22/09/21
//=============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_set>

using namespace std::string_literals;

namespace { //=================================================================

void print()
{
    std::cout << "\n";
}

template <typename T, typename... Types>
void print(T arg, Types... args)
{
    std::cout << arg << " ";

    if (0 == sizeof...(args)) {
        std::cout << "...";
    }
    print(args...);
}

void test_print()
{
    print("Hello", "Qt"s, 6, 4.0);
}

template <typename... T>
auto sum(T... value)
{
    return (... + value);   // (((0 + 1) + 2) + ...)
}

void test_sum()
{
    std::cout << sum(1, 2, 3, 4, 5) << "\n";
}

template <typename T>
class AddSpace
{
    const T& ref;
public:
    AddSpace(const T& r) : ref(r)
    {}
    friend std::ostream& operator<<(std::ostream& os, AddSpace<T> s) {
        return os << s.ref << " ";
    }
};

template <typename... T>
void print2(const T&... args)
{
    (std::cout << ... << AddSpace(args));
}

void test_fold_print()
{
    print2("Hello,", "Qt"s, 6, 2.0);
}

template <typename... T>
void printDoubled(const T&... args)
{
    print(args + args...);
}

void test_print_doubled()
{
    printDoubled(4, 8.5, "Hello,"s);
}

template <typename T, typename... TN>
constexpr bool isSame(T, TN...)
{
    return (std::is_same<T, TN>::value && ...);
}

void test_is_same()
{
    std::cout << "isSame(1, 4, 65535) ? " << isSame(1, 4, 65535) << "\n";
    std::cout << "isSame(1, 4u, 65535) ? " << isSame(1, 4u, 65535) << "\n";
}

template <typename C, typename... Idx>
void printElems(const C& container, Idx... idx)
{
    print(container[idx]...);
}

template <size_t... Idx, typename C>
void printIdx(const C& container)
{
    print(container[Idx]...);
}

void test_variadic_index()
{
    auto v = std::vector<int>{ 0, 1, 2, 3, 4, 5 };

    printElems(v, 5, 3, 1);

    printIdx<0, 2, 4>(v);
}

template <size_t...>
struct Indices
{};

template <typename T, size_t... Index>
void printByIndex(T t, Indices<Index...>)
{
    print(std::get<Index>(t)...);
}

void test_class_template()
{
    auto array = std::array<std::string, 5>{ "Hello", "Qt", "6.2", "gui", "qml" };
    printByIndex(array, Indices<0, 2, 4>{});

    auto t = std::make_tuple(12, "Qt", 2.0);
    printByIndex(t, Indices<0, 1, 2>{});
}

namespace varusing {

class Customer {
    std::string name;

public:
    Customer(const std::string& name) : name(name)
    {}
    std::string getName() const {
        return name;
    }
};

struct CustomerEqal {
    bool operator()(const Customer& c0, const Customer& c1) const {
        return c0.getName() == c1.getName();
    }
};

struct CustomerHash {
    size_t operator()(const Customer& c) const {
        return std::hash<std::string>()(c.getName());
    }
};

template <typename... Bases>
struct Overloader : Bases...
{
    using Bases::operator()...;
};

void test()
{
    using CustomerOp = Overloader<CustomerEqal, CustomerHash>;
}

} //varusing ------------------------------------------------------------------

} //namespace =================================================================

void test_ch_04()
{
    std::cout << std::boolalpha;

#if (0)
    test_print();
    test_sum();
    test_fold_print();
    test_print_doubled();
    test_is_same();
    test_variadic_index();
#endif

    test_class_template();
}
