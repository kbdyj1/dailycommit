#include <iostream>
#include <algorithm>
#include <vector>
#include "util.h"

namespace { //=================================================================

class Buffer
{
    unsigned char* ptr;
    size_t length;

public:
    Buffer() : ptr(nullptr), length(0)
    {
        std::cout << "Buffer()\n";
    }
    explicit Buffer(const size_t size) : ptr(new unsigned char[size]), length(size)
    {
        std::cout << "Buffer(const size_t)\n";
    }
    ~Buffer()
    {
        delete [] ptr;
    }
    Buffer(const Buffer& other) : ptr(nullptr), length(0)
    {
        std::cout << "Buffer(const Buffer&)\n";
        //std::copy(other.ptr, other.ptr+length, ptr);
        *this = std::move(other);
    }
    Buffer(Buffer&& other) : ptr(other.ptr), length(other.length)
    {
        std::cout << "Buffer(Buffer&&)\n";
        other.ptr = nullptr;
        other.length = 0;
    }
    Buffer& operator=(const Buffer& other)
    {
        std::cout << "Buffer& operator=(const Buffer&)\n";
        if (this != &other) {
            delete [] ptr;

            length = other.length;
            ptr = new unsigned char[length];
            std::copy(other.ptr, other.ptr+length, ptr);
        }
        return *this;
    }
    Buffer& operator=(Buffer&& other)
    {
        std::cout << "Buffer& operator=(Buffer&&)\n";
        if (this != &other) {
            delete [] ptr;

            ptr = other.ptr;
            length = other.length;

            other.ptr = nullptr;
            other.length = 0;
        }
        return *this;
    }
    size_t size() const {
        return length;
    }
    unsigned char* data() const {
        return ptr;
    }
};

void test_rvalue()
{
    auto v = std::vector<Buffer>{};
#if (0)
    v.push_back(Buffer(100));
#else
    v.emplace_back(100);
#endif

    auto a = Buffer(10);
    auto b = Buffer(20);

    PRINT_FUNC(b.size());

    a = std::move(b);

    PRINT_FUNC(b.size());
}

} //namespace =================================================================

void test_ch_09_move()
{
    test_rvalue();
}
