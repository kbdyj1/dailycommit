//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.02 class template
//
//  22/09/20
//=============================================================================

#include <iostream>
#include <deque>
#include <string>

#include "stack.hpp"

template <typename T>
class Stack<T*> {
    std::vector<T*> elems;

public:
    void push(T* elem) {
        elems.push_back(elem);
    }
    T* pop() {
        assert(!elems.empty());

        T* ret = elems.back();
        elems.pop_back();
        return ret;
    }
    T* top() const {
        assert(!elems.empty());

        return elems.back();
    }
};

template <typename T>
struct ViewWithComment {
    T value;
    std::string comment;
};

ViewWithComment(const char*, const char*) -> ViewWithComment<std::string>;

namespace { //=================================================================

void test_stack()
{
    auto s = Stack<std::pair<int, int>>{};
    s.push({1, 1});
    s.push({2, 2});

#if (0)
    std::cout << s.top().first << ", " << s.top().second << "\n";
#else
    std::cout << s << "\n";
#endif
}

void test_stack_partial_specialization()
{
    auto s = Stack<int*>{};
    s.push(new int{62});
    auto* ret = s.pop();
    std::cout << "*ret: " << *ret << "\n";
    delete ret;
}

template <typename T>
using DequeStack = Stack<T, std::deque<T>>;

void test_type_deduction()
{
    auto s0 = Stack<int>{};
    auto s1 = s0;
}

} //namespace =================================================================

void test_ch_02()
{
#if (0) // done
    test_stack();
    test_stack_partial_specialization();
#endif

    test_type_deduction();
}
