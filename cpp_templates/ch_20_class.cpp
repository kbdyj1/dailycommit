#include <iostream>
#include <vector>
#include <utility>
#include <map>

#include "traits.h"

namespace { //=================================================================

namespace _1 {

template <typename Key, typename Value, typename = void>
class Dictionary
{
    std::vector<std::pair<const Key, Value>> data;

public:
    Value& operator[](const Key& key) {
        for (auto& elem : data) {
            if (elem.first == key) {
                return elem.second;
            }
        }
        data.push_back(std::pair<const Key, Value>(key, Value()));

        return data.back().second;
    }
};

template <typename T, typename = std::void_t<>>
struct HasLess : std::false_type
{
};

template <typename T>
struct HasLess<T, std::void_t<decltype(std::declval<T>() < std::declval<T>())>> : std::true_type
{
    using Type = T;
};

#if (0) //TODO
template <typename Key, typename Value>
class Dictionary<Key, Value, std::enable_if_t<HasLess<Key>, Key>>
{
    std::map<Key, Value> data;

public:
    Value& operator[](const Key& key) {
        return data[key];
    }
};
#endif

void test()
{
    auto dic = Dictionary<int, std::string>();
    dic[0] = "Hello";
    dic[1] = "Qt";

    std::cout << dic[0] << ", " << dic[1] << "!\n";
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_20_class()
{
    _1::test();
}
