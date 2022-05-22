#include <iostream>
#include <atomic>
#include <mutex>
#include <unistd.h>
#include <thread>

namespace { //=================================================================

// Lock free programming

// CP.100 Don't use lock-free programming unless you absolutely have to

struct Link {
    int data;
    Link* next;
};

std::atomic<Link*> head;

// CP.101 Distrust your hardware/compiler combination

// CP.102 Carefully study the literature

// CP.110 Do not write your own double-checked locking for initialization

void f_callonce()
{
    static std::once_flag myOnceFlag;
    std::call_once(myOnceFlag, []{
        std::cout << "call_once()" << std::endl;
    });
}

class A
{
public:
    A() { std::cout << "A()" << std::endl; }
};

void f_thread_safe_local_variables()
{
    static A a;
}

std::once_flag once0;
std::once_flag once1;

void simple_do_once(int id)
{
    std::call_once(once0, [=]{ std::cout << "simple_do_once(" << id << ")" << std::endl; });
}
void may_throw_function(int id, bool do_throw)
{
    if (do_throw) {
        std::cout << "may_throw_function(" << id << ", do_throw : " << do_throw << ")" << std::endl;
        throw std::exception();
    }
    std::cout << "may_throw_function(" << id << ") Didn't throw, call_once will not attmept again" << std::endl;
}
void do_once(int id, bool do_throw)
{
    try {
        std::call_once(once1, may_throw_function, id, do_throw);
    } catch (...) {
    }
}

void test_call_once()
{
    std::thread t0(simple_do_once, 0);
    std::thread t1(simple_do_once, 1);
    std::thread t2(simple_do_once, 2);
    std::thread t3(simple_do_once, 3);
    t0.join();
    t1.join();
    t2.join();
    t3.join();

    std::thread t4(do_once, 4, true);
    std::thread t5(do_once, 5, true);
    std::thread t6(do_once, 6, false);
    std::thread t7(do_once, 7, true);
    t4.join();
    t5.join(); // blocked here ???
    t6.join();
    t7.join();
}

} //===========================================================================

void test_cp_100()
{
    int data = 0;

    Link* nh = new Link();
    Link* h = head.load();
    do {
        if (h->data <= data)
            break;
        nh->next = h;
    } while (!head.compare_exchange_weak(h, nh));
}

class Singleton
{
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static std::unique_ptr<Singleton> singleton;
    static std::once_flag once;

public:
    Singleton() = default;
    static Singleton& instance()
    {
        std::call_once(once, []{
            singleton.reset(new Singleton);
        });
        return *singleton.get();
    }

    void log()
    {
        std::cout << "instance : %p" << singleton.get() << std::endl;
    }
};
std::once_flag Singleton::once;
std::unique_ptr<Singleton> Singleton::singleton;

void test_cp_110()
{
    //std::cout << "test_cp_110()" << std::endl;

    //f_callonce();
    //f_thread_safe_local_variables();
    //test_call_once();

    Singleton::instance().log();

    sleep(1);
}
