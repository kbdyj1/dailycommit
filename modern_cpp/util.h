#ifndef UTIL_H
#define UTIL_H

#include <iterator>
#include <iostream>
#include <chrono>
#include <functional>

template <typename C>
void print(const C& c)
{
    std::copy(std::begin(c), std::end(c), std::ostream_iterator<typename C::value_type>(std::cout, " "));
    std::cout << "\n";
}

#define STRINGFY2(x)    #x
#define STRINGFY(x)     STRINGFY2(x)
#define CONNECT2(x, y)  x##y
#define CONNECT(x, y)   CONNECT2(x, y)

#define PRINT_FUNC(x)   std::cout << STRINGFY(x) << ": " << x << "\n"

template <typename Time = std::chrono::milliseconds, typename Clock = std::chrono::high_resolution_clock>
struct PerfTimer {
    template <typename F, typename... Args>
    static Time duration(F&& func, Args... args) {
        auto start = Clock::now();
        std::invoke(std::forward<F>(func), std::forward<Args...>(args...));
        auto end = Clock::now();

        return std::chrono::duration_cast<Time>(end - start);
    }
};

#endif // UTIL_H
