#include <variant>
#include <any>
#include <iostream>
#include <QDebug>

namespace internal
{

/******************************************************************************
 * fold expression
 *
 */
template <typename... Args>
bool all(Args... args)
{
    return (... && args);
}

template <typename... Args>
int sum(Args&&... args)
{
    return (args + ... + (1 * 2));
}

template <typename... Args>
void printer(Args&&... args)
{
    (std::cout << ... << args) << std::endl;
}

template  <typename T, typename... Args>
void push_back_vec(std::vector<T>& v, Args&&... args)
{
    static_assert((std::is_constructible_v<T, Args&&> && ...));
    (v.push_back(std::forward<Args>(args)), ...);
}

template <class T, std::size_t... N>
constexpr T bswap_impl(T i, std::index_sequence<N...>)
{
    return (((i >> N*CHAR_BIT & std::uint8_t(-1)) << (sizeof(T)-1-N)*CHAR_BIT) | ...);
}

template <class T, class U = std::make_unsigned_t<T>>
constexpr U bswap(T i)
{
    return bswap_impl<U>(i, std::make_index_sequence<sizeof(T)>{});
}

void test_fold_expression()
{
    auto b = all(true, true, true, false);

    // (((true & true) & true) & false)
    qDebug() << "all(true, true, true, false) -> " << b;

    // (1 + (2 + (3 + (4 + 5 + (1 * 2))))) = 17
    qDebug() << "sum(1, 2, 3, 4, 5)" << sum(1, 2, 3, 4, 5);

    printer(1, 2, 3.14, "hello, Qt");

    auto v = std::vector<int>{};
    push_back_vec(v, 1, 2, 3, 4);

    qDebug() << (bswap<std::uint16_t>(0x1234u) == 0x3412u);
}

/******************************************************************************
 * variant
 *
 */
void test_variant()
{
    auto v = std::variant<int, float>{};
    auto w = std::variant<int, float>{};
    v = 42;
    auto i = std::get<int>(v);

    qDebug() << "i:" << i;

    w = std::get<0>(v);

    qDebug() << "w:" << std::get<int>(w);

    try {
        std::get<float>(w);
    } catch (const std::bad_variant_access &e) {
        qDebug() << e.what();
    }

    using namespace std::literals;

    auto x = std::variant<std::string>{"abc"};
    x = "def";

    auto y = std::variant<std::string, void const*>{"abc"};
    qDebug() << "{abc}";
    qDebug() << "std::holds_alternative<std::string>:" << std::holds_alternative<std::string>(y);
    qDebug() << "std::holds_alternative<void const*>:" << std::holds_alternative<void const*>(y);
    qDebug() << "index:" << y.index();

    y = "xyz"s;
    assert(std::holds_alternative<std::string>(y));
    qDebug() << "{xyz}s";
    qDebug() << "index:" << y.index();

    using var_t = std::variant<int, double, std::string>;

    auto vars = std::vector<var_t>{ 10, 3.14, "hello, Qt"s };

    for (auto &i : vars) {
        std::visit([](auto&& arg){
            using T = std::decay_t<decltype(arg)>;

            if constexpr(std::is_same_v<T, int>) {
                qDebug() << "type: int";
            } else if constexpr(std::is_same_v<T, double>) {
                qDebug() << "type: double";
            } else if constexpr(std::is_same_v<T, std::string>) {
                qDebug() << "type: std::string";
            }
        }, i);
    }
}

/******************************************************************************
 * any
 *
 */
void test_any()
{
    auto a = std::any{ 1 };
    qDebug() << a.type().name() << ":" << std::any_cast<int>(a);

    int *pi = std::any_cast<int>(&a);
    qDebug() << "int *:" << pi;

    // std::any_cast<float> -> terminate called after throwing an instance of 'std::bad_any_cast'
    a = 3.14;
    qDebug() << a.type().name() << ":" << std::any_cast<double>(a);

    a = true;
    qDebug() << a.type().name() << ":" << std::any_cast<bool>(a);

    qDebug() << "a.hasValue():" << a.has_value();

    a.reset();
    qDebug() << "after reset -> a.hasValue():" << a.has_value();
}

}

void test_cxx17_features()
{
    //internal::test_variant();
    //internal::test_any();
    internal::test_fold_expression();
}
