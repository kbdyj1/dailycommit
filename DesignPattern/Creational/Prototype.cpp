#include <iostream>

namespace { //=================================================================

template <typename T>
struct Cloneable
{
    virtual T clone() const = 0;
};

struct Address {
    std::string street;
    std::string city;
    int suite;
};
struct Contact {
    std::string name;
    Address address;
};
struct Contact2 : public Cloneable<Contact2> {
    std::string name;
    Address* address;

    Contact2(std::string name, Address* addr)
        : name(name)
        , address(addr)
    {}
    virtual ~Contact2()
    {
        delete address;
    }

    Contact2 clone() const
    {
        return Contact2{ name, new Address{*address}};
    }
};

void test_simple()
{
    auto worker = Contact {"", {"123 East Dr", "London", 0}};
    auto john = worker;
    john.name = "John Doe";
    john.address.suite = 10;
}
void test_cloneable()
{
    auto worker = Contact2 {"", new Address{"123 East Dr", "London", 0}};
    auto john = worker.clone();
    john.name = "John Doe";
    john.address->suite = 10;
}

} //namespace =================================================================

