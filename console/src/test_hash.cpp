#include <iostream>
#include <functional>
#include <iomanip>

namespace { //=================================================================

// 1. Accepts a single parameter of type Key
// 2. Returns a value of type std::size_t that represents the hash value of the parameter
// 3. Does not throw exceptions when called
// 4. For two parameters k1 and k2 that are equal, std::hash<Key>()(k1) == std::hash<Key>()(k2)
// 5. For two different parameter k1 and k2 that are not equal, the probability that
//    std::hash<Key>()(k1) == std::hash<Key>()(k2) should be very small

struct S
{
    std::string firstName;
    std::string secondName;
};
bool operator==(const S& l, const S& r)
{
    return l.firstName == r.firstName && l.secondName == r.secondName;
}

struct Hash
{
    std::size_t operator()(S const& s) const noexcept
    {
        std::size_t h0 = std::hash<std::string>{}(s.firstName);
        std::size_t h1 = std::hash<std::string>{}(s.secondName);

        return h0 ^ (h1 << 1);
    }
};
#if (0)
template<>
struct std::hash<S>
{
    std::size_t operator()(S const& s) const noexcept
    {
        std::size_t h0 = std::hash<std::string>{}(s.firstName);
        std::size_t h1 = std::hash<std::string>{}(s.secondName);

        return h0 ^ (h1 << 1);
    }
};
#endif

} // namespace ================================================================

void test_hash()
{
    std::string str = "Hello, Qt6";
    std::size_t h0 = std::hash<std::string>{}(str);
    std::cout << "hash(" << std::quoted(str) << ") : " << h0 << std::endl;

    S s{"Hello, ", "Qt6"};
    std::cout << "hash(" << std::quoted(s.firstName) << ", " << std::quoted(s.secondName) << ") : "
              << Hash{}(s) << std::endl;
}
