#include <iostream>
#include <span>
#include <vector>
#include <algorithm>
#include <numeric>

namespace { //=================================================================

void print(std::span<int> container)
{
    std::cout << "container.size() : " << container.size() << std::endl;
    for (auto elem : container) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void test1()
{
    auto v1 = std::vector{1, 2, 3, 4, 5};
    auto v2 = std::vector{6, 7, 8, 9};

    auto dspan = std::span<int>{v1};
    auto sspan = std::span<int, 4>{v2};

    print(dspan);
    print(sspan);

    //sspan = dspan;    // error
    dspan = sspan;

    print(sspan);
}

void test2()
{
    std::cout << "int array[] = {1, 2, 3, 4}" << std::endl;
    int array[] = {1, 2, 3, 4};
    print(array);

    std::cout << "std::vector{1, 2, 3, 4, 5}" << std::endl;
    auto vec = std::vector{1, 2, 3, 4, 5};
    print(vec);

    std::cout << "std::array{1, 2, 3, 4}" << std::endl;
    auto array2 = std::array{1, 2, 3, 4};
    print(array2);
}

void test3()
{
    auto vec = std::vector{1, 2, 3, 4, 5};

    auto span1 = std::span{vec};
    auto span2 = std::span{vec.data(), vec.size()};

    auto equal = std::equal(span1.begin(), span1.end(), span2.begin(), span2.end());

    std::cout << "span1 == span2 : " << equal << std::endl;
}

void test4()
{
    auto vec = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print(vec);

    auto span1 = std::span{vec};
    auto span2 = std::span{span1.subspan(1, span1.size()-2)};

    std::cout << "subspan(1, size()-2)" << std::endl;
    print(span2);

    std::transform(span2.begin(), span2.end(), span2.begin(), [](int i){ return i * i; });
    std::cout << "transform([](int){ return i*i; }" << std::endl;
    print(span2);
}

void test5()
{
#if (0)
    auto vec = std::vector<int>{20};
    std::itoa(vec.begin(), vec.end(), 0);
#else
    auto vec = std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
#endif
    print(vec);

    std::cout << std::endl;
    std::cout << std::endl;

    auto s = std::span{vec};
    auto len = s.size();
    auto count = std::size_t{5};
    for (auto first = std::size_t{0}; first <= (len - count); first += count) {
        for (auto elem : s.subspan(first, count)) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

void test6()
{
    auto v0 = std::vector{1, 2, 3, 4, 5};
    auto v1 = v0;
    v1[2] = 10;
    v1.push_back(6);
    print(v1);

    const std::vector<int> v2 = v0;
    //v2[2] = 10;
    //v2.push_back(6);

    auto cspan = std::span<const int>{v0};
    //cspan[2] = 2;

    auto dspan = std::span<int>{v0};
    dspan[2] = 10;
    //dspan.push_back(6);
    print(dspan);
}

} // namespace ================================================================

void test_ch4_2()
{
    std::cout << std::endl;
    std::cout << std::boolalpha;

    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    test6();
}
