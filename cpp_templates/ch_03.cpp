//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.03 Non type template parameter
//
//  22/09/21
//=============================================================================

#include <iostream>
#include <array>
#include <cassert>
#include <algorithm>
#include <vector>
#include <iterator>

namespace { //=================================================================

#if 201103L <= __cpluplus
template <typename T, auto MaxSize>
#else
template <typename T, std::size_t MaxSize = 32>
#endif
class Stack {
    using size_type = decltype(MaxSize);

    std::array<T, MaxSize> elems;
    size_type numElems = 0;

public:
    Stack()
    {}

    void push(const T& elem) {
        assert(numElems < MaxSize);

        elems[numElems] = elem;
        numElems++;
    }
    void pop() {
        assert(!empty());

        numElems--;
    }
    const T& top() const {
        assert(!empty());

        return elems[numElems-1];
    }
    bool empty() const {
        return numElems == 0;
    }
    size_type size() const {
        return numElems;
    }
};

void test_stack()
{
    auto s = Stack<int, 2u>{};
    s.push(0);
    std::cout << "s[0]: " << s.top() << "\n";
    s.push(1);
    std::cout << "s[1]: " << s.top() << "\n";
}

template <typename T, int InitValue>
T addValue(T x) {
    return InitValue + x;
}

void test_function_template_parameter()
{
    auto ret = addValue<int, 10>(8);

    std::cout << "addValue<int, 10>(8): " << ret << "\n";

    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    std::transform(std::begin(v), std::end(v), std::begin(v), addValue<int, 10>);
    std::copy(std::begin(v), std::end(v), std::ostream_iterator<int>(std::cout, " "));
}

template <const char* TAG>
class My {
public:
    My() {
        std::cout << "My(" << TAG << ")\n";
    }
};

const char tag[] = "template";

void test_pointer_parameter()
{
    My<tag> m;

    static char local[] = "local";
    My<local> l;
}

} //namespace =================================================================

void test_ch_03()
{
#if (0)
    test_stack();
    test_function_template_parameter();
#endif

    test_pointer_parameter();
}
