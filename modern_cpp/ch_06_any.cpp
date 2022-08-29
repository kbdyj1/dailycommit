#include <iostream>
#include <any>
#include <optional>
#include <string>
#include <vector>
#include <map>

namespace { //=================================================================

using namespace std::literals;

void test_any()
{
    auto printAny = [](const auto& value) {
        std::cout << value << ", type: " << typeid(value).name() << "\n";
    };
    auto value = std::any{42};

    try {
        printAny(std::any_cast<int>(value));

        value = 42.5;
        printAny(std::any_cast<double>(value));

        value = "42"s;
#if (1)
        printAny(std::any_cast<std::string>(value));
#else
        printAny(std::any_cast<double>(value)); // bad any_cast
#endif

    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    auto a = std::any{};
    std::cout << "has_value(): " << a.has_value() << "\n";

    a = 100;
    std::cout << "has_value(): " << a.has_value() << "\n";
    if (a.has_value()) {
        std::cout << "type: " << a.type().name() << ", " << std::any_cast<int>(a) << "\n";
    }
}

void log(const std::any& value)
{
    if (value.type() == typeid(int)) {
        std::cout << "value(int): " << std::any_cast<int>(value) << "\n";
    } else if (value.type() == typeid(double)) {
        std::cout << "value(double): " << std::any_cast<double>(value) << "\n";
    } else if (value.type() == typeid(std::string)) {
        std::cout << "value(string): " << std::any_cast<std::string>(value) << "\n";
    } else {
        std::cout << "value()\n";
    }
}

void test_any_log()
{
    auto values = std::vector<std::any>{};

    values.push_back(std::any{});
    values.push_back(10);
    values.push_back("10.5"s);
    values.push_back(3.14);

    for (auto v : values) {
        log(v);
    }
}

template <typename K, typename V>
std::optional<V> find(const K key, const std::map<K, V>& m) {
    auto iter = m.find(key);
    if (iter != m.end()) {
        return iter->second;
    }
    return {};
}

void test_optional()
{
    auto m = std::map<int, std::string>{
        { 1, "one" },
        { 2, "two" },
        { 3, "three" }
    };

    auto value = find(1, m);
    if (value) {
        std::cout << *value << "\n";
    }
    std::cout << std::boolalpha << "value.has_value(): " << value.has_value() << "\n";

    value = find(4, m);
    std::cout << value.value_or("none") << "\n";
}

} //namespace =================================================================

void test_ch_06_any()
{
#if (0)
    test_any();
    test_any_log();
#endif

    test_optional();
}
