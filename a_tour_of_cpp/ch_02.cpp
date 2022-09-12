//=============================================================================
//  A tour of C++
//  Ch.02  User defined type
//
//  22/09/12
//=============================================================================

#include <iostream>
#include <variant>

#include "util.h"

namespace { //=================================================================

void test_variant() // prefer union instead
{
    auto v = std::variant<std::monostate, int, float>{};
    std::cout << v.index() << "\n";

    v = 1;
    std::cout << "v = 1\n";
    std::cout << v.index() << "\n";

    try {
        auto w = std::get<float>(v);
        std::cout << "w(float): " << w << "\n";
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }

    PRINT_FUNC(std::holds_alternative<int>(v));
    PRINT_FUNC(std::holds_alternative<float>(v));
    std::cout << std::variant_size<std::variant<std::monostate, int, float>>::value << "\n";
}

enum class Color {
    Black = 0,
    Red,
    Green,
    Blue,
    White
};
std::ostream& operator<<(std::ostream& os, const Color& color) {
    switch (color) {
    case Color::Black: os << "Black";
        break;
    case Color::Red: os << "Red";
        break;
    case Color::Green: os << "Green";
        break;
    case Color::Blue: os << "Blue";
        break;
    case Color::White: os << "White";
        break;
    }
    return os;
}

void test_enum_class()
{
    auto color = Color::White;
    auto iColor = static_cast<int>(color);

    std::cout << "color: " << color << ", iColor: " << iColor << "\n";

    auto badColor = 10;
    color = static_cast<Color>(badColor);

    std::cout << "color: " << color << ", iColor: " << iColor << "\n";
}

} //namespace =================================================================

void test_ch_02()
{
#if (0)
    test_variant();
#endif

    test_enum_class();
}
