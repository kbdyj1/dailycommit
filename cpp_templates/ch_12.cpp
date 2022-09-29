//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.12 Template details
//
//  22/09/29
//=============================================================================

#include <iostream>

namespace { //=================================================================

template <typename T>
class Data {
public:
    static constexpr bool copyable = true;
};

template <typename T>
void log(T message)
{
    std::cout << message << "\n";
}

template <typename T>
T zero = 0;

template <typename T>
bool dataCopyable = Data<T>::copyable;

template <typename T>
using DataList = Data<T*>;

class Collection {
public:
    template <typename T>
    class Node {
    };

    template <typename T>
    T* alloc() {
        return nullptr;
    }

    template <typename T>   //c++14
    static T zero = 0;

    template <typename T>
    using NodePtr = Node<T>*;
};

} //namespace =================================================================

void test_ch_12()
{

}
