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

void test0()
{

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

class MyInt3
{
public:
    constexpr explicit MyInt3(int value) : mValue{value}
    {}
    auto operator<=>(const MyInt3 &rhs) const = default;

    constexpr auto operator<=>(const int& rhs) const {
        return mValue <=> rhs;
    }
private:
    int mValue;
};

void test1()
{
    constexpr auto m0 = MyInt3{2000};
    constexpr auto m1 = MyInt3{2002};
    constexpr auto i0 = int{2000};
    constexpr auto i1 = int{2002};

    auto res = m0 < m1;
    std::cout << "m0 < m1 : " << res << std::endl;

    res = (m0 <=> m1) < 0;
    std::cout << "(m0 <=> m1) < 0 : " << res << std::endl;

    res = m0 < i1;
    std::cout << "m0 < i1 : " << res << std::endl;

    res = (m0 <=> i1) < 0;
    std::cout << "(m0 <=> i1) < 0 : " << res << std::endl;

    res = i0 < m1;
    std::cout << "i0 < m1 : " << res << std::endl;

    res = 0 < (i1 <=> m0);
    std::cout << "0 < (i1 <=> m0) : " << res << std::endl;
}

class MyInt4
{
public:
    constexpr explicit MyInt4(int value) : mValue{value}
    {}
    bool operator==(const MyInt4& rhs) const {
        std::cout << "== " << std::endl;
        return mValue == rhs.mValue;
    }
    bool operator<(const MyInt4& rhs) const {
        std::cout << "< " << std::endl;
        return mValue < rhs.mValue;
    }
    auto operator<=>(const MyInt4& rhs) const = default;

private:
    int mValue;
};

void test2()
{
    auto m0 = MyInt4{2000};
    auto m1 = MyInt4{2002};

    std::cout << "m0 == m1" << std::endl;
    m0 == m1;
    std::cout << "m0 != m1" << std::endl;
    m0 != m1;
    std::cout << "m0 < m1" << std::endl;
    m0 < m1;
    std::cout << "m0 <= m1" << std::endl;
    m0 <= m1;
    std::cout << "m0 > m1" << std::endl;
    m0 > m1;
    std::cout << "m0 >= m1" << std::endl;
    m0 >= m1;
}

} // namespace

void test_ch3_3_3way_comparision()
{
    std::cout << std::boolalpha << std::endl;

    //test0();
    //test1();
    test2();
}
