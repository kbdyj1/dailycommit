#include <iostream>
#include <iterator>
#include <cassert>
#include <vector>
#include <utility>

using namespace std::literals;

namespace { //=================================================================

// T.120 Use template metaprogramming only when you really need to
//  if result is a value, rather than a type, use a constexpr function

// T.121 Use template metaprogramming primarily to emulate concepts

#define USING_CONCEPTS

#if defined(USING_CONCEPTS)
void advance(std::random_access_iterator auto p, int n)
#else
template <typename Iter>
std::enable_if<std::random_access_iterator<Iter>, void>
advance(Iter p, int n)
#endif
{
    p += n;
}

#if defined(USING_CONCEPTS)
void advance(std::forward_iterator auto p, int n)
#else
template <typename Iter>
std::enable_if<std::forward_iterator<Iter>, void>
advance(Iter p, int n)
#endif
{
    assert(n >= 0);
    while (n--)
        p++;
}

// T.122 Use templates (usally template aliases) to compute types at compile time
//  "Traits" techniques are mostly replaced by template aliases to compute types and
//  constexpr functions to compute values

// T.123 Use constexpr functions to compute values at compile time

#define USE_TEMPLATE

#ifndef USE_TEMPLATE
template <typename T>
constexpr T pow(T v, int n)
{
    T res = 1;
    while (n--)
        res *= v;
    return res;
}
#else
template <typename T, int v, int n>
struct pow {
    static T const value = pow<T, v, n-1>::value * v;
};

template <typename T, int v>
struct pow<T, v, 0> {
    static const T value = 1;
};
#endif

// T.124 Prefer to use standard-library TMP facilities

// T.125 If you need to go beyond the standard-library TMP facilities, use an existing library

// T.140 Name all operations with potential for reuse

struct Rec {
    std::string name;
    std::string addr;
    int id;
};
bool same(const Rec& a, const Rec& b)
{
    return a.id == b.id;
}
bool compare_insensitive(const std::string& s0, const std::string& s1)
{
    if (s0.size() != s1.size())
        return false;
    for (int i=0; i<s0.size(); i++) {
        if (std::tolower(s0[i]) != std::tolower(s1[i]))
            return false;
    }
    return true;
}

// T.141 Use an unnamed lambda if you need a simple function object in one place only

// T.142 Use template variables to simplify notation

// T.143 Don't write unintentionally non-generic code

// T.144 Don't specialize function templates
//  flag all specialization of a function template. Overload instead

// T.150 Check that a class matches a concept using static_assert
class X {
public:
    X() = default;
    X(const X&) = default;
    X(X&&) = default;
    X& operator=(const X&) = default;
};

} //===========================================================================
void test_t_123()
{
#ifndef USE_TEMPLATE
    std::cout << "pow(3, 4) : " << pow(3, 4) << std::endl;
#else
    std::cout << "pow<int, 3, 4>::value : " << pow<int, 3, 4>::value << std::endl;
#endif
}

void test_t_140()
{
    auto v = std::vector<Rec*>();
    v.push_back(new Rec{"aaa", "aaa-addr", 0});
    v.push_back(new Rec{"bbb", "bbb-addr", 1});
    v.push_back(new Rec{"ccc", "ccc-addr", 2});

    auto target = "BBB"s;

#if (0)
    auto x = std::find_if(std::begin(v), std::end(v), [&](Rec* r){
        if (r->name.size() != target.size())
            return false;
        for (auto i=0; i<r->name.size(); i++) {
            if (std::tolower(r->name[i]) != std::tolower(target[i]))
                return false;
            return true;
        }
    });
#else
    // for reuse
    auto cmp_to_target = [&target](const std::string& s) {
        return compare_insensitive(target, s);
    };
    auto x = std::find_if(std::begin(v), std::end(v), [&](Rec* r){
        return cmp_to_target(r->name);
    });
#endif
    if (x != v.end()) {
        std::cout << "found : " << (*x)->id << ", name: " << (*x)->name << std::endl;
    }
}

void test_t_150()
{
    static_assert(std::is_default_constructible_v<X>);
    static_assert(std::is_copy_assignable_v<X>);
}
