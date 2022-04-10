#include <iostream>
#include <thread>

namespace { //=================================================================

// F50. Use a lambda when a function won't do (to capture local variables, or to write a local function

// F51. Where there is a choice, prefer default arguments over overloading

// F52. Prefer capturing by reference in lambdas that will be used locally, including passed to algorithms

// F53. Avoid capturing by reference in lambdas that will be used non-locally, including returned,
//      stored on the heap, or passed to another

// F54. If you capture this, capture all variables explicitly (no default capture)
void use(int a, int b)
{
    std::cout << "use(" << a << ", " << b << ")" << std::endl;
}

class A
{
    int x = 0;

public:
    void f()
    {
        int i = 0;
        auto lambdaBad = [=]{ use(i, x); };

        x = 1;
        lambdaBad();
        x = 2;
        lambdaBad();

        auto lambdaGood = [i, this] { use(i, x); };

        x = 3;
        lambdaGood();
        x = 4;
        lambdaGood();
    }
};

// F55. Don't use va_arg arguments
//  alternatives
//      overloads
//      variadic templates
//      variant arguments
//      initialize_list

// F56. Avoid unnecessary condition nesting

} // namespace ================================================================

void test_f_54()
{
    A a;

    a.f();
}
