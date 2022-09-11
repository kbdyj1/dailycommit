#include <iostream>
#include <vector>
#include <memory>

#include "util.h"

#define USE_ATTORNEY

namespace { //=================================================================

class Client {
    int data0;
    int data1;

    void action0() const { std::cout << "Client::action0()\n"; }
    void action1() const { std::cout << "Client::action1()\n"; }

#if defined(USE_ATTORNEY)
    friend class Attorney;
#else
    friend class Friend;
#endif

public:
    Client(int d0, int d1) : data0(d0), data1(d1)
    {}
};

#if defined(USE_ATTORNEY)
class Attorney {
    static inline void runAction0(const Client& c) {
        c.action0();
    }
    static inline int data0(const Client& c) {
        return c.data0;
    }
    friend class Friend;
};
#endif

class Friend {
public:
    void accessClient(const Client& c) {

#if defined(USE_ATTORNEY)
        Attorney::runAction0(c);
        std::cout << Attorney::data0(c) << std::endl;
#else
        PRINT_ATTR(c.data0);
        PRINT_ATTR(c.data1);

        c.action0();
        c.action1();
#endif
    }
};

class B {
    virtual void exec() {
        std::cout << "B::exec()\n";
    }
    friend class BAttorney;
};

class D : public B {
    void exec() override {
        std::cout << "D::exec()\n";
    }
};

class BAttorney {
    static inline void exec(B& b) {
        b.exec();
    }
    friend class F;
};

class F {
public:
    void run() {
        auto b = B{};
        BAttorney::exec(b);

        auto d = D{};
        BAttorney::exec(d);
    }
};

} //namespace =================================================================

void test_ch_10_friendship()
{
    {
        auto c = Client(6, 2);
        auto f = Friend{};

        f.accessClient(c);
    }

    {
        auto f = F{};
        f.run();
    }
}
