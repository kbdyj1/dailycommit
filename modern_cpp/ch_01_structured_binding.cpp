#include <iostream>
#include <tuple>
#include <map>

namespace { //=================================================================

namespace tuple {

std::tuple<double, char, std::string> getStudents(int id)
{
    switch (id) {
    case 0:
        return std::make_tuple(3.8, 'A', "Lisa");
    case 1:
        return std::make_tuple(2.9, 'C', "Jeny");
    case 2:
        return std::make_tuple(1.7, 'D', "Rose");
    default:
        throw std::invalid_argument("id");
        break;
    }
}

void test()
{
    auto s0 = getStudents(0);
    std::cout << "id: 0\n"
              << "\tGPA: " << std::get<0>(s0) << ", "
              << "grade: " << std::get<1>(s0) << ", "
              << "name: " << std::get<2>(s0) << "\n";

    {
        double gpa;
        char grade;
        std::string name;
        std::tie(gpa, grade, name) = getStudents(1);
        std::cout << "id: 1\n"
                  << "\tGPA: " << gpa << ", "
                  << "grade: " << grade << ", "
                  << "name: " << name << "\n";
    }
    {
        //C++17
        auto [gpa, grade, name] = getStudents(2);
        std::cout << "id: 2\n"
                  << "\tGPA: " << gpa << ", "
                  << "grade: " << grade << ", "
                  << "name: " << name << "\n";
    }
}

} // tuple ----------------------------------------------------------

namespace test_map {

void test()
{
    std::map<int, std::string> m;

    std::cout << std::boolalpha;

    // std::pair<iterator, bool> insert( const value_type& value );

    auto result = m.insert({1, "one"});
    std::cout << "inserted: " << result.second << ", key: " << result.first->first << ", value: " << result.first->second << "\n";

    {
        auto [iter, inserted] = m.insert({2, "two"});
        std::cout << "inserted: " << inserted << ", value: " << iter->second << "\n";
    }
    {
        auto [iter, inserted] = m.insert({2, "duplicated"});
        std::cout << "inserted: " << inserted << ", value: " << iter->second << "\n";
    }
    if (auto [iter, inserted] = m.insert({3, "three"}); inserted) {
        std::cout << "inserted: " << inserted << ", value: " << iter->second << "\n";
    }
}

} // test_map -------------------------------------------------------

} //namespace =================================================================

void test_ch_01_structured_binding()
{
    //tuple::test();
    test_map::test();
}
