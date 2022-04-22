#include <iostream>
#include <algorithm>

namespace { //=================================================================

std::string encode(std::string message, int key)
{
    std::transform(std::begin(message), std::end(message), std::begin(message), [key](char c) -> char {
        return c + key;
    });
    return message;
}
std::string decode(std::string message, int key)
{
    std::transform(std::begin(message), std::end(message), std::begin(message), [key](char c) -> char {
        return c - key;
    });
    return message;
}

} // namespace ================================================================

void test_caesar_cipher()
{
    std::string message = "Hello, Qt6";
    int key = 5;

    auto enc = encode(message, key);
    std::cout << "encode : " << enc << std::endl;

    auto dec = decode(enc, key);
    std::cout << "decode : " << dec << std::endl;
}
