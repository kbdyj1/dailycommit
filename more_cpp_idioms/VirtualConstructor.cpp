#include <iostream>
#include <memory>

namespace { //=================================================================

class Employee
{
public:
    virtual ~Employee()
    {}
    typedef std::shared_ptr<Employee> Ptr;

    virtual Ptr create() const = 0;
    virtual Ptr clone() const = 0;
};
class Manager : public Employee
{
public:
    Manager()
    {}
    Manager(const Manager&)
    {
    }
    Ptr create() const override { // co-variant return type (not Employee*)
        return Ptr(new Manager());
    }
    Ptr clone() const override {
        return Ptr(new Manager(*this));
    }
};
class Programmer : public Employee {
public:
    Programmer()
    {}
    Ptr duplicate(Employee const& e) {
        return e.clone();
    }
    Ptr create() const override {
        return Ptr(new Programmer());
    }
    Ptr clone() const override {
        return Ptr(new Programmer());
    }
};

} //namespace =================================================================

void test_virtual_constructor()
{
    auto m = Manager();
    auto prg = Programmer();
    auto c = prg.duplicate(m);
}
