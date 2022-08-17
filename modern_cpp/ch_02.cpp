#include <iostream>
#include <string>
#include <limits>
#include <bitset>

namespace { //=================================================================

namespace number_string_conversion {

void test()
{
    std::cout << std::to_string(42) << "\n";
    std::cout << std::to_string(42l) << "\n";
    std::cout << std::to_string(42u) << "\n";
    std::cout << std::to_string(42.0) << "\n";
    std::cout << std::to_string(42.0l) << "\n";

    std::cout << std::stoi("42") << "\n";
    std::cout << std::stoi("101010", nullptr, 2) << "\n";
    std::cout << std::stoi("052", nullptr, 8) << "\n";
    std::cout << std::stoi("0x1f", nullptr, 16) << "\n";

    std::cout << std::stof("34.6") << "\n";
    std::cout << std::stod("1.23e+2") << "\n";
    std::cout << std::stold("0xf.6e666p3") << "\n";

    size_t index;
    std::string s = "    43.2#00 ";
    auto num = std::stoi(s, &index);
    std::cout << "(" << s << "), result: " << num << ", index: " << index << "\n";

    try {
#if (0)
        // invalid argument
        auto i = std::stoi("");
#else
        // out of range
        auto i = std::stol("12345678901234567890");
#endif
        std::cout << i << "\n";
    } catch (const std::exception& e) {
        std::cout << "exception: " << e.what() << "\n";
    }
}

} //number_string_conversion ----------------------------------------

namespace numeric_limits {

int getIndex() {
    return 0;
}

void test()
{
    int table[std::numeric_limits<char>::max()+1] = {0};

    switch (getIndex()) {
    case std::numeric_limits<unsigned int>::min():
        std::cout << "min\n";
        break;
    }

    auto n = 42u;
    std::bitset<std::numeric_limits<decltype (n)>::digits> bits{
        static_cast<unsigned long long>(n)
    };
    std::cout << "decltype(n): " << typeid(decltype (n)).name() << "\n";
    std::cout << "std::numeric_limits<decltype (n)>::digits: " << std::numeric_limits<decltype (n)>::digits << "\n";
    std::cout << "bits: " << bits << "\n";
}

} //numeric_limits --------------------------------------------------

} //===========================================================================

void test_ch_02()
{
    //number_string_conversion::test();
    numeric_limits::test();
}
