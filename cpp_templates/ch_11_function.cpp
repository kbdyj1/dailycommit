#include <iostream>
#include <vector>
#include <functional>

namespace { //=================================================================

#if 201703L <= __cplusplus
#   define USE_INVOKE
#endif

#if !defined(USE_INVOKE)
template <typename Iter, typename Pred>
void foreach(Iter current, Iter end, Pred pred)
{
    while (current != end) {
        pred(*current);
        current++;
    }
}
#else
template <typename Iter, typename Pred, typename... Args>
void foreach(Iter current, Iter end, Pred pred, const Args&... args)
{
    while (current != end) {
        std::invoke(pred, args..., *current);
        ++current;
    }
}
#endif

void func(int value)
{
    std::cout << "(" << value << ") ";
}

struct Functor {
    void operator()(int value) const {
        std::cout << "[" << value << "] ";
    }
    void print(int value) {
        std::cout << "{" << value << "} ";
    }
};

void test_foreach()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    constexpr int method = 4;

    if constexpr(method == 0) {
        std::cout << "use lambda.\n";
        foreach(std::begin(v), std::end(v), [](int value){
            std::cout << value << " ";
        });
    } else if constexpr(method == 1) {
        std::cout << "use function.\n";
        foreach(std::begin(v), std::end(v), func);
    } else if constexpr(method == 2) {
        std::cout << "use Functor.\n";
        foreach(std::begin(v), std::end(v), Functor{});
    }
#if defined(USE_INVOKE)
    else if constexpr(method == 3) {
        std::cout << "call Functor's member.\n";
        auto obj = Functor{};
        foreach(std::begin(v), std::end(v), &Functor::print, &obj);
    } else if constexpr (method == 4) {
        std::cout << "use lambda #2.\n";
        foreach(std::begin(v), std::end(v), [](const std::string& prefix, int value){
            std::cout << prefix << value << " ";
        }, "...");
    }
#endif
    std::cout << std::endl;
}

#if 201103L < __cplusplus //c++14 decltype(auto) -> variant type, return type, template argument type
template <typename Callable, typename... Args>
decltype(auto) call(Callable&& op, Args&&... args)
{
    if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>) {
        std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...);
        std::cout << "void call(Callable&& op, Args&&... args)\n";
        return;
    } else {
        decltype(auto) ret{std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...)};
        std::cout << typeid(ret).name() << " call(Callable&& op, Args&&... args)\n";
        return ret;
    }
}
#endif

void print(int value)
{
    std::cout << "print(" << value << ")\n";
}
int x2(int value)
{
    std::cout << "x2(" << value << ")\n";
    return value * 2;
}

void test_call()
{
#if 201103L < __cplusplus
    call(print, 10);
    call(x2, 4);
#endif
}

} //namespace =================================================================

void test_ch_11_function()
{
#if (0)
    test_foreach();
#endif
    test_call();
}
