#include <iostream>

namespace { //=================================================================

template <typename T>
class Trab {
public:
    enum { x };
};

template <typename T>
class Victim {
public:
    int y;
    void poof() {
//        Trab<T>::x * y;
    }
};

template <>
class Trab<void> {
public:
    using x = int;
};

void boom(Victim<void>& bomb)
{
    bomb.poof();
}

void test()
{

}

} //namespace =================================================================

void test_ch_13_dependency()
{

}
