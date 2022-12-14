#include <iostream>
#include <utility>
#include <string>
#include <functional>

namespace { //=================================================================

namespace _1 {

void test() {
    int value = 1;

#if (0)
    //cannot assign to a variable captured by copy in a non-mutable lambda
    auto increment = [=]{ return ++value; };
#else
    auto increment = [&immutableValue = std::as_const(value)]{
        return immutableValue + 1;
    };
#endif

    const int* p = new int(1);
    auto incrementPointer = [=]{ return (*p) + 1; };
    delete p;
}

} //_1 --------------------------------------------------------------

namespace _2 {

class ImaginaryNumber {
    int real;
    int imaginary;

public:
    ImaginaryNumber() : real(0), imaginary(0)
    {}
    ImaginaryNumber(int r, int i) : real(r), imaginary(i)
    {}
    ImaginaryNumber(const ImaginaryNumber& other) : real(other.real), imaginary(other.imaginary)
    {}

#if (0)
    //'auto' not allowed in non-static class member
    auto toString = []{
#else
    std::function<std::string()> toString = [=]{
#endif
        return std::to_string(real) + " + " + std::to_string(imaginary) + "I";
    };

    static std::function<std::string(const ImaginaryNumber&)> toStringStatic;

    std::string toStringUsingLambda(){
        auto l = ImaginaryNumber::toStringStatic;

        return l(*this);
    }
};

std::function<std::string(const ImaginaryNumber&)> ImaginaryNumber::toStringStatic = [](const ImaginaryNumber& number) {
    return std::to_string(number.real) + " + " + std::to_string(number.imaginary) + "i";
};

void test() {
    auto i0 = ImaginaryNumber(1, 2);

    std::cout << i0.toString() << "\n";
    std::cout << ImaginaryNumber::toStringStatic(i0) << "\n";
    std::cout << i0.toStringUsingLambda() << "\n";
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_03()
{
    _2::test();
}
