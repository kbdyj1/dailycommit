#ifndef A_H
#define A_H

#include <iostream>

class A {
    int value;
public:
    A(int value = 0) : value(value)
    {
        std::cout << "A(" << value << ")\n";
    }
    A(const A& rhs) : value(rhs.value)
    {
        std::cout << "A(A) : value(" << value << ")\n";
    }
    A& operator=(const A&rhs)
    {
        if (this != &rhs) {
            value = rhs.value;
            std::cout << "A::operator=(" << rhs.value << ")\n";
        }
        return *this;
    }
    ~A()
    {
        std::cout << "~A(" << value << ")\n";
    }
    int getValue() const {
        return value;
    }
};

#endif // A_H
