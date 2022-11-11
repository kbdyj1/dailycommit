//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.28 Debugging
//
//  22/11/11
//=============================================================================

#include <iostream>
#include <utility>
#include <type_traits>

namespace { //=================================================================

template <typename T>
void clear(T& t)
{
    *t = 0;
}

template <typename T>
void core(T& p)
{
    clear(p);
}

template <typename T>
void middle(typename T::Index p)
{
    core(p);
}

namespace _1 { //----------------------------------------------------

class Client
{
public:
#if (1)
    using Index = int*;
#else
    using Index = int;
#endif
};

template <typename T>
void shell(const T& env)
{
    typename T::Index i;

    middle<T>(i);
}

void test()
{
    auto c = Client{};

    shell(c);
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
void ignore(const T&)
{}

template <typename T>
void shell(const T& env)
{
    class ShallowCheck {
        void deref(typename T::Index ptr) {
            ignore(*ptr);
        }
    };
    typename T::Index i;
    middle<T>(i);
}

class Client {
public:
#if (1)
    using Index = int*;
#else
    using Index = int;
#endif
};

void test()
{
    auto c = Client{};
    shell(c);
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T>
class HasDereference {
    template<typename U>
    struct Identity;

    template<typename U>
    static std::true_type test(Identity<decltype(*std::declval<U>())>*);

    template<typename U>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template <typename T>
void shell(const T& env)
{
    static_assert (HasDereference<T>::value, "T is not dereferenceable");

    typename T::Index i;
    middle(i);
}

} //_3 --------------------------------------------------------------

namespace _4 {

class EqualityComparableArcheType
{};

class ConvertibleToBoolArcheType
{
public:
    operator bool() const;
    bool operator!() = delete;
};
ConvertibleToBoolArcheType operator==(const EqualityComparableArcheType&, const EqualityComparableArcheType&);

template <typename T>
int find(const T* array, int n, const T& value)
{
    int i = 0;
    while (i != n && !(array[i] == value))
        ++i;
    return i;
}

} //_4 --------------------------------------------------------------

namespace _5 {

class SortTracer {
    int value;
    int generation;
    inline static long created = 0;
    inline static long destroyed = 0;
    inline static long assigned = 0;
    inline static long compared = 0;
    inline static long maxLive = 0;

    static void updateMaxLive() {
        if (maxLive < created - destroyed) {
            maxLive = created - destroyed;
        }
    }

public:
    static long creations() {
        return created;
    }
    static long destructions() {
        return destroyed;
    }
    static long assignments() {
        return assigned;
    }
    static long comparisions() {
        return compared;
    }
    static long maxLiveCnt() {
        return maxLive;
    }

    SortTracer(int v = 0) : value(v), generation(1)
    {
        ++created;
        updateMaxLive();
    }
    SortTracer(const SortTracer& other) : value(other.value), generation(other.generation+1)
    {
        ++created;
        updateMaxLive();
    }
    ~SortTracer()
    {
        ++destroyed;
        updateMaxLive();
    }
    SortTracer& operator=(const SortTracer& other)
    {
        if (this != &other) {
            ++assigned;
            value = other.value;
        }
        return *this;
    }
    friend bool operator<(const SortTracer& l, const SortTracer& r)
    {
        ++compared;
        return l.value < r.value;
    }
    int getValue() const
    {
        return value;
    }
};

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_28()
{

}
