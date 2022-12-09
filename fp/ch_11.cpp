#include <iostream>
#include <vector>
#include <numeric>
#include <tuple>
#include <functional>
#include <string>

using namespace std::string_literals;

namespace { //=================================================================

namespace _1 {

#define REMOVE_QUALIFIER

template <typename T>
using contained_type_t =
#if defined(REMOVE_QUALIFIER)
        std::remove_cv_t<
        std::remove_reference_t<
#endif
            decltype(*std::begin(T()))
#if defined(REMOVE_QUALIFIER)
        >>
#endif
        ;

template <typename C, typename R = contained_type_t<C>>
R sum(const C& collection)
{
#if (0)
    auto ret = R{};
    for (auto iter = collection.begin(); iter != collection.end(); iter++) {
        ret += *iter;
    }
    return ret;
#else
    return std::accumulate(std::begin(collection), std::end(collection), R{});
#endif
}

#if (0)
template <typename T>
class Error;

//24:50: error: invalid declarator before ‘(’ token
//   24 | Error<contained_type_t<std::vector<std::string>>>();
//      |                                                  ^
Error<contained_type_t<std::vector<std::string>>>();
#else
//41:86: error: static assertion failed
//   41 | static_assert(std::is_same<contained_type_t<std::vector<std::string>>, std::string>::value);
//      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~
//In instantiation of ‘R {anonymous}::_1::sum(const C&) [with C = std::vector<int>; R = const int&]’:
static_assert(std::is_same<contained_type_t<std::vector<std::string>>, std::string>::value);
#endif

void test()
{
    auto vec = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto result = sum(vec);
    std::cout << "sum(vec): " << result << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T1, typename T2>
struct is_same : std::false_type
{};

template <typename T>
struct is_same<T, T> : std::true_type
{};

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

void test()
{
    std::cout << "is_same<int, int>: " << is_same<int, int>::value << "\n";
    std::cout << "is_same<int, double>: " << is_same<int, double>::value << "\n";

    std::cout << is_same<int, remove_reference_t<int&>>::value << "\n";
    std::cout << is_same<int, remove_reference_t<int&&>>::value << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T, typename E>
class Expected
{
public:
    using value_type = T;
};

template <typename...>
using void_t = void;

template <typename C, typename = void_t<>>
struct has_value_type : std::false_type
{};

template <typename C>
struct has_value_type<C, void_t<typename C::value_type>> : std::true_type
{};

template <typename C, typename = void_t<>>
struct is_iterable : std::false_type
{};

template <typename C>
struct is_iterable<C, void_t<decltype(*std::begin(std::declval<C>())),
                             decltype(*std::end(std::declval<C>()))>
    >
    : std::true_type
{};

} //_3 --------------------------------------------------------------

namespace _4 {

template <typename Func, typename... CapturedArgs>
class Curried
{
    using CapturedArgsTuple = std::tuple<std::decay_t<CapturedArgs>...>;

    template<typename... Args>
    static auto captured_by_copy(Args&&... args)
    {
        return std::tuple<std::decay_t<Args>...>(std::forward<Args>(args)...);
    }

    Func func;
    std::tuple<CapturedArgs...> args;

public:
    Curried(Func func, CapturedArgs... args) : func(func), args(std::move(args...))
    {}

    Curried(Func func, std::tuple<CapturedArgs...> args) : func(func), args(std::move(args))
    {}

    template<typename... NewArgs>
    auto operator()(NewArgs&&... args) const
    {
        auto new_args = captured_by_copy(std::forward<NewArgs>(args)...);

        auto all_args = std::tuple_cat(args..., std::move(new_args));

        if constexpr (std::is_invocable_v<Func, NewArgs...>) {
            return std::apply(func, all_args);
        } else {
            return Curried<Func, CapturedArgs..., NewArgs...>(func, all_args);
        }
    }
};

} //_4 --------------------------------------------------------------

namespace _5 {

template <typename Member, typename Value>
struct Update {
    Update(Member member, Value value) : member(member), value(value)
    {}

    template<typename Record>
    bool operator()(Record& record)
    {
        if constexpr (std::is_invocable_r<bool, Member, Record, Value>()) {
            std::cout << "return std::invoke(member, record, value);\n";
            return std::invoke(member, record, value);
        } else if constexpr (std::is_invocable<Member, Record, Value>()) {
            std::invoke(member, record, value);
            std::cout << "std::invoke(member, record, value);\n";
            return true;
        } else {
            std::invoke(member, record) = value;
            std::cout << "std::invoke(member, record) = value;\n";
            return true;
        }
    }

    Member member;
    Value value;
};

template <typename Member>
struct Field {
    Field(Member member) : member(member)
    {}

    template<typename Value>
    Update<Member, Value> operator=(const Value& value) const
    {
        return Update{member, value};
    }

    Member member;
};

template <typename Record>
class Transaction {
    Record& record;

    template<typename... Updates>
    bool all(Updates... results) const
    {
        return (... && results);
    }

public:
    Transaction(Record& record) : record(record)
    {}

    template<typename... Updates>
    bool operator()(Updates... updates)
    {
        auto temp = record;
        if (all(updates(temp)...)) {
            std::swap(record, temp);
            return true;
        }
        return false;
    }
};

template <typename Record>
auto with(Record& record)
{
    return Transaction(record);
}

namespace Person
{
    struct Record {
        std::string name;
        int age;
    };
    auto name = Field{&Record::name};
    auto age = Field{&Record::age};
};

void test()
{
    using namespace Person;

    auto person = Record{"Minji", 19};

    with(person)(
        name = "Hani"s, // Field::operator=() -> Update -> Update(person) -> invoke(member, person) = value
        age = 18
    );

    std::cout << person.name << ", " << person.age << "\n";
}

} //_5 --------------------------------------------------------------

namespace _6 {

int add(int first, int second)
{
    return first + second;
}

template <typename T>
T add_generic(T first, T second)
{
    return first + second;
}

auto add_lambda = [](auto first, auto second) { return first + second; };

template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& args)
{
    std::apply([&os](const Ts&... tupleArgs){
        os << '[';
        std::size_t n{0};
        ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
        os << ']';
    }, args);
}

void test()
{
    std::cout << std::apply(add, std::pair(1, 2)) << "\n";
    std::cout << std::apply(add_lambda, std::pair(1.0f, 2.0f)) << "\n";
    std::tuple myTuple{25, "Hello,Qt6", 3.4f, 'Q'};
    std::cout << myTuple << "\n";
}

} //_6 --------------------------------------------------------------

} //===========================================================================

void test_ch_11()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _2::test();
    _5::test();
#endif

    _6::test();
}
