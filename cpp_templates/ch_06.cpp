//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.06 Move semantics
//
//  22/09/25
//=============================================================================

#include <iostream>
#include <utility>
#include <string>

using namespace std::string_literals;

namespace { //=================================================================

#define USE_FORWARD_REFERENCE

template <typename T>
using IsConvitableToString = std::enable_if_t<std::is_convertible_v<T, std::string>>;

class Person {
    std::string name;

public:
#if defined(USE_FORWARD_REFERENCE)
    template <typename T, typename = IsConvitableToString<T>>
    explicit Person(T&& s) : name(std::forward<T>(s))
    {
        std::cout << "Person(T&&)\n";
    }
#else
    explicit Person(const std::string& s) : name(s)
    {
        std::cout << "Person(const std::string&)\n";
    }
    explicit Person(std::string&& s) : name(std::move(s))
    {
        std::cout << "Person(const std::string&&)\n";
    }
#endif

    Person(const Person& other) : name(other.name)
    {
        std::cout << "Person(const Persong&)\n";
    }
    Person(Person&& other) : name(std::move(other.name))
    {
        std::cout << "Person(Persong&&)\n";
    }
};

void test_person()
{
    auto s = "Hello, Qt"s;

    auto p0 = Person{s};
    auto p1 = Person{"tmp"};
    auto p2 = Person{p0};
    auto p3 = Person{std::move(p0)};
}

class B {
public:
    B()
    {
        std::cout << "B()\n";
    }
    B(const volatile B&) = delete;

    template <typename T, typename = std::enable_if_t<!std::is_integral_v<T>>>
    B(const T&) {
        std::cout << "B(const T&) T = " << typeid(T).name() << "\n";
    }
};

void test_specital_member_function()
{
    auto b0 = B{};
    auto b1 = B{b0};
    auto s = "Hello, Qt"s;
    auto b2 = B{s};

#if (0)
    auto i = 0U;
    auto b3 = B{i};
#endif
}

} //namespace =================================================================

void test_ch_06_perfect_forwarding();
void test_ch_06_enable_if();

void test_ch_06()
{
#if (0)
    test_ch_06_perfect_forwarding();
    test_ch_06_enable_if();
    test_person();
#endif

    test_specital_member_function();
}
