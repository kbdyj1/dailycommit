#include <iostream>
#include <vector>

namespace { //=================================================================

namespace _1 {

struct Employee
{
    int level;
    std::string name;
};

void printEmployee(std::ostream& os, const Employee& employee, int salary)
{
    os << "level: " << employee.level << ", name: " << employee.name << ", salary: " << salary << "\n";
}

void test()
{
    auto employees = std::vector<Employee>{
        { 0, "haerin" },
        { 1, "hani"}
    };

    for (const auto& employee : employees) {
        printEmployee(std::cout, employee, 1000);
    }
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_12()
{
    _1::test();
}
