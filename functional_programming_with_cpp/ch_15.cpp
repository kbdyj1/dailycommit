#include <iostream>
#include <functional>
#include <iterator>
#include <numeric>

namespace { //=================================================================

namespace _1 {

void freeFunc()
{
    std::cout << "Hello, Qt6\n";
}

class A
{
public:
    int member() const
    {
        return 1;
    }
};

void test()
{
    auto f = std::function<void()>(freeFunc);

    f();

    auto mf = std::function<int(const A&)>(&A::member);
    auto a = A{};

    std::cout << "A::member(): " << mf(a) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

// ariithmetic : plus, minus, multiplies, divides, modules, negate
// comparision : equal_to, not_equal_to, greater, less, greater_equal, less_equal
// logical_and, logical_or, logical_not
// bit_and, bit_or, bit_xor

void test()
{
    auto v = std::vector<int>{ 3, 8, 4, 1, 9, 16, 2, 20 };

    std::sort(v.begin(), v.end(), std::greater<int>());

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

} //_2 --------------------------------------------------------------

namespace _3 {

auto add = [](int l, int r) {
    std::cout << l << " + " << r << "\n";
    return l + r;
};

void test()
{
    auto increment = std::bind(add, 1, std::placeholders::_1);//, 1);

    std::cout << "increment(2) : " << increment(2) << "\n";
}

} //_3 --------------------------------------------------------------

namespace _4 {

struct Person {
    int age;
    std::string name;
};
std::ostream& operator<<(std::ostream& os, const Person& person) {
    os << "Person{ age: " << person.age << ", name: " << person.name << " }\n";
    return os;
};

auto peopleToJson = [](const Person& person) {
    std::string ret;
    {
        ret += "people{age: ";
        ret += std::to_string(person.age);
        ret += ", name: ";
        ret += person.name;
        ret += "}";
    }
    return ret;
};

void test()
{
    auto persons = std::vector<Person>{
        { 18, "Minji" },
        { 18, "Hani" },
        { 17, "Daaniel" },
        { 16, "Haerin" },
        { 14, "Hyein" }
    };

    auto json = std::vector<std::string>(persons.size());
    std::transform(persons.begin(), persons.end(), std::back_inserter(json), peopleToJson);
    auto personJson = std::string{"people: {"} + std::accumulate(json.begin(), json.end(), std::string(), [](const std::string& l, const std::string& r){
        return l.empty() ? r : (l + ", " + r);
    }) + "}";

    std::cout << personJson << std::endl;

    auto sisters = std::vector<Person>{};
    std::copy_if(persons.begin(), persons.end(), std::back_inserter(sisters), [](const Person& person){
        return 17 < person.age;
    });

    std::cout <<"\nNew Jeans sister's\n";
    std::copy(sisters.begin(), sisters.end(), std::ostream_iterator<Person>(std::cout, ""));
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15()
{
#if (0)
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
