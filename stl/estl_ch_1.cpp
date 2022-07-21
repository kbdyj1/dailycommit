#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>

// 1. vector, string, deque
// 2. list (balanced tree)

namespace { //=================================================================

template <typename C>
void print_all(const C& c, const std::string& desc)
{
    std::cout << desc << "\n";

    for (auto i : c) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

void test_container_feature()
{
    constexpr int SIZE = 16;
    {
        auto v = std::vector<int>{0, 2, 1};
        print_all(v, "vector{0, 2, 1}");
        auto iter = v.begin();
        iter = v.erase(iter);
        std::cout << "v.erase(iter) : " << *iter << "\n";
        auto ref = v.front();
        std::cout << "v.front() : " << ref << "\n";

        v.reserve(SIZE);
        std::cout << "v.capacity() : " << v.capacity() << "\n";
    }

    {
        auto l = std::list<int>{0, 2, 1};
        print_all(l, "list{0, 2, 1}");
    }

    {
        auto s = std::set<int>{0, 2, 1};
        print_all(s, "set{0, 2, 1}");
        std::cout << "s.count(1): " << s.count(1) << "\n";
        auto iter = s.lower_bound(1);
        std::cout << "s.lower_bound(1) : " << *iter << "\n";
        iter = s.upper_bound(1);
        std::cout << "s.upper_bound(1) : " << *iter << "\n";
    }
    {
        auto m = std::map<int, std::string>{{0, "zero"}, {1, "one"}, {2, "two"}};
        // 0 or 1
        std::cout << "m.count(3) : " << m.count(3) << "\n";
        std::cout << m[0] << "\n";

        auto mm = std::multimap<int, std::string>{{0, "zero"}, {0, "void"}, {1, "one"}, {1, "solo"}, {2, "two"}, {2, "duo"}};
        // 0~
        std::cout << "m.count(0) : " << mm.count(0) << "\n";
        for (auto iter = mm.begin(); iter != mm.end(); iter++) {
            std::cout << "(" << iter->first << ", " << iter->second << ") ";
        }
    }
}

} // namespace ================================================================

void test_estl_ch_1()
{
    test_container_feature();
}
