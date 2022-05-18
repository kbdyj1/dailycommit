#include <iostream>
#include <future>
#include <thread>
#include <list>

namespace { //=================================================================

// CP.26 Don't detach() a thread

// CP.31 Pass small amounts of data detween threads by value, rather than by reference or pointer

std::string modify1(std::string s)
{
    return "[ " + s + " ]";
}

void modify2(const std::string& s)
{
    std::cout << "( " << s << ")" << std::endl;
}

void f(std::string& s)
{
    auto a0 = std::async(modify1, s);

    auto a1 = std::async(std::launch::deferred, modify2, s);
    a1.wait();

    std::cout << "a0.get() : " << a0.get() << std::endl;
}

std::mutex m;

struct X
{
    void foo(int i, const std::string& str)
    {
        auto l = std::lock_guard(m);
        std::cout << "X::foo(" << i << ", " << str << ")" << std::endl;
    }
    void bar(const std::string& str)
    {
        auto l = std::lock_guard(m);
        std::cout << "X::bar(" << str << ")" << std::endl;
    }
    int operator()(int i)
    {
        auto l = std::lock_guard(m);
        std::cout << "X::operator(" << i << ")" << std::endl;

        return i*2;
    }
};

void test_async()
{
    auto x = X();

    auto a0 = std::async(&X::foo, &x, 6, "Hello, Qt");
    auto a1 = std::async(std::launch::deferred, &X::bar, x, "World");
    auto a2 = std::async(std::launch::async, X(), 6);

    a1.wait();

    std::cout << "a2.get() : " << a2.get() << std::endl;
}

// CP.32 To share ownership between unrelated threads use shared_ptr

// CP.40 Minimize context switching

// CP.41 Minimize thread creation and destruction
//  thread creation is expensive
//  use thread-pool or message queue

// CP.42 Don't wait without a condition

std::condition_variable cv;

void thread0()
{
    while (true) {
        // work

        auto l = std::unique_lock(m);
        cv.notify_one();
    }
}

void thread1()
{
    while (true) {
        auto l = std::unique_lock(m);

        // if some other thread consumes thread0's notification, thread1 can wait forever
        cv.wait(l);

        // work
    }
}

template <typename T>
class SyncQueue
{
    std::mutex mMutex;
    std::condition_variable mCond;
    std::list<T> mList;

public:
    void put(const T& value)
    {
        auto l = std::lock_guard(m);
        mList.push_back(value);
        mCond.notify_one();
    }
    void get(T& value)
    {
        auto l = std::lock_guard(m);
        mCond.wait(l, [this]{ return !mList.empty(); });
        value = mList.front();
        mList.pop_front();
    }
};

// CP.43 Minimize time spent in a critical section
//  thread suspension and resumption are expensive

void prepare() {}
void transaction() {}
void cleanup() {}

void badTransaction()
{
    auto l = std::unique_lock(m);
    prepare();
    transaction();
    cleanup();
}

void goodTransaction()
{
    prepare();
    {
        auto l = std::unique_lock(m);
        transaction();
    }
    cleanup();
}

// CP.44 Remember to name your lock_guards and unique_locks
//  flag all unnamed lock_guards and unique_locks

// CP.50 Define a mutex together the data it guards. Use synchronized_value<T> where possible

} //===========================================================================

void test_cp_31()
{
    std::string s = "Hello, Qt 6";

    //f(s);

    test_async();
}
