#include <iostream>

namespace { //=================================================================

template <typename T>
class Node {
    Node<T>* allocate() {
        return nullptr;
    }
};

template <typename T>
class Tree {
    friend class Node<T>;
};

template <typename T>
class List {
    friend Node<T>* Node<T>::allocate();
};

template <typename T>
class Stack {
public:
    template <typename U>
    Stack<T>& operator=(const Stack<T>& other);

    template <typename>
    friend class Stack;
};

template <typename T>
class Creator {
    friend void feed(Creator<T>) {
        std::cout << "feed(Creator<" << typeid(T).name() << ">)\n";
    }
};

void test_creator()
{
    auto c0 = Creator<void>{};
    feed(c0);

    auto c1 = Creator<double>{};
    feed(c1);
}

template <typename T>
class Task
{};

template <typename T>
class Schedule
{
public:
    void dispatch(Task<T>* task) {

    }
};

class Manager {
    static int counter;

    template <typename T>
    friend class Task;

    template <typename T>
    friend void Schedule<T>::dispatch(Task<T>*);

    template <typename T>
    friend int ticket() {
        return ++Manager::counter;
    }
};

int Manager::counter = 0;

} //namespace =================================================================


void test_ch_12_friend()
{
    test_creator();
}
