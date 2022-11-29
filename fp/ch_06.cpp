#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>
#include <map>

namespace { //=================================================================

namespace _1 {

#define USE_ONCE_FLAG

template <typename F>
class LazyEval {
    F expression;

    mutable decltype(expression()) cachedValue;
#if !defined(USE_ONCE_FLAG)
    mutable bool cached = false;
    mutable std::mutex mutex;
#else
    mutable std::once_flag once;
#endif

public:
    LazyEval(F exp) : expression(exp)
    {}

    operator const decltype(expression())& () const {

#if !defined(USE_ONCE_FLAG)
        auto lock = std::unique_lock(mutex);

        if (!cached) {
            cachedValue = expression();
            cached = true;
        }
#else
        //LIBS += -pthread
        std::call_once(once, [this]{
            cachedValue = expression();
        });
#endif

        return cachedValue;
    }
};

template <typename F>
inline LazyEval<F> makeLazyEval(F&& exp)
{
    return LazyEval<F>(std::forward<F>(exp));
}

void test()
{
    auto eval = makeLazyEval([]{
        return 2;
    });

    std::cout << "eval(): " << eval << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

unsigned int fib0(unsigned int n)
{
#if (0)
    return (n == 0) ? 0 :
           (n == 1) ? 1 :
                    fib0(n-1) + fib0(n-2);
#else
    return n < 2 ? n : fib0(n-1) + fib0(n-2);
#endif
}

std::vector<unsigned int> cache{0, 1};

unsigned int fib1(unsigned int n)
{
    if (cache.size() > n) {
        return cache[n];
    } else {
        const auto result = fib1(n-1) + fib1(n-2);
        cache.push_back(result);
        return result;
    }
}

class FibCache {
    unsigned int first;
    unsigned int second;
    std::size_t length;

public:
    FibCache() : first(0)
               , second(1)
               , length(2)
    {}

    std::size_t size() const {
        return length;
    }
    unsigned int operator[](unsigned int n) const {
        return n == length - 1 ? second :
               n == length - 2 ? first :
               0;
    }
    void push_back(unsigned int value)
    {
        length++;
        first = second;
        second = value;
    }
};

FibCache fibCache;

unsigned int fib2(unsigned int n)
{
    if (fibCache.size() > n) {
        return fibCache[n];
    } else {
        const auto result = fib1(n-1) + fib1(n-2);
        fibCache.push_back(result);
        return result;
    }
}

template <typename Result, typename... Args>
auto make_memoized(Result(*func)(Args...))
{
    std::map<std::tuple<Args...>, Result> cache;

    return [func, cache](Args... args) mutable -> Result {
        const auto tuple = std::make_tuple(args...);
        const auto cached = cache.find(tuple);
        if (cache.end() == cached) {
            auto result = func(args...);
            cache[tuple] = result;
            return result;
        } else {
            return cached->second;
        }
    };
}

unsigned int fib3(unsigned int n)
{
    if (n < 2) {
        return n;
    } else {
        return fib3(n-1) + fib3(n-2);
    }
}

void test()
{
    constexpr unsigned int K = 20;
    std::cout << "fib0(" << K << "): " << fib0(K) << "\n";
    std::cout << "fib1(" << K << "): " << fib1(K) << "\n";
    std::cout << "fib2(" << K << "): " << fib2(K) << "\n";

    auto memoizedFib = make_memoized(fib3);
    std::cout << "fib3(" << K << "): " << memoizedFib(K) << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

std::string a = "Hello, Qt";
std::string b = "Helme, Qt6.4";
unsigned int lev(unsigned int m, unsigned int n)
{
    return m == 0 ? n :
           n == 0 ? m :
                    std::min({lev(m-1, n) + 1, lev(m, n-1) + 1, lev(m-1, n-1) + (a[m-1] != b[n-1])});
}

void test()
{
    auto m = a.length();
    auto n = b.length();
    std::cout << "lev(m, n): " << lev(m, n) << "\n";
}

} //_3 --------------------------------------------------------------

namespace _4 {



} //_4 --------------------------------------------------------------

} //===========================================================================

void test_ch_06()
{
#if (0) //done
    _1::test();
    _2::test();
#endif
    _2::test();
}
