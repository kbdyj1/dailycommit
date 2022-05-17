#include <iostream>
#include <memory>

namespace { //=================================================================

// ES.49 If you must use a cast, use a named cast

// static_cast
// const_cast
// reinterpret_cast
// dynamic_cast
// std::move
// std::forward
// gsl::narrow_cast (static_cast)
// gsl::narrow

class A {};
class B {};

void test_c_49(A* pa)
{
#if (0)
    B* pb0 = pa;
    B* pb2 = static_cast<B*>(pa);
    B* pb4 = dynamic_cast<B*>(pa);
#endif
    B* pb1 = (B*)pa;
    B* pb3 = reinterpret_cast<B*>(pa);

    float f0 = 1.0f;
    double d0{f0};

#if (0)
    int64_t i0 = 1;
    int32_t i1{i0};
#endif
}

// ES.50 Don't cast away const

void f(const int& x)
{
    const_cast<int&>(x) = 6;
}

class C {};

class D
{
    C mC;
public:

#if (0)
    C& get() { return mC; }
#else
    C& get() { return const_cast<C&>(static_cast<const D&>(*this).get()); }
#endif
    const C& get() const { return mC; }
};

class E
{
    C mC;
    template<typename T>
    static auto& getImpl(T& t)
    {
        return t.mC;
    }
public:
    C& get()
    {
        std::cout << "C& get()" << std::endl;
        return getImpl(*this);
    }
    const C& get() const
    {
        std::cout << "const C& get() const" << std::endl;
        return getImpl(*this);
    }
};

int compute(int x)
{
    return x + 1;
}

class Cache
{
public:
    std::pair<bool, int> find(int x) const
    {
        return std::pair<bool, int>{true, x};
    }
    void set(int x, int v)
    {
    }
};

//#define USE_MUTABLE

class X
{
#ifdef USE_MUTABLE
    mutable Cache mCache;
#else
    Cache mCache;
#endif

public:
    int get(int x) const
    {
        auto p = mCache.find(x);
        if (p.first)
            return p.second;

        int value = compute(x);
#ifdef USE_MUTABLE
        mCache.set(x, value);
#else
        const_cast<Cache&>(mCache).set(x, value);
#endif
        return value;
    }
};

class Y
{
    std::unique_ptr<Cache> mCache;

public:
    Y() : mCache(new Cache)
    {
    }
   ~Y() = default;

    int get(int x) const
    {
        auto p = mCache->find(x);
        if (p.first)
            return p.second;

        int value = compute(x);
        mCache->set(x, value);
        return value;
    }
};

// ES.55 Avoid the need for range checking

// ES.56 Write std::move() only when you need to explicit move an object to another scope

class F
{
};

void sink(F&&)
{}
void sink(std::unique_ptr<F>)
{}

} // namespace ================================================================

void test_es_50()
{
    int i = 0;
    const int c = 0;

    f(i); // side effect
    f(c); // undefined behavior

    E e0;
    const E e1;

    auto c0 = e0.get();
    auto c1 = e1.get();
}

void test_es_56()
{
    F f;
#if (0)
    sink(f);
#else
    sink(std::move(f));
#endif

    std::string s0 = "Hello, Qt6";
    std::string s1 = s0;
    std::cout << "s0: " << s0 << ", s1: " << s1 << std::endl;

    std::string s2 = std::move(s0);
    std::cout << "s0: " << s0 << ", s2: " << s2 << std::endl;

    auto p = std::make_unique<F>();
#if (0)
    sink(p); // disable copy
#else
    sink(std::move(p));
#endif
}
