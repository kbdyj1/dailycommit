//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.09 use template
//
//  22/09/28
//=============================================================================
#include <iostream>
#include <unordered_set>

namespace { //=================================================================

class Customer {
    std::string name;

public:
    Customer(const std::string& s) : name(s)
    {}
    std::string getName() const {
        return name;
    }
};

void test_hash()
{
    struct CustomerHash {
        std::size_t operator()(const Customer& c) {
            return std::hash<std::string>()(c.getName());
        }
    };

    auto c = std::unordered_set<Customer, CustomerHash>{};
}

} //namespace =================================================================

void test_ch_09()
{
    test_hash();
}
