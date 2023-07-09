#include <utility>
#include <string>
#include <iostream>
#include <vector>

// move - unconditionaly cast to rvalue
// forward - conditionaly cast to rvalue

namespace { //=================================================================

namespace _1 {

template <typename T>
#if 201103L < __cplusplus
decltype (auto) my_move(T&& param)
{
    using ReturnType = std::remove_reference_t<T>&&;

    return static_cast<ReturnType>(param);
}
#else
typename std::remove_reference<T>::type&& my_move(T&& param)
{
    using ReturnType = typename std::remove_reference<T>::type&&;

    return static_cast<ReturnType>(param);
}
#endif

class Annotation
{
public:
    explicit Annotation(const std::string text) : text(my_move(text))
    {}

private:
    std::string text;
};

void process(const std::string& message)
{
    std::cout << "process(const std::string&)\n";
}
void process(std::string&& message)
{
    std::cout << "process(std::string&&)\n";
}

template <typename T>
void logAndProcess(T&& param)
{
    process(std::forward<T>(param));
}

template <typename T>
void f(T&& param)
{
    logAndProcess(std::forward<T>(param));
}

template <typename T>
void g(std::vector<T>&& param)
{
}

void test()
{
    std::string message = "Hello, Qt6";

    logAndProcess(message);
    logAndProcess(std::move(message));

    std::cout << "\n";

    f(message);
    f("Hello, Qt6");

    std::vector<std::string> v{"Hello, Qt6"};
#if (0)
    g(v);
#else
    g(std::move(v));
#endif
}

} //_1 --------------------------------------------------------------

namespace _2 {


} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_move()
{
    _1::test();
}
