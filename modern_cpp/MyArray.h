#ifndef MYARRAY_H
#define MYARRAY_H

#include <iterator>
#include <functional>
#include <assert.h>

template <typename T, const size_t SIZE>
class MyArrayIterator {
public:
    typedef MyArrayIterator self_type;
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::random_access_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;

    MyArrayIterator() = default;
    MyArrayIterator(pointer ptr, const size_t index) : p(ptr), index(index)
    {}

    MyArrayIterator(const MyArrayIterator&) = default;
    MyArrayIterator& operator=(const MyArrayIterator&) = default;
    ~MyArrayIterator() = default;

    self_type& operator++() {
        if (SIZE <= index)
            throw std::out_of_range("Iterator can't be incremented past the end of range");
        ++index;
        return *this;
    }
    self_type& operator++(int) {
        self_type temp = *this;
        ++*this;
        return temp;
    }
    self_type& operator--() {
        if (index <= 0)
            throw std::out_of_range("Iterator can't be decrement past the begin of range");
        --index;
        return *this;
    }
    self_type& operator--(int) {
        self_type temp = *this;
        --*this;
        return temp;
    }
    reference operator*() const {
        if (p == nullptr)
            throw std::bad_function_call();
        return *(p + index);
    }
    reference operator->() const {
        if (p == nullptr)
            throw std::bad_function_call();
        return *(p + index);
    }
    self_type operator+(difference_type offset) const {
        self_type temp = *this;

        return temp += offset;
    }
    self_type operator-(difference_type offset) const {
        self_type temp = *this;

        return temp -= offset;
    }
    difference_type operator-(const self_type& other) const {
        assert(compatible(other));

        return index - other.index;
    }
    bool operator<(const self_type& other) const {
        assert(compatible(other));

        return index < other.index;
    }
    bool operator>(const self_type& other) const {
        return other < *this;
    }
    bool operator<=(const self_type& other) const {
        return !(other < *this);
    }
    bool operator>=(const self_type& other) const {
        return !(*this < other);
    }
    self_type& operator+=(const difference_type offset)
    {
        if (index + offset < 0 || SIZE < index + offset)
            throw std::out_of_range("Iterator can't be incremented past the end of range");
        index += offset;
        return *this;
    }
    self_type& operator-=(const difference_type offset)
    {
        return *this += -offset;
    }
    value_type& operator[](const difference_type offset)
    {
        return *(this + offset);
    }
    const value_type& operator[](const difference_type offset) const {
        return *(this + offset);
    }

    bool operator==(const self_type& other) const {
        assert(compatible(other));

        return index == other.index;
    }
    bool operator!=(const self_type& other) const {
        return !(*this == other);
    }

private:
    bool compatible(const self_type& other) const {
        return p == other.p;
    }

private:
    pointer p = nullptr;
    size_t index = 0;
};

template <typename T, const size_t SIZE>
class MyArray {
    T data[SIZE] = {};

public:
    typedef T value_type;
    typedef MyArrayIterator<T, SIZE> iterator;
    typedef MyArrayIterator<const T, SIZE> const_iterator;

    iterator begin() {
        return iterator{data, 0};
    }
    const_iterator begin() const {
        return const_iterator{data, 0};
    }
    iterator end() {
        return iterator{data, SIZE};
    }
    const_iterator end() const {
        return const_iterator{data, SIZE};
    }

    T& operator[](const size_t index) {
        if (index < SIZE)
            return data[index];
        else
            throw std::out_of_range("index out of range");
    }

    const     T& operator[](const size_t index) const {
        if (index < SIZE)
            return data[index];
        else
            throw std::out_of_range("index out of range");
    }

    size_t size() const { return SIZE; }
};

#endif // MYARRAY_H
