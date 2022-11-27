#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <unordered_map>
#include <map>
#include <cstring>

namespace { //=================================================================

struct person_t {
    std::string name;
    int age;

    void print(std::ostream& os, bool printAge)
    {
        if (printAge) {
            std::cout << "{" << name << ", " << age << "}\n";
        } else {
            std::cout << "{" << name << "}\n";
        }
    }
};

std::vector<person_t> people = { { "Minji", 18}, { "Daniel", 17}, { "Hani", 18}, { "Haerin", 16}, { "Hyein", 14} };

void print_person(std::ostream& os, const person_t& person, bool printAge)
{
    if (printAge) {
        std::cout << "[" << person.name << ", " << person.age << "]\n";
    } else {
        std::cout << "[" << person.name << "]\n";
    }
}

auto vInt = std::vector<int>{ 1, 10, 100, 30, 50, 8, 95, 20, 48, 25 };

namespace _1 {

class GreaterThan {
    int value;

public:
    GreaterThan(int value) : value(value)
    {}
    bool operator()(int arg) const {
        return arg > value;
    }
};

void test()
{
    auto iter = std::partition(vInt.begin(), vInt.end(), GreaterThan(30));

    for (auto i : vInt) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "distance: " << std::distance(vInt.begin(), iter) << "\n";
    for ( ; iter != vInt.end(); iter++) {
        std::cout << *iter << " ";
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename Function, typename SecondArg>
class Bind2ndImpl {
    Function func;
    SecondArg secondArg;

public:
    Bind2ndImpl(Function func, SecondArg arg) : func(func), secondArg(arg)
    {}

    template<typename FirstArg>
    auto operator()(FirstArg&& firstArg) const -> decltype(func(std::forward<FirstArg>(firstArg), secondArg)) {
        return func(std::forward<FirstArg>(firstArg), secondArg);
    }
};

template <typename Function, typename SecondArg>
Bind2ndImpl<Function, SecondArg> bind2nd(Function&& func, SecondArg&& secondArg)
{
    return Bind2ndImpl<Function, SecondArg>(std::forward<Function>(func), std::forward<SecondArg>(secondArg));
}

bool greater_than(int l, int r)
{
    return l > r;
}

void test_greater_than()
{
#if (0)
    Bind2ndImpl greater_than_30(greater_than, 30);
#else
    auto greater_than_30 = bind2nd(greater_than, 30);
#endif

    auto iter = std::partition(vInt.begin(), vInt.end(), greater_than_30);

    for (auto i : vInt) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "distance: " << std::distance(vInt.begin(), iter) << "\n";
    for ( ; iter != vInt.end(); iter++) {
        std::cout << *iter << " ";
    }
}

void test_deg_to_rad()
{
    auto deg_to_rad = bind2nd(std::multiplies<double>(), M_PI/180.0);
    auto v = std::vector<double>{ 15.0, 45.0, 90.0, 180.0, 270.0, 38.0 };
    auto result = std::vector<double>(v.size());
    std::transform(v.begin(), v.end(), result.begin(), deg_to_rad);

    for (auto iter = result.begin(); iter != result.end(); iter++) {
        std::cout << *iter << " ";
    }
    std::cout << "\n";
}

void test()
{
    //test_greater_than();

    test_deg_to_rad();
}

} //_2 --------------------------------------------------------------

namespace _3 { //std::bind

void test()
{
    auto greater_than_40 = std::bind(std::greater<int>(), std::placeholders::_1, 40);
    auto less_than_40 = std::bind(std::greater<int>(), 40, std::placeholders::_1);

    std::cout << " 50 > 40 : " << greater_than_40(50) << "\n";
    std::cout << " 50 < 40 : " << less_than_40(50) << "\n";

    auto v = std::vector<int>{ 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };

    std::sort(v.begin(), v.end(), std::bind(std::greater<int>(), std::placeholders::_2, std::placeholders::_1));
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test()
{
#if (0)
    //function call
    std::for_each(people.begin(),
                  people.end(),
                  std::bind(print_person, std::ref(std::cout), std::placeholders::_1, true));
#else
    //member function call
    std::for_each(people.begin(),
                  people.end(),
                  std::bind(&person_t::print, std::placeholders::_1, std::ref(std::cout), true));
#endif
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test()
{
    auto greater_than_40 = [](int value){
        return std::greater<int>()(value, 40);
    };
    auto less_than_40 = [](int value) {
        return std::greater<int>()(40, value);
    };

    std::cout << "50 > 40 : " << greater_than_40(50) << "\n";
    std::cout << "50 < 40 : " << less_than_40(50) << "\n";

    auto v = std::vector<int>{ 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };

    std::sort(v.begin(), v.end(), [](int l, int r){
        return std::greater<int>()(r, l);
    });
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));

    bool printAge = false;
    std::for_each(people.begin(), people.end(), [printAge](const person_t& person){
        print_person(std::cout, person, printAge);
    });
}

} //_5 --------------------------------------------------------------

namespace _6 { //curried

bool greater(int l, int r)
{
    return l > r;
}

auto greater_than(int l)
{
    return [l](int r) {
        return l > r;
    };
}

auto print_person_curried(const person_t& person)
{
    return [&](std::ostream& os) {
        return [&](bool printAge) {
            print_person(os, person, printAge);
        };
    };
}

void test()
{
    std::cout << "1 > 2 : " << greater(1, 2) << "\n";
    std::cout << "1 > 2 : " << greater_than(1)(2) << "\n";
    std::cout << "1 < 2 : " << greater_than(2)(1) << "\n";

    print_person_curried(people.at(0))
                        (std::cout)
                        (true);
}

} //_6 --------------------------------------------------------------

namespace _7 {

constexpr std::size_t constexpr_strlen(const char* s)
{
    return std::string::traits_type::length(s);
}

void test()
{
#if (0)
    char buffer[ constexpr_strlen("Hello, Qt6") ];
#else
    char buffer[ strlen("Hello, Qt6") ];
#endif
}

} //_7 --------------------------------------------------------------

namespace _8 {

void to_upper(std::string& str)
{
    std::cout << "to_upper(std::string& str)\n";
}

template <typename Function>
auto pointer_lift(Function f)
{
    return [f](auto* item) {
        if (item)
            f(*item);
    };
}

template <typename Function>
auto collection_lift(Function f)
{
    return [f](auto& items) {
        for (auto& item : items) {
            f(item);
        }
    };
}

template <typename C,
          typename P1 = typename std::remove_cv<typename C::value_type::first_type>::type,
          typename P2 = typename C::value_type::second_type>
std::vector<std::pair<P2, P1>> reverse_pairs(const C& items)
{
    std::vector<std::pair<P2, P1>> result(items.size());

    std::transform(std::begin(items), std::end(items), std::begin(items), [](const std::pair<P1, P2>& p) {
        return std::make_pair(p.second, p.first);
    });

    return result;
}

} //_8 --------------------------------------------------------------

} //===========================================================================

void test_ch_04()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
    _5::test();
    _6::test();
#endif

    _7::test();
}
