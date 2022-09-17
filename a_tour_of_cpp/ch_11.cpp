//=============================================================================
//  A tour of C++
//  Ch.11  Container
//
//  22/09/17
//=============================================================================

#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <unordered_set>

struct Record {
    int id;
    std::string name;
};

std::ostream& operator<<(std::ostream& os, const Record& r) {
    os << "Record{ id: " << r.id << ", name: " << r.name << "}";
    return os;
}

bool operator==(const Record& r0, const Record& r1) {
    return r0.id == r1.id && r0.name == r1.name;
}

namespace std { //=============================================================

template<>
struct hash<Record> {
    using argument_type = Record;
    using result_type = std::size_t;

    size_t operator()(const Record& r) const {
        return hash<string>()(r.name) ^ hash<int>()(r.id);
    }
};

} //std =======================================================================

namespace { //=================================================================

void test_vector_range()
{
    auto const magic = 62;
    auto v = std::vector<int>(8, magic);
    try {

#if (0)
        std::cout << v[v.size()] << "\n";       // 0
#else
        std::cout << v.at(v.size()) << "\n";    // vector::_M_range_check: __n (which is 8) >= this->size() (which is 8)
#endif
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}

void test_record() {
    auto m = std::unordered_set<Record>{
        { 101, "one"},
        { 102, "two"},
        { 103, "three"},
        { 104, "four"},
        { 105, "five"}
    };

    for (auto iter = std::begin(m); iter != std::end(m); ++iter) {
        std::cout << *iter << "\n";
    }
}

} //namespace =================================================================

void test_ch_11()
{
#if (0) // done
    test_vector_range();
#endif

    test_record();
}
