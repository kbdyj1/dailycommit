#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

namespace { //=================================================================

namespace _1 {

struct person_t {
    bool famale;
    std::string name;
};

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

} //===========================================================================

void test_ch_07()
{

}
