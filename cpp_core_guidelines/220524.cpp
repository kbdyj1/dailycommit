#include <iostream>

namespace { //=================================================================

// E.25 If you can't throw exceptions, simulate RAII for resource management

// E.26 If you can't throw exceptions, consider failing fast

// E.27 If you can't throw exceptions, use error codes systematically

class Gadget
{
public:
    bool isValid() const
    {
        return true;
    }
};

Gadget makeGadget(int n)
{
    return Gadget();
}

void useGadget()
{
    auto g = makeGadget(10);
    if (g.isValid()) {
        //...
    }
}

// E.28 Avoid error handling based on global state (e.g. errno)
//      global state is hard to manage it and it is easy to forget to check it.

// E.30 Don't use exception specifications

// E.31 Properly order your catch-clauses

} //===========================================================================
