#include <iostream>
#include <vector>
#include <numeric>
#include <execution>

namespace { //=================================================================

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

auto test_people = std::vector<person_t>{ {true, "Jane"}, {false, "Robert"}, {true, "Minji"}, {true, "Hani"}, {false, "Jason"} };

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

void test_remove(std::vector<person_t>& v)
{
    v.erase(std::remove_if(v.begin(), v.end(), is_not_female), v.end());
}

std::vector<person_t> test_copy(std::vector<person_t>& v)
{
    auto females = std::vector<person_t>{};

#if (0)
    std::copy_if(v.begin(), v.end(), std::back_inserter(females), is_female);
#else
    std::copy_if(v.begin(), v.end(), std::back_inserter(females), [](const person_t& person){
        return person.female == true;
    });
#endif

    return females;
}

void test()
{
    auto v = test_people;

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

namespace _8 {

template <typename FilterFunction>
std::vector<std::string> names_for(const std::vector<person_t>& people, FilterFunction filter)
{
    std::vector<std::string> result;

    for (const auto& person : people) {
        if (filter(person)) {
            result.push_back(name(person));
        }
    }
    return result;
}

template <typename FilterFunction, typename Iterator>
std::vector<std::string> names_for_helper(Iterator b, Iterator e, FilterFunction filter, std::vector<std::string> prev)
{
    if (b == e) {
        return prev;
    } else {
        const auto head = *b;
        if (filter(head)) {
            prev.push_back(name(head));
        }

        return names_for_helper(b+1, e, filter, std::move(prev));
    }
}

template <typename FilterFunction, typename Iterator>
std::vector<std::string> names_for(Iterator b, Iterator e, FilterFunction filter)
{
    std::vector<std::string> result;

    return names_for_helper(b, e, filter, result);
}

bool odd_len(const person_t& person)
{
    return person.name.length() % 2 != 0;
}

void test()
{
#if (0)
    auto res = names_for(test_people, odd_len);
#else
    auto res = names_for(test_people.begin(), test_people.end(), odd_len);
#endif

    for (const auto& s : res) {
        std::cout << s << '\n';
    }
}

} //_8 --------------------------------------------------------------

namespace _9 {

bool filter(const person_t& person)
{
    return person.name.length() % 2 != 0;
}

std::vector<std::string> append_name_if(std::vector<std::string> prev, const person_t& person)
{
    if (filter(person)) {
        prev.push_back(name(person));
    }
    return prev;
}

void test()
{
    auto res = std::accumulate(test_people.begin(), test_people.end(), std::vector<std::string>{}, append_name_if);
    for (const auto& person : res) {
        std::cout << person << "\n";
    }
}

} //_9 --------------------------------------------------------------

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
    _7::test();
    _8::test();
#endif

    _9::test();
}
