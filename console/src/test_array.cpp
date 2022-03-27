#include <array>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace { //=================================================================

void test0()
{
    auto a = std::array<int, 3>{ 1, 2, 3 };

#if (0)
    // -1516206592
    std::cout << a[3] << std::endl;
#else
    // SIGABORT
    //terminate called after throwing an instance of 'std::out_of_range'
    //  what():  array::at: __n (which is 3) >= _Nm (which is 3)
    std::cout << a.at(3) << std::endl;
#endif
}

void test1()
{
    auto a = std::array<int, 3>{ 1, 2, 3 };
    try {
        std::cout << a.at(3) << std::endl;
    }  catch (const std::exception& e) {
        //array::at: __n (which is 3) >= _Nm (which is 3)
        std::cout << e.what() << std::endl;
    }
}

void test2()
{
    auto a = std::array<int, 3>{ 1, 2, 3 };
    std::cout << "front : " << a.front() << std::endl;
    std::cout << "back : " << a.back() << std::endl;

    std::copy(a.begin(), a.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    auto iter0 = a.begin();
    auto iter1 = a.end();

    a = std::array<int, 3>{ 4, 5, 6 };

    std::copy(iter0, iter1, std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    auto b = std::array<int, 3>{ 7, 8, 9 };

    a.swap(b);

    std::copy(iter0, iter1, std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::cout << "size : " << a.size() << std::endl;
    std::cout << "max_size : " << a.max_size() << std::endl;
    std::cout << "empty : " << a.empty() << std::endl;
}

template <typename T>
void test_tuple_size(T)
{
    int a[ std::tuple_size<T>::value ];
    std::cout << "std::tuple_size<" << typeid(a).name() << "> : " << std::size(a) << std::endl;
}
void test3()
{
    int a[] = { 1, 2, 3, 4, 5 };
    auto b = std::to_array(a);
    std::cout << "size : " << b.size() << std::endl;

    test_tuple_size(b);
}

} // namespace ================================================================

void test_array()
{
    //test0();
    //test1();
    //test2();
    test3();
}
