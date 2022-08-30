#include <iostream>
#include <any>
#include <optional>
#include <variant>
#include <string>
#include <vector>
#include <map>

#include "util.h"

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

struct A {
    int value;

    explicit A(const int v) : value(v)
    {}
};
std::ostream& operator<<(std::ostream& os, const A& a)
{
    os << "A(" << a.value << ")";
    return os;
}

void test_variant()
{
    std::variant<int, std::string, A> v{62};

    std::cout << std::boolalpha;

    PRINT_FUNC(std::get<int>(v));
    PRINT_FUNC(std::get<0>(v));
    PRINT_FUNC(v.index());
    PRINT_FUNC(std::holds_alternative<int>(v));

    v = "62"s;
    PRINT_FUNC(v.index());
    PRINT_FUNC(std::holds_alternative<int>(v));

#if (0) // static_assert
    try {
        auto d = std::get<double>(v);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
#endif

    std::visit([](auto&& value){ std::cout << value << "\n"; }, v);
}

enum class Genre {
    Drama,
    Action,
    SF,
    Commedy
};

struct Movie {
    std::string title;
    std::chrono::minutes length;
    std::vector<Genre> genre;
};

struct Track {
    std::string title;
    std::chrono::seconds length;
};

struct Music {
    std::string title;
    std::string artist;
    std::vector<Track> tracks;
};

struct Software {
    std::string title;
    std::string vendor;
};

using dvd = std::variant<Movie, Music, Software>;

#define USE_DVD_VISITOR

struct DvdVisitor {
    void operator()(const Movie& movie) const {
        std::cout << "Movie(title:" << movie.title << ", len: " << movie.length.count() << "min)\n";
    }
    void operator()(const Music& music) const {
        std::cout << "Music(title: " << music.title << ", artist: " << music.artist << ")\n";
    }
    void operator()(const Software& sw) const {
        std::cout << "Software(title: " << sw.title << ", vendor: " << sw.vendor << ")\n";
    }
};

void test_visit_using_visitor(const dvd& d)
{
    std::visit(DvdVisitor{}, d);
}

void test_toupper(const dvd& d)
{
    dvd result = std::visit([](auto&& arg) -> dvd {
        auto copy{arg};
        std::transform(copy.title.begin(), copy.title.end(), copy.title.begin(), [](char c){
            return std::toupper(c);
        });
        return copy;
    }, d);
    std::visit([](auto&& arg){
        std::cout << arg.title << "\n";
    }, result);
}

void test_visit_using_lambda(const dvd& d)
{
    std::visit([](auto&& arg){
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, Movie>) {
            std::cout << "Movie(title:" << arg.title << ", len: " << arg.length.count() << "min)\n";
        } else if constexpr (std::is_same_v<T, Music>) {
            std::cout << "Music(title: " << arg.title << ", artist: " << arg.artist << ")\n";
        } else if constexpr (std::is_same_v<T, Software>) {
            std::cout << "Software(title: " << arg.title << ", vendor: " << arg.vendor << ")\n";
        }
    }, d);
}

void test_complex_variant()
{
    std::vector<dvd> dvds = {
        Movie { "Starwars", 100min, {Genre::SF, Genre::Action}},
        Music { "New Jeans", "New Jeans", {{"Attention", 179s}, {"Hurt", 177s}, {"Cookie", 235s}, {"Hype Boy", 178s}}},
        Software { "Ubuntu 22.04.1 LTS", "Canonical"}
    };
    for (const auto& d : dvds) {
#if (0)
        test_visit_using_visitor(d);
        test_toupper(d);
#endif
        test_visit_using_lambda(d);
    }
}

} //namespace =================================================================

void test_ch_06_any()
{
#if (0)
    test_any();
    test_any_log();
    test_optional();
    test_variant();
#endif

    test_complex_variant();
}
