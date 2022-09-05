#include <iostream>
#include <future>
#include <thread>
#include <chrono>

namespace { //=================================================================

using namespace std::chrono_literals;

std::mutex gMutex;

void produce(std::promise<int>& p)
{
    std::this_thread::sleep_for(2s);

    p.set_value(62);
}


void consume(std::future<int>& f)
{
    auto value = f.get();

    std::cout << "consume(" << value << ")" << std::endl;
}

void test_promise()
{
    auto p = std::promise<int>{};
    auto t0 = std::thread(produce, std::ref(p));

    auto f = p.get_future();
    auto t1 = std::thread(consume, std::ref(f));

    t0.join();
    t1.join();
}

void func0()
{
    std::this_thread::sleep_for(2s);
    auto lock = std::lock_guard(gMutex);
    std::cout << "func0 done." << std::endl;
}

void func1()
{
    std::this_thread::sleep_for(1s);
    auto lock = std::lock_guard(gMutex);
    std::cout << "func1 done." << std::endl;
}

int func2()
{
    std::this_thread::sleep_for(2s);

    return 62;
}

int func3()
{
    std::this_thread::sleep_for(1s);

    return 61;
}

void test_async()
{
    {
        auto f = std::async(std::launch::async, func0);
        func1();
        f.wait();
    }
    {
        auto f = std::async(std::launch::async, func2);
        auto value = func3();
        std::cout << "func3(): " << value << std::endl;
        value += f.get();
        std::cout << "value: " << value << std::endl;
    }
}

void test_async_wait_for()
{
    auto f = std::async(std::launch::async, func0);
    while (true) {
        auto status = f.wait_for(500ms);
        if (status == std::future_status::ready)
            break;

        std::cout << "waiting..." << std::endl;
    }
}

} //namespace =================================================================

void test_ch_08_promise()
{
#if (0) // done
    test_promise();
    test_async();
#endif

    test_async_wait_for();
}
