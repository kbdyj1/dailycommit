#include <iostream>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include <cmath>
#include <memory>

namespace { //=================================================================

// CP.111 Use a conventional pattern if you really need double-checked locking

namespace cp111 {

std::mutex actionGuard;

void takeAction()
{
    std::cout << "takeAction()" << std::endl;
}
void bad()
{
    volatile bool actionNeeded = true;
    if (actionNeeded) {
        auto lock = std::lock_guard(actionGuard);
        if (actionNeeded) {
            takeAction();
            actionNeeded = false;
        }
    }
}
void good()
{
    std::atomic_bool actionNeeded = true;
    if (actionNeeded) {
        auto lock = std::lock_guard(actionGuard);
        if (actionNeeded) {
            takeAction();
            actionNeeded = false;
        }
    }
}
} //cp111

// E.1 Develop an error-handling strategy early in a design

// E.2 Throw an exception to signal that a function can't perform its assigned task
//  exceptional ?
//      a precondition that cannot be met
//      a constructor that cannot construct an object
//      an out of range error
//      inability to acquire a resource

// E.3 Use exceptions for error handling only

namespace e3 {

int badFindIndex(std::vector<std::string>& v, const std::string& x)
{
    try {
        for (auto i=0; i<v.size(); i++) {
            if (v[i] == x)
                throw i;
        }
    } catch (int i) {
        return i;
    }

    return -1;
}

} //e3

// E.4 Design your error-handling strategy around invariants

// E.5 Let a constructor establish an invariant, and throw if it cannot

// E.6 Use RAII to prevent leaks

// E.7 State your preconditions

// E.8 State your postconditions

// E.12 Use noexcept when exiting a function because of a throw is impossible or unacceptable

double compute(double d) noexcept
{
    return log(sqrt(d <= 0 ? 1 : d));
}

// E.13 Never throw while being the direct owner of an object

namespace es13 {

class get_me_out_of_here : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "get me out of here";
    }
};

void leak(int x)
{
    auto p = new int{7};
    if (x < 0)
        throw get_me_out_of_here();
    delete p;
}

void noLeak(int x)
{
    auto p = std::make_unique<int>(7);
    if (x < 0)
        throw get_me_out_of_here();
}

} //es13

} //===========================================================================
