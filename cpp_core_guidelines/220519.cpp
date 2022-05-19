#include <iostream>
#include <future>
#ifdef OS_MAC
#include <experimental/coroutine>
#define TEST_COROUTINE
#endif

namespace { //=================================================================

// Coroutines

// co_await
// co_yield
// co_return

class Task
{
public:
#ifdef TEST_COROUTINE
    struct promise_type
    {
        Task get_return_object()
        {
            return Task { std::experimental::coroutine_handle<promise_type>::from_promise(*this) };
        }

        auto initial_suspend() {
            std::cout << "initial_suspend()" << std::endl;
            return std::experimental::suspend_always{};
        }
        auto return_void() { return std::experimental::suspend_never{}; }
        auto final_suspend() noexcept {
            std::cout << "final_suspend()" << std::endl;
            return std::experimental::suspend_always{};
        }
        void unhandled_exception() { std::exit(1); }
    };

    std::experimental::coroutine_handle<promise_type> co_handler;

    Task(std::experimental::coroutine_handle<promise_type> handler) : co_handler(handler)
    {}

    ~Task()
    {
        if (true == (bool)co_handler) {
            co_handler.destroy();
        }
    }
#endif
};
Task foo()
{
    std::cout << "foo() #1" << std::endl;
#ifdef TEST_COROUTINE
    co_await std::experimental::suspend_always{};
#endif
    std::cout << "foo() #2" << std::endl;
}
void test_coroutine()
{
    auto task = foo();
    std::cout << "test_cp_51() #1" << std::endl;
#ifdef TEST_COROUTINE
    task.co_handler.resume();
#endif

    std::cout << "test_cp_51() #2" << std::endl;

#ifdef TEST_COROUTINE
    task.co_handler.resume();
#endif
}
// CP.51 Do not use capturing lambdas that are coroutines

#if (0)
// Bad ------------------------------------------------------------------------
int value = getValue();
std::shared_ptr<Foo> foo = get_foo();
{
    const auto lambda = [value, foo]() -> std::future<void>
    {
        co_await something();
    };
    lambda();
}

// Better ---------------------------------------------------------------------
int value = getValue();
std::shared_ptr<Foo> foo = get_foo();
{
    const auto lambda = [](auto foo, auto value) -> std::future<void>
    {
        co_await something();
    };
    lambda(foo, value);
}

// Best -----------------------------------------------------------------------
std::future<void> Class::do_something(int value, std::shared_ptr<Foo> s)
{
    co_await something();
}

void SomeOtherFunction()
{
    int value = getValue();
    std::shared_ptr<Foo> foo = get_foo();
    do_something(value, foo);
}
#endif

// CP.52 Do not hold locks or other synchronization primitives across suspension points

#if (0)
std::mutex m;

std::future<void> Class::bad()
{
    std::lock_guard<std::mutex> lock(m);
    co_await something();
    co_await somethingElse();
}

std::future<void> Class::good()
{
    {
        std::lock_guard<std::mutex> lock(m);
    }
    co_await something();
    co_awiat comethingElse();
}
#endif

// CP.53 Parameters to coroutines should not be passed by reference

} //===========================================================================

void test_cp_51()
{
    test_coroutine();
}
