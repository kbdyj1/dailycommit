#include <iostream>
#include <compare>

namespace
{

struct MyInt
{
    int mValue;
    explicit MyInt(int value) : mValue{value}
    {}
    bool operator<(const MyInt &rhs) const {
        return mValue < rhs.mValue;
    }
    bool operator==(const MyInt &rhs) const {
        return mValue == rhs.mValue;
    }
    bool operator!=(const MyInt &rhs) const {
        return !(*this == rhs);
    }
    bool operator<=(const MyInt &rhs) const {
        return !(rhs < *this);
    }
    bool operator>(const MyInt &rhs) const {
        return rhs < *this;
    }
    bool operator>=(const MyInt &rhs) const {
        return !(*this < rhs);
    }
};

struct MyInt2
{
    int mValue;
    explicit MyInt2(int value) : mValue{value}
    {}
    auto operator<=>(const MyInt2& rhs) const {
        return mValue <=> rhs.mValue;
    };
};

struct MyDouble
{
    double mValue;
    explicit constexpr MyDouble(double value) : mValue{value}
    {}
    auto operator<=>(const MyDouble& rhs) const = default;
};

template <typename T>
constexpr bool isLessThan(const T& lhs, const T& rhs)
{
    return lhs < rhs;
}

} // namespace

void test_ch3_3_3way_comparision()
{
    std::cout << std::boolalpha << std::endl;
#if (0)
    auto m0 = MyInt{2000};
    auto m1 = MyInt{2010};
#else
    auto m0 = MyInt2{2000};
    auto m1 = MyInt2{2010};
#endif
    std::cout << "isLessThan(m0, m1) : " << isLessThan(m0, m1) << std::endl;

    auto d0 = MyDouble{2000};
    auto d1 = MyDouble{2010};

    std::cout << "isLessThan(d0, d1) : " << isLessThan(d0, d1) << std::endl;
}
