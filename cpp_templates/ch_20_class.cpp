#include <iostream>
#include <vector>
#include <utility>
#include <map>

#include "EnableIf.hpp"
#include "IsConvertible.hpp"
#include "IsContextualBool.hpp"

namespace { //=================================================================

namespace _1 { //----------------------------------------------------

template <typename Key, typename Value, typename = void>
class Dictionary
{
    std::vector<std::pair<const Key, Value>> data;

public:
    Value& operator[](const Key& key) {
        for (auto& elem : data) {
            if (elem.first == key) {
                return elem.second;
            }
        }
        data.push_back(std::pair<const Key, Value>(key, Value()));

        return data.back().second;
    }
};

template <typename T, typename = std::void_t<>>
struct HasLess : std::false_type
{
};

template <typename T>
struct HasLess<T, std::void_t<decltype(std::declval<T>() < std::declval<T>())>> : std::true_type
{
    using Type = T;
};

#if (0) //TODO
template <typename Key, typename Value>
class Dictionary<Key, Value, std::enable_if_t<HasLess<Key>, Key>>
{
    std::map<Key, Value> data;

public:
    Value& operator[](const Key& key) {
        return data[key];
    }
};
#endif

void test()
{
    auto dic = Dictionary<int, std::string>();
    dic[0] = "Hello";
    dic[1] = "Qt";

    std::cout << dic[0] << ", " << dic[1] << "!\n";
}

} //_1 --------------------------------------------------------------

namespace _2 { //tag dispatch

template <typename... Types>
struct MatchOverloads;

template <>
struct MatchOverloads<>{
    static void match(...);
};

template <typename T, typename... Args>
struct MatchOverloads<T, Args...> : public MatchOverloads<Args...> {
    static T match(T);
    using MatchOverloads<Args...>::match;
};

template <typename T, typename... Args>
struct BestMatchT {
    using Type = decltype(MatchOverloads<Args...>::match(std::declval<T>()));
};

template <typename T, typename... Args>
using BestMatch = typename BestMatchT<T, Args...>::Type;

template <typename Iter, typename Tag = BestMatch<typename std::iterator_traits<Iter>::iterator_category(),
                                                  std::input_iterator_tag,
                                                  std::bidirectional_iterator_tag,
                                                  std::random_access_iterator_tag>>
class Advance;

template <typename Iter>
class Advance<Iter, std::input_iterator_tag> {
public:
    using DifferenceType = typename std::iterator_traits<Iter>::difference_type;
    void operator()(Iter& iter, DifferenceType n) const {
        std::cout << "input\n";
        while (0 < n) {
            --n;
            ++iter;
        }
    }
};

template <typename Iter>
class Advance<Iter, std::bidirectional_iterator_tag> {
public:
    using DifferenceType = typename std::iterator_traits<Iter>::difference_type;
    void operator()(Iter& iter, DifferenceType n) const {
        std::cout << "bidirectional\n";
        if (0 < n) {
            for ( ; 0 < n; --n)
                ++iter;
        } else {
            for ( ; n < 0; ++n)
                --iter;
        }
    }
};

template <typename Iter>
class Advance<Iter, std::random_access_iterator_tag> {
public:
    using DifferenceType = typename std::iterator_traits<Iter>::difference_type;
    void operator()(Iter& iter, DifferenceType n) const {
        std::cout << "random_access\n";
        iter += n;
    }
};

void test()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto iter = v.begin();
    using Iter = decltype(iter);
#if (0) //TODO
    auto adv = Advance<Iter>();
#endif
}

} //_2 --------------------------------------------------------------

namespace _3 { //instantiation free

template <typename T0, typename T1>
class HasLess {
    template <typename T>
    struct Identity;

    template<typename U0, typename U1>
    static std::true_type test(Identity<decltype(std::declval<U0>() < std::declval<U1>())>*);

    template<typename U0, typename U1>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T0, T1>(nullptr))::value;
};

template <typename T0, typename T1, bool HasLess>
class LessResultImpl {
public:
    using Type = decltype (std::declval<T0>() < std::declval<T1>());
};

template <typename T0, typename T1>
class LessResultImpl<T0, T1, false>
{};

template <typename T0, typename T1>
class LessResultT : public LessResultImpl<T0, T1, HasLess<T0, T1>::value>
{};

template <typename T0, typename T1>
using LessResult = typename LessResultT<T0, T1>::Type;

#define ENABLE_CONTEXTUAL_BOOL

template <typename T>
#if defined (ENABLE_CONTEXTUAL_BOOL)
EnableIf<IsContextualBool<LessResult<const T&, const T&>>, const T&>
#else
EnableIf<IsConvertable<LessResult<const T&, const T&>, bool>::value, const T&>
#endif
min(const T& l, const T& r)
{
    if (l < r)
        return l;
    else
        return r;
}

struct A
{};
bool operator<(const A&, const A&) { return true; }

struct BoolLike {
    explicit operator bool() const { return true; }
};
struct B
{};
BoolLike operator<(const B&, const B&) { return BoolLike{}; }

void test()
{
    auto a0 = A{};
    auto a1 = A{};
    auto amin = min(a0, a1);

    auto i0 = 0;
    auto i1 = 1;
    auto imin = min(i0, i1);
    std::cout << "min(0, 1): " << imin << "\n";

#if defined(ENABLE_CONTEXTUAL_BOOL)
    auto b0 = B{};
    auto b1 = B{};
    auto bmin = min(b0, b1);
#endif
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_20_class()
{
#if (0) //done
    _1::test();
#endif

    _3::test();
}
