#include <iostream>
#include <atomic>
#include <mutex>
#include <unistd.h>

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

void test_cp_110()
{
    std::cout << "test_cp_110()" << std::endl;

    //f_callonce();
    f_thread_safe_local_variables();

    sleep(1);
}
