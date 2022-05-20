#include <iostream>
#include <utility>
#include <numbers>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <bit>
#include <bitset>

namespace { //=================================================================

void test1()
{
    int x = -3;
    unsigned int y = 7;

    std::cout << "-3 < 7  : " << (x < y) << std::endl;
    std::cout << "-3 <= 7 : " << (x <= y) << std::endl;
    std::cout << "-3 > 7  : " << (x > y) << std::endl;
    std::cout << "-3 >= 7 : " << (x >= y) << std::endl;
}

void test2()
{
    int x = -3;
    unsigned int y = 7;

#if (0)
    std::cout << "-3 == 7 : " << std::cmp_equal(x, y) << std::endl;
    std::cout << "-3 != 7 : " << std::cmp_not_equal(x, y) << std::endl;
    std::cout << "-3 <  7 : " << std::cmp_less(x, y) << std::endl;
    std::cout << "-3 <= 7 : " << std::cmp_equal(x, y) << std::endl;
    std::cout << "-3 >  7 : " << std::cmp_greater(x, y) << std::endl;
    std::cout << "-3 >= 7 : " << std::cmp_greater_equal(x, y) << std::endl;
#endif
}

void test3()
{
    std::cout << std::setprecision(10);

    std::cout << "std::numbers::e: " << std::numbers::e << std::endl;
    std::cout << "std::numbers::log2e: " << std::numbers::log2e << std::endl;
    std::cout << "std::numbers::log10e: " << std::numbers::log10e << std::endl;
    std::cout << "std::numbers::pi: " << std::numbers::pi << std::endl;
    std::cout << "std::numbers::inv_pi: " << std::numbers::inv_pi << std::endl;
    std::cout << "std::numbers::inv_sqrtpi: " << std::numbers::inv_sqrtpi << std::endl;
    std::cout << "std::numbers::ln2: " << std::numbers::ln2 << std::endl;
    std::cout << "std::numbers::sqrt2: " << std::numbers::sqrt2 << std::endl;
    std::cout << "std::numbers::sqrt3: " << std::numbers::sqrt3 << std::endl;
    std::cout << "std::numbers::inv_sqrt3: " << std::numbers::inv_sqrt3 << std::endl;
    std::cout << "std::numbers::egamma: " << std::numbers::egamma << std::endl;
    std::cout << "std::numbers::phi: " << std::numbers::phi << std::endl;
}

void test4()
{
    std::cout << "std::midpoint(10, 20): " << std::midpoint(10, 20) << std::endl;
    for (auto v : {.0, .1, .2, .3, .4, .5, .6, .7, .8, .9, 1.0}) {
        std::cout << "std::lerp(10, 20, " << v << ") : " << std::lerp(10, 20, v) << std::endl;
    }
}

void test5()
{
    if constexpr (std::endian::native == std::endian::little) {
        std::cout << "little endian" << std::endl;
    } else if constexpr (std::endian::native == std::endian::big) {
        std::cout << "big endian" << std::endl;
    }
}

void test6()
{
    auto n = std::uint8_t{0b00110010};

    std::cout << "std::has_single_bit(00110010) : " << std::has_single_bit(n) << std::endl;
    std::cout << "std::bit_ceil(00110010) : " << std::bitset<8>(std::bit_ceil(n)) << std::endl;
    std::cout << "std::bit_floor(00110010) : " << std::bitset<8>(std::bit_floor(n)) << std::endl;
    std::cout << "std::bit_width(5u) : " << std::bit_width(5u) << std::endl;
    std::cout << "std::rotl(00110010, 2) : " << std::bitset<8>(std::rotl(n, 2)) << std::endl;
    std::cout << "std::rotr(00110010, 2) : " << std::bitset<8>(std::rotr(n, 2)) << std::endl;
    std::cout << "std::countl_zero(00110010) : " << std::countl_zero(n) << std::endl;
    std::cout << "std::countr_zero(00110010) : " << std::countr_zero(n) << std::endl;
    std::cout << "std::countl_one(00110010) : " << std::countl_one(n) << std::endl;
    std::cout << "std::countr_one(00110010) : " << std::countr_one(n) << std::endl;
    std::cout << "std::popcount(00110010) : " << std::popcount(n) << std::endl;
}

void test7()
{
    for (auto i=2u; i<8u; i++) {
        std::cout << "bit_floor(" << std::bitset<8>(i) << ") : " << std::bit_floor(i) << std::endl;
        std::cout << "bit_ceil(" << std::bitset<8>(i) << ") : " << std::bit_ceil(i) << std::endl;
        std::cout << "bit_width(" << std::bitset<8>(i) << ") : " << std::bit_width(i) << std::endl;
        std::cout << "popcount(" << std::bitset<8>(i) << ") : " << std::popcount(i) << std::endl;
    }
}

} // namespace ================================================================

void test_ch4_4()
{
    std::cout << std::endl;
    std::cout << std::boolalpha;

    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
    test7();
}
