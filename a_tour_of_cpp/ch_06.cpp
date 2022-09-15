//=============================================================================
//  A tour of C++
//  Ch.06  Template
//
//  22/09/14
//=============================================================================

#include <iostream>
#include <algorithm>

namespace { //=================================================================

using namespace std::string_literals;

template <typename T>
class Vector {
    T* data;
    size_t len;

public:
    Vector(std::initializer_list<T> list) : len(list.size()), data(new T[list.size()]) {
        std::copy(std::begin(list), std::end(list), data);
    }
    explicit Vector(int s) : len(s), data(new T[len])
    {}
    ~Vector() {
        delete [] data;
    }
    Vector& operator=(const Vector&);

    Vector(Vector&& other);
    Vector& operator=(Vector&& other);

    const T& operator[](int index) const;
    T& operator[](int index);
    size_t size() const {
        return len;
    }
};

template<typename T>
Vector<T>::Vector(Vector&& other) {
    data = other.data;
    len = other.len;
    for (auto i=0; i<len; i++) {
        data[i] = other.data[i];
    }
    other.data = nullptr;
    other.len = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other) {
        delete [] data;
        data = new T[other.len];
        len = other.len;
    }
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) {
    if (this != &other) {
        data = other.data;
        len = other.len;
        other.data = nullptr;
        other.len = 0;
    }

    return *this;
}

template<typename T>
const T& Vector<T>::operator[](int index) const {
    if (index < 0 || len <= index) {
        throw std::out_of_range{"Vector[]"};
    }
    return data[index];
}

template<typename T>
T& Vector<T>::operator[](int index) {
    if (index < 0 || len <= index) {
        throw std::out_of_range{"Vector[]"};
    }
    return data[index];
}

template<typename T>
T* begin(Vector<T>& v) {
    return v.size() ? &v[0] : nullptr;
}

template<typename T>
    T* end(Vector<T>& v) {
    return v.size() ? &v[0] + v.size() : nullptr;
}

template<typename T>
const T* begin(const Vector<T>& v) {
    return v.size() ? &v[0] : nullptr;
}

template<typename T>
const T* end(const Vector<T>& v) {
    return v.size() ? &v[0] + v.size() : nullptr;
}

void test_vector()
{
    auto v = Vector<int>{1, 2, 3, 4, 5};
    for (auto i : v) {
        std::cout << i << ",";
    }
    std::cout << "\n";
}

void test_pair()
{
    auto p0 = std::pair<int, std::string>{1, "one"};
    std::pair p1 = std::make_pair(2, "two");
    std::pair p2 = {3, "three"s};
}

} //namespace =================================================================

void test_ch_06_lambda();

void test_ch_06()
{
#if (0) // done
    test_vector();
#endif

    test_ch_06_lambda();
}
