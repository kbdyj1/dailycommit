#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include <vector>
#include <cassert>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::pair<T, T>& pair)
{
    os << "pair(" << pair.first << ", " << pair.second << ")";
    return os;
}

template <typename T, typename C = std::vector<T>>
class Stack {
    C elems;

public:
    Stack() = default;
    Stack(T elem) : elems(std::move(elem))
    {
        std::cout << "Stack(const T&): T = " << typeid(T).name() << "\n";
    }

    void push(const T& elem) {
        elems.push_back(elem);
    }
    const T& top() const {
        assert(!elems.empty());

        return elems.back();
    }
    void pop() {
        assert(!elems.empty());

        elems.pop_back();
    }
    bool empty() const {
        return elems.empty();
    }
    void print(std::ostream& os) const {
        for (const auto& elem : elems) {
            os << elem << " ";
        }
    }

    template <typename T2>
    Stack& operator=(const Stack<T2>&);

    template <typename U>
    friend std::ostream& operator<<(std::ostream&, const Stack<U>&);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& stack)
{
    stack.print(os);
    return os;
}

#endif // STACK_HPP
