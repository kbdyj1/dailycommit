#include <iostream>
#include <unordered_set>
#include <unordered_map>

#include "util.h"

namespace { //=================================================================

struct A {
    int id;
    std::string name;

    A(int id, const std::string& name) : id(id), name(name)
    {}

    bool operator==(const A& other) const {
        return id == other.id && name == other.name;
    }
};
std::ostream& operator<<(std::ostream& os, const A& a)
{
    os << "A(" << a.id << ", " << a.name << ")";
};

} //namespace =================================================================

#define PRINT_HASH_VALUE

namespace std {

template <>
struct hash<A> {
    typedef A argument_type;
    typedef size_t result_type;

    // no exception
    result_type operator()(const argument_type& item) const {
        result_type hashValue = 17;
        hashValue = 31 * hashValue + std::hash<int>{}(item.id);
        hashValue = 31 * hashValue + std::hash<std::string>{}(item.name);

#ifdef PRINT_HASH_VALUE
        std::cout << "A(" << item.id << ", " << item.name << "): " << hashValue << "\n";
#endif

        return hashValue;
    }
};

}

void test_ch_06_hash()
{
    auto s = std::unordered_set<A>{
        { 0, "zero" },
        { 1, "one"},
        { 2, "two" }
    };

    print(s);
}
