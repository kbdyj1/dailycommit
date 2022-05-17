#include <iostream>
#include <mutex>
#include <thread>

namespace { //=================================================================

// CP.9 Whenever feasible use tools to validate your concurrent code
//  clang, gcc have some support for static annotation of thread safety properties
//  Clang's Thread Sanitizer(TSAN)

// CP.20 Use RAII, never plain lock() / unlock()

std::mutex m;

void bad()
{
    m.lock();
    {
        //...
    }
    m.unlock();
}

void good()
{
    std::unique_lock<std::mutex> lock{m};

    //...
}

struct Box
{
    int mNum;
    std::mutex mMutex;
    explicit Box(int num) : mNum(num)
    {}
};
void transfer(Box& from, Box& to, int num)
{
    auto lock0 = std::unique_lock<std::mutex>(from.mMutex, std::defer_lock);
    auto lock1 = std::unique_lock<std::mutex>(to.mMutex, std::defer_lock);

    std::lock(lock0, lock1);

    from.mNum -= num;
    to.mNum += num;
}

// CP.21 Use std::lock() or std::scoped_lock to acquire multiple mutexes

std::mutex m0;
std::mutex m1;

void maybeDeadLock()
{
    auto l0 = std::lock_guard<std::mutex>(m0);
    auto l1 = std::lock_guard<std::mutex>(m1);
}

void better()
{
    std::lock(m0, m1);
    auto l0 = std::lock_guard<std::mutex>(m0);
    auto l1 = std::lock_guard<std::mutex>(m1);
}

void better_cxx17()
{
    auto l = std::scoped_lock(m0, m1);
}

// CP.22 Never call unknown code while holding a lock(e.g., a callback)

// Flag calling a virtual function with a non-recursive mutex held
// Flag calling a callback with a non-recursive mutex held

// CP.23 Think of a joining thread as a scoped container

struct Joining
{
    std::thread mThread;

    Joining(std::thread&& t) : mThread(std::move(t))
    {}
   ~Joining()
    {
        mThread.join();
    }
};

void func(int* p)
{
    std::cout << "func(" << *p << ") -> 99" << std::endl;
    *p = 99;
}

int globalX = 3;

void testJoinable(int* p)
{
    int x = 1;
    Joining j0(std::thread(func, &x));
    Joining j1(std::thread(func, p));
    Joining j2(std::thread(func, &globalX));
    auto q = std::make_unique<int>(4);
    Joining j3(std::thread(func, q.get()));
}

// CP.24 Think of a thread as a global container

void testThreadDetach(int* p)
{
    int x = 1;
    auto t0 = std::thread(func, &x);
    auto t1 = std::thread(func, p);
    auto t2 = std::thread(func, &globalX);
    auto q = std::make_unique<int>(4);
    auto t3 = std::thread(func, q.get());

    t0.detach();
    t1.detach();
    t2.detach();
    t3.detach();
}

void threadCallee()
{
    std::cout << "\tthreadCallee() #1" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "\tthreadCallee() #2" << std::endl;
}

void threadCaller()
{
    std::cout << "threadCaller() #1" << std::endl;
    auto t = std::thread(threadCallee);
    t.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "threadCaller() #2" << std::endl;
}

// CP.25 Prefer gsl::joining_thread over std::thread

void g()
{
    std::cout << "Hello, ";
}
struct Func
{
    void operator()() const { std::cout << "Qt 6." << std::endl; };
};

} //===========================================================================

void test_cp_20()
{
    auto box0 = Box(100);
    auto box1 = Box(50);

    std::thread t0(transfer, std::ref(box0), std::ref(box1), 10);
    std::thread t1(transfer, std::ref(box1), std::ref(box0), 5);

    t0.join();
    t1.join();

    std::cout << "box0.num: " << box0.mNum << std::endl;
    std::cout << "box1.num: " << box1.mNum << std::endl;
}

void test_cp_23()
{
    int x = 2;

    std::cout << "before testJoinable(&x)" << std::endl;

    testJoinable(&x);

    std::cout << "after testJoinable(&x)" << std::endl;

    std::cout << "x : " << x << std::endl;
    std::cout << "globalX : " << globalX << std::endl;
}

void test_cp_24()
{
#if (0)
    int x = 2;

    std::cout << "before testThreadDetach(&x)" << std::endl;

    testThreadDetach(&x);

    std::cout << "after testThreadDetach(&x)" << std::endl;

    std::cout << "x : " << x << std::endl;
    std::cout << "globalX : " << globalX << std::endl;
#else
    threadCaller();
    std::this_thread::sleep_for(std::chrono::seconds(5));
#endif
}

void test_cp_25()
{
    auto t0 = std::thread(g);
    auto t1 = std::thread(Func());

#if (0)
//    libc++abi: terminating
//    Hello, Qt 6.
#else
    t0.join();
    t1.join();
//    Qt 6.
//    Hello,
#endif
}
