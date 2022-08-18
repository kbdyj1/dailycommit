#include <iostream>

namespace { //=================================================================

constexpr size_t operator"" _KB(const unsigned long long size)
{
    return static_cast<size_t>(1024 * size);
}
constexpr size_t operator"" _MB(const unsigned long long size)
{
    return static_cast<size_t>(1024*1024*size);
}

void test_kb()
{
    std::cout << "4KB: " << 4_KB << " bytes\n";
    std::cout << "4MB: " << 4_MB << " bytes\n";
}

enum class Unit {
    Kilogram,
    Liter,
    Meter,
    Piece
};

template <Unit U>
class Quanty {
    const double amount;
public:
    constexpr explicit Quanty(const double a) : amount(a)
    {}
    explicit operator double() const {
        return amount;
    }
};

template <Unit U>
constexpr Quanty<U> operator+(const Quanty<U>& l, const Quanty<U>& r)
{
    return Quanty<U>(static_cast<double>(l) + static_cast<double>(r));
}

template <Unit U>
constexpr Quanty<U> operator-(const Quanty<U>& l, const Quanty<U>& r)
{
    return Quanty<U>(static_cast<double>(l) - static_cast<double>(r));
}

namespace unit_literals {

constexpr Quanty<Unit::Kilogram> operator"" _kg(const long double amount)
{
    return Quanty<Unit::Kilogram>{ static_cast<double>(amount) };
}

constexpr Quanty<Unit::Kilogram> operator"" _kg(const unsigned long long amount)
{
    return Quanty<Unit::Kilogram>{ static_cast<double>(amount) };
}

constexpr Quanty<Unit::Liter> operator"" _l(const long double amount)
{
    return Quanty<Unit::Liter>{ static_cast<double>(amount) };
}

constexpr Quanty<Unit::Meter> operator"" _m(const long double amount)
{
    return Quanty<Unit::Meter>{ static_cast<double>(amount) };
}

constexpr Quanty<Unit::Piece> operator"" _pcs(const unsigned long long amount)
{
    return Quanty<Unit::Piece>{ static_cast<double>(amount) };
}

}

using namespace unit_literals;

void test_unit_literals()
{
    auto k = 3_kg;
    std::cout << "3kg: " << double(k) << "\n";

    auto l = 2.5_l;
    std::cout << "2.5l: " << double(l) << "\n";

    auto p = 12_pcs;
    std::cout << "12 pcs: " << double(p) << "\n";
}

using namespace std::literals;

void test_string_literals()
{
    auto s0 = "Hello, Qt6"s;
    std::cout << typeid(s0).name() << "\n"; //Ic

    auto s1 = L"Hello, Qt6"s;
    std::cout << typeid(s1).name() << "\n"; //Iw

    auto s2 = u"Hello, Qt6"s;
    std::cout << typeid(s2).name() << "\n"; //IDs

    auto s3 = U"Hello, Qt6"s;
    std::cout << typeid(s3).name() << "\n"; //IDi
}

} //namespace =================================================================

void test_literals()
{
    //test_kb();
    //test_unit_literals();
    test_string_literals();
}
