#include <iostream>
#include <vector>
#include <numeric>
#include <execution>

namespace { //=================================================================

namespace _1 {

double average_score(const std::vector<int>& scores)
{
    int sum = 0;
    for (auto score : scores) {
        sum += score;
    }
    return sum / (double)scores.size();
}

} //_1 --------------------------------------------------------------

namespace _2 {

double average_score(const std::vector<int>& scores)
{
    return std::accumulate(scores.cbegin(), scores.cend(), 0) / (double)scores.size();
}

} //_2 --------------------------------------------------------------

namespace _3 {

double average_score(const std::vector<int>& scores)
{
    return std::reduce(std::execution::par,
                       scores.cbegin(),
                       scores.cend(),
                       0)
            / (double)scores.size();
}

} //_3 --------------------------------------------------------------

namespace _4 {

double scores_product(const std::vector<int>& scores)
{
    return std::accumulate(scores.cbegin(), scores.cend(), 1, std::multiplies<int>{});
}

} //_4 --------------------------------------------------------------

namespace _5 {

int f(int prev, char c)
{
    return (c == '\n') ? prev+1 : prev;
}

int count_lines(const std::string& s)
{
    return std::accumulate(s.cbegin(), s.cend(), 0, f);
}

void test()
{
    auto s = std::string{"Hello, Qt6!\nFunctional Programming\nHacking Connected Car.\n"};
    std::cout << count_lines(s) << '\n';
}

} //_5 --------------------------------------------------------------

namespace _6 {

bool is_not_space(char c)
{
    return c != ' ' || c != '\n';
}

std::string trim_left(std::string s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), is_not_space));

    return s;
}

void test()
{
    auto s = std::string{"Hello, Qt6!\nFunctional Programming\nHacking Connected Car.\n"};
    std::cout << trim_left(s) << '\n';
}

} //_6 --------------------------------------------------------------

namespace _7 {

struct person_t {
    bool female;
    std::string name;
};


bool is_female(const person_t& person)
{
    return person.female;
}

bool is_not_female(const person_t& person)
{
    return !is_female(person);
}

std::string name(const person_t& person)
{
    return person.name;
}


void test_remove(std::vector<person_t>& v)
{
    v.erase(std::remove_if(v.begin(), v.end(), is_not_female), v.end());
}

std::vector<person_t> test_copy(std::vector<person_t>& v)
{
    auto females = std::vector<person_t>{};

    std::copy_if(v.begin(), v.end(), std::back_inserter(females), is_female);

    return females;
}

void test()
{
    auto v = std::vector<person_t>{ {true, "Jane"}, {false, "Robert"}, {true, "Minji"}, {true, "Hani"}, {false, "Jason"} };

#if (0)
    test_remove(v);
    auto result = v;
#else
    auto result = test_copy(v);
#endif

//    for (auto& person : result) {
//        std::cout << "{ female: " << person.female << ", name: " << person.name << "}\n";
//    }

    std::vector<std::string> names(result.size());
    std::transform(result.begin(), result.end(), names.begin(), name);

    for (const auto& name : names) {
        std::cout << name << '\n';
    }
}

} //_7 --------------------------------------------------------------

void test()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    //std::cout << _3::average_score(v) << '\n';
    std::cout << _4::scores_product(v);
}

} //===========================================================================

void test_ch_02()
{
#if (0)
    test();
    _5::test();
    _6::test();
#endif

    _7::test();
}
