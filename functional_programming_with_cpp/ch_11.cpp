#include <iostream>
#include <random>
#include <algorithm>

namespace { //=================================================================

template <typename Dest>
auto transformAll = [](auto const& source, auto pred) {
    Dest result;
    result.reserve(source.size());
    std::transform(source.begin(), source.end(), std::back_inserter(result), pred);
    return std::move(result);
};

auto print_generated_values = [](const std::string& name, const auto& values) {
    std::cout << "generator(" << name <<")\n";
    for (auto value : values) {
        std::cout << value << ",";
    }
    std::cout << "\n";
};

auto check_property = [](const auto& generator, const auto& property, const std::string& generatorName) {
    auto values = generator();
    print_generated_values(generatorName, values);

    return std::all_of(values.begin(), values.end(), property);
};

namespace _1 {

auto range = [](const int min, const int max) {
    auto v = std::vector<int>(max - min + 1);
    std::iota(v.begin(), v.end(), min);
    return v;
};

auto generate_ints = [](const int min, const int max) {
    auto rd = std::random_device{};
    auto generator = std::mt19937{rd()};
    auto distribution = std::uniform_int_distribution<int>(min, max);
    auto values = transformAll<std::vector<int>>(range(0, 98), [&distribution, &generator](auto){
        return distribution(generator);
    });
    values.push_back(min);
    values.push_back(max);

    return values;
};

auto generate_ints_greater_than_1 = std::bind(generate_ints, 1, std::numeric_limits<int>::max());

auto generate_ints_greater_than_2_less_sqrt_max_int = std::bind(generate_ints, 2, std::sqrt(std::numeric_limits<int>::max()));

auto logMaxIntBaseX = [](const int x) -> int {
    auto maxInt = std::numeric_limits<int>::max();
    return std::floor(std::log(maxInt) / std::log(x));
};

auto generate_exponent_less_than_log_max_int = [](const int x) {
    return generate_ints(1, logMaxIntBaseX(x));
};

auto property_0_to_0_is_1 = []{
    return std::pow(0, 0) == 1;
};

auto property_0_to_x_is_0 = [](const int exp) {
    return std::pow(0, exp) == 0;
};

auto property_x_to_1_is_x = [](const int base) {
    return std::pow(base, 1) == base;
};

auto property_next_power_of_x_is_prev_power_of_x_multiply_x = [](const int x) {
    auto exps = std::bind(generate_exponent_less_than_log_max_int, x);

    return check_property(exps, [x](auto y){
        return std::pow(x, y) == std::pow(x, y-1) * y;
    }, "x^y == x^(y-1)*x");
};

void test_0_to_x_is_0()
{
    auto result = check_property(generate_ints_greater_than_1, property_0_to_x_is_0, "0^x");
    std::cout << "0^x == 0 ? " << result << "\n";
}

void test_x_to_1_is_x()
{
   auto result = check_property(generate_ints_greater_than_1, property_x_to_1_is_x, "x^1");
   std::cout << "x^1 == x ? " << result << "\n";
}

void test_next_power_of_x_is_prev_power_of_x_multiply_x()
{
    auto result = check_property(generate_ints_greater_than_2_less_sqrt_max_int, property_next_power_of_x_is_prev_power_of_x_multiply_x, "...");
    std::cout << "x^y == x^(y-1) * y ? " << result << "\n";
}

void test()
{
    std::cout << "0^0 == 1 ? " << property_0_to_0_is_1() << "\n";

    //test_0_to_x_is_0();
    //test_x_to_1_is_x();
    test_next_power_of_x_is_prev_power_of_x_multiply_x();
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_11()
{
    _1::test();
}
