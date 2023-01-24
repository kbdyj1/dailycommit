//=============================================================================
//  ADL - Argument Dependent Lookup
//=============================================================================

#include <iostream>

namespace  {

namespace _1 {

namespace adl {
    struct My {};
    void function(const My&) { std::cout << "adl::function(My&)\n"; }
}
void test()
{
    adl::My my;

    function(my);
}

} //_1 --------------------------------------------------------------

namespace _2 {

class Distance {
    double m;

public:
    Distance(double i) : m(i)
    {}

    friend Distance operator+(const Distance& l, const Distance& r) {
        return Distance(l.m + r.m);
    }
    friend Distance operator-(const Distance& l, const Distance& r) {
        return Distance(l.m - r.m);
    }
    friend std::ostream& operator<<(std::ostream& os, const Distance& d) {
        os << "Distance(" << d.m << ")\n";
        return os;
    }
};

void test()
{
    std::cout << "\n";
    std::cout << "Distance(5.5) + Distance(4.5): " << Distance(5.5) + Distance(4.5) << "\n";
    std::cout << "Distance(5.5) - Distance(4.5): " << Distance(5.5) - Distance(4.5) << "\n";
    std::cout << "\n";
}

} //_2 --------------------------------------------------------------

}

void test_adl()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
