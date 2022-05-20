#include <iostream>
#include <experimental/coroutine>

namespace { //=================================================================

template <typename T>
struct MyFuture {
    std::shared_ptr<T> value;
    MyFuture(std::shared_ptr<T> p) : value(p)
    {}
    ~MyFuture()
    {}
    T get() {
        return *value;
    }

    struct promise_type {
        std::shared_ptr<T> p = std::make_shared<T>();
        ~promise_type()
        {}
        MyFuture<T> get_return_object() {
            return p;
        }
        void return_value(T v) {
            *p = v;
        }
        std::experimental::suspend_never initial_suspend() {
            return {};
        }
        std::experimental::suspend_never final_suspend() noexcept {
            return {};
        }
        void unhandled_exception() {
            return std::exit(1);
        }
    };
};

MyFuture<int> createFuture() {
    co_return 2022;
}

template <typename T>
struct Generator
{
    struct promise_type;
    using handle_type = std::experimental::coroutine_handle<promise_type>;

    Generator(handle_type h) : coro(h)
    {}
    handle_type coro;

    ~Generator() {
        if (coro)
            coro.destroy();
    }

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : coro(other.coro) {
        other.coro = nullptr;
    }
    Generator& operator=(Generator&& other) noexcept {
        coro = other.coro;
        other.coro = nullptr;
        return *this;
    }
    T getValue() {
        return coro.promise().current_value;
    }
    bool next() {
        coro.resume();
        return not coro.done();
    }

    struct promise_type {
        promise_type() = default;
        ~promise_type() = default;

        auto initial_suspend() {
            return std::experimental::suspend_always{};
        }
        auto final_suspend() noexcept {
            return std::experimental::suspend_always{};
        }
        auto get_return_object() {
            return Generator{ handle_type::from_promise(*this) };
        }
        auto return_void() {
            return std::experimental::suspend_never();
        }
        auto yield_value(const T value) {
            current_value = value;
            return std::experimental::suspend_always{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
        T current_value;
    };
};

void test_future()
{
    auto future = createFuture();
    std::cout << "future.get() : " << future.get() << std::endl;
}

Generator<int> getNext(int start = 0, int step = 1)
{
    auto value = start;
    while (true) {
        co_yield value;
        value += step;
    }
}

void test_generator()
{
    std::cout << "getNext() : ";
    auto gen = getNext();
    for (int i= 0; i<=10; i++) {
        gen.next();
        std::cout << " " << gen.getValue();
    }
    std::cout << "\n\n";

    std::cout << "getNext(100, -10) : ";
    auto gen2 = getNext(100, -10);
    for (int i=0; i<=20; i++) {
        gen2.next();
        std::cout << " " << gen2.getValue();
    }
    std::cout << std::endl;
}

} // namespace ================================================================

void test_ch5_1()
{
    std::cout << std::endl;
    //test_future();
    test_generator();
}
