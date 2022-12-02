#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

namespace { //=================================================================

struct person_t {
    bool famale;
    std::string name;
};

namespace _1 {

std::string name(const person_t& person)
{
    return person.name;
}

template <typename Persons, typename F>
void group_by_team(Persons& persons, F func, const std::vector<std::string>& teams)
{
    auto b = std::begin(persons);
    const auto e = std::end(persons);

    for (const auto& team : teams) {
        b = std::partition(b, e, [&](const auto& person){
            return team == func(person);
        });
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    auto v = std::vector<person_t>{ {true, "Hyerin"}, {false, "Wonbin"} };
    auto result = std::vector<std::string>(v.size());
    std::transform(v.begin(), v.end(), result.begin(), [](const person_t& person) -> std::string {
        std::string ret = person.famale ? "famale_" : "man_";
        ret += person.name;
        return ret;
    });
    std::copy(result.begin(), result.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_ch_07()
{
    _2::test();
}
