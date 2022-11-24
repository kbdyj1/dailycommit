#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace { //=================================================================

struct person_t {
    bool female;
    std::string name;
    int age;
};

std::vector<person_t> people = { {true, "Minji", 18}, {true, "Daniel", 17}, {true, "Hani", 18}, {true, "Haerin", 16}, {true, "Hyein", 14} };

struct vehicle_t {
    std::string model;
    int age;
};

std::vector<vehicle_t> cars = { {"K9", 10}, {"SM5", 12}, {"Santafe", 15}, {"Sorento", 11}, {"Ionic5", 3}};

struct employee_t {
    std::string name;
    std::string team;
};

namespace _1 {

auto factorial(int n)
{
#if (1)
    if (0 == n)
        return 1;
    else
        return factorial(n-1) * n;
#else
    //error: funcation 'factorial' with deduced return type cannot be used before it is defined
    if (0 < n)
        return factorial(n-1) * n;
    else
        return 1;
#endif
}

template <typename Object, typename Function>
decltype(auto) call_on_object(Object&& object, Function func)
{
    return func(std::forward<Object>(object));
}

} //_1 --------------------------------------------------------------

namespace _2 {

int ask() { return 10; }

typedef decltype(ask)* func_ask;

class ConvertibleToAskFunc {
public:
    operator func_ask() const {
        return ask;
    }
    void operator()(int n) const {
        std::cout << "operator(int: " << n << ")\n";
    }
    void operator()(const std::string& s) {
        std::cout << "operator(string: " << s << ")\n";
    }
};

void test()
{
    auto ask_ptr = &ask;
    std::cout << "ask_ptr(): " << ask_ptr() << "\n";

    auto& ask_ref = ask;
    std::cout << "ask_ref(): " << ask_ref() << "\n";

    auto ask_class = ConvertibleToAskFunc();
    std::cout << "ask_class(): " << ask_class() << "\n";

    ask_class(20);
    ask_class("Hello, Qt6");
}

} //_2 --------------------------------------------------------------

namespace _3 {

class OlderThan {
    int limit;

public:
    OlderThan(int limit) : limit(limit)
    {}
#if (0)
    bool operator()(const person_t& person) const
    {
        return limit < person.age;
    }
#else
    template <typename T>
    bool operator()(T&& obj) const
    {
        std::cout << "sizeof(T): " << sizeof(T) << ", type: " << typeid(T).name() << "\n";
        return std::forward<T>(obj).age > limit;
    }
#endif
};

void test()
{
    {
        const auto limit = 15;
        auto count = std::count_if(people.begin(), people.end(), OlderThan(limit));
        std::cout << "OlderThan(" << limit << "): " << count << "\n";
    }
    {
        const auto limit = 10;

#define USE_LAMBDA

#if !defined(USE_LAMBDA) //functor
        auto predicate = OlderThan(limit);
#else   //lambda
        auto predicate = [limit = 12](auto&& object){
            return object.age > limit;
        };
#endif
        auto count = std::count_if(cars.begin(), cars.end(), predicate);
        std::cout << "cars OlderThan(" << limit << "): " << count << "\n";

        count = std::count_if(people.begin(), people.end(), predicate);
        std::cout << "people OlderThan(" << limit << "): " << count << "\n";
    }
}

} //_3 --------------------------------------------------------------

namespace _4 {

class company_t {
    std::vector<employee_t> employees{{"Minji", "NJ"}, {"Won0", "IVE"}, {"Hani", "NJ"}, {"Rei", "IVE"}, {"Sakura", "LSF"}, {"Haerin", "NJ"}};

public:
    company_t()
    {}

    std::string team_name_for(const employee_t& employee) const {
        return employee.team;
    }
    int count_team_members(const std::string& team) const {
        return std::count_if(employees.begin(), employees.end(), [this, &team](const employee_t& e){
            return team == team_name_for(e);
        });
    }
};

void test()
{
    auto company = company_t{};

    std::cout << "New Jeans: " << company.count_team_members("NJ") << "\n";
    std::cout << "IVE: " << company.count_team_members("IVE") << "\n";
    std::cout << "LE SSERAFIM: " << company.count_team_members("LSF") << "\n";
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test()
{
    int count = 0;
    auto words = std::vector<std::string>{"An", "ancient", "pond", "New jeans"};

    std::for_each(words.cbegin(), words.cend(), [count](const std::string& word) mutable {
        if (isupper(word[0])) {
            std::cout << word << " " << count << "\n";
            ++count;
        }
    });
}

} //_5 --------------------------------------------------------------

namespace _6 {

struct response_t {
    int resCode;

    bool error() const {
        return resCode != 200 && resCode != 204;
    }
};

class ErrorTest {
    bool error;

public:
    ErrorTest(bool error = true) : error(error)
    {}
    template<typename T>
    bool operator()(T&& obj) const
    {
        return error == (bool)std::forward<T>(obj).error();
    }

    // error(true) == true
    // noError(false) == false
    ErrorTest operator==(bool test) {
        return ErrorTest(test ? error : !error);
    }
    ErrorTest operator!() const {
        return ErrorTest(!error);
    }
};

void test()
{
    auto error = ErrorTest(true);
    auto noError = ErrorTest(false);
    auto responses = std::vector<response_t>{ {200}, {400}, {204}, {500}, {100}, {200}, {410}};

    auto failed = std::count_if(responses.begin(), responses.end(), error);
    auto successed = std::count_if(responses.begin(), responses.end(), noError);

    std::cout << "#1 successed: " << successed << ", failed: " << failed << "\n";

    failed = std::count_if(responses.begin(), responses.end(), error == true);
    successed = std::count_if(responses.begin(), responses.end(), error == false);

    std::cout << "#2 successed: " << successed << ", failed: " << failed << "\n";

    failed = std::count_if(responses.begin(), responses.end(), error);
    successed = std::count_if(responses.begin(), responses.end(), !error);

    std::cout << "#3 successed: " << successed << ", failed: " << failed << "\n";
}

} //_6 --------------------------------------------------------------

namespace _7 {

void test()
{
    auto v = std::vector<int>{ 1, 2, 3, 4 };
#if 201103L < __cplusplus
    auto result = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>());  //c++14~ std::multiplies<>()
#else
    auto result = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
#endif

    std::cout << "std::accumulate(..., std::multiplies<int>()): " << result << "\n";
}

} //_7 --------------------------------------------------------------

namespace _8 {

void test()
{
    auto s = std::string{"Qt 6"};
    std::function<bool(std::string)> empty = &std::string::empty;   // link error in clang

    std::cout << "empty(s): " << empty(s) << "\n";
}

} //_8 --------------------------------------------------------------

} //===========================================================================

void test_ch_03()
{
    std::cout << std::boolalpha;

#if (0) //done
    _2::test();
    _3::test();
    _4::test();
    _5::test();
    _6::test();
    _7::test();
#endif

    _8::test();
}
