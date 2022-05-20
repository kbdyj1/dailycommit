#include <iostream>
#include <fstream>
#include <future>
#include <functional>
#include <unistd.h>

namespace { //=================================================================

// Parallelism

// Message passing
//  low level : threads, mutexes, atomic types, ...
//  high level: messaging libraries, parallel algorithms, vetorization

// CP.60 Use a future to return a value from a concurrent task

// CP.61 Use async() to spawn concurrent tasks

int read_value(const std::string& filename)
{
    std::ifstream in(filename);
    in.exceptions(std::ifstream::failbit);
    int value;
    in >> value;
    return value;
}

void async_test()
{
    {
        std::fstream v0("v0.txt", std::ios::out);
        v0 << 6;

        std::fstream v1("v1.txt", std::ios::out);
        v1 << 3;
    }

    try {
        auto f0 = std::async(read_value, "v0.txt");
        auto f1 = std::async(read_value, "v1.txt");

        std::cout << "v0.txt + v1.txt : " << f0.get() + f1.get() << std::endl;
    } catch (std::ios_base::failure& fail) {
        std::cerr << fail.what() << std::endl;
    }
    sleep(1);
}

void async_bad_example()
{
    std::promise<int> p0;
    std::future<int> f0 = p0.get_future();
    std::thread t0([p0 = std::move(p0)]() mutable {
        p0.set_value(read_value("v0.txt"));
    });
    t0.detach();

    std::packaged_task<int()> task(std::bind(read_value, "v1.txt"));
    std::future<int> f1 = task.get_future();
    std::thread t1(std::move(task));
    t1.detach();

    std::cout << "async_bad_example : " << f0.get() + f1.get() << std::endl;

    sleep(1);
}

int f(int x, int y) { return x + y; }

void task_lambda()
{
    std::packaged_task<int(int,int)> task([](int a, int b){
        return a + b;
    });
    std::future<int> result = task.get_future();

    task(6, 3);
    std::cout << "task_lambda : " << result.get() << std::endl;
}

void task_bind()
{
    std::packaged_task<int()> task(std::bind(f, 6, 3));
    std::future<int> result = task.get_future();

    task();
    std::cout << "task_bind : " << result.get() << std::endl;
}

void task_thread()
{
    std::packaged_task<int(int,int)> task(f);
    std::future<int> result = task.get_future();

    std::thread t(std::move(task), 5, 3);
    t.join();

    std::cout << "task_thread : " << result.get() << std::endl;
}

// Vectorization

} //===========================================================================

void test_cp_61()
{
    //async_test();
    async_bad_example();
    //task_lambda();
    //task_bind();
    //task_thread();
}
