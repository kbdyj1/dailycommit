#include <iostream>
#include <mutex>

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
    mutable std::once_flag mutex;
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
        std::call_once(mutex, [this]{
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

}

} //===========================================================================

void test_ch_06()
{

}
