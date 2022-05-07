#include <iostream>
#include <set>
#include <vector>
#include <iterator>

namespace { //=================================================================

// ES.10 Declare one name (only) per declaration

// ES.11 Use auto to avoid redundant repetition of type names

//template <class T>
//auto Container<T>::first() -> Iterator;

void test_es_11_exception()
{
    auto l = { 1, 2, 3 }; // std::initialize_list
    auto x{1};  // c++11 -> std::initialize_list
                // c++17 -> int

    std::cout << typeid(l).name() << std::endl; //St16initializer_listIiE
    std::cout << typeid(x).name() << std::endl; //i
}

// ES.12 Do not reuse names in nested scopes

// ES.20 Always initialize an object

// ES.21 Don't introduce a variable (or constant) before you need to use it

// ES.22 Don't declare a variable until you have a value to initialize it with
//       Initialization is often more efficient than assignment

// ES.23 Prefer {} - initializer syntax
//       Avoid () initialization, which allows parsing ambiguities.
//       {} - does not allow norrowing

void test_es_23_exception()
{
    auto v0 = std::vector<int>(10); // size : 10
    auto v1 = std::vector<int>{10}; // size : 1
    auto v2 = std::vector<int>(5, 1);
    auto v3 = std::vector<int>{5, 1};

    auto printVec = [](const std::vector<int>& v) {
        std::for_each(std::begin(v), std::end(v), [](int value){
            std::cout << value << " ";
        });
        std::cout << std::endl;
    };

    printVec(v0);
    printVec(v1);
    printVec(v2);
    printVec(v3);
}

struct S
{
    int m {0}; // default initializer
};

struct Z
{
    explicit Z()
    {}
};

template <typename T>
void f()
{
    T t0(0);
    T t1();     // bad. function declaration
    T t2{0};
    T t3{};     // default initialized T
}

// ES.24 Use a unique_ptr<T> to hold pointers

// ES.25 Declare an object const or constexpr unless you want to modify its value later on

} // namespace ================================================================

void test_es_11()
{
    test_es_11_exception();

    auto values = std::set<int>();
    auto [position, inserted] = values.insert(6);
    std::cout << "[" << *position << ", inserted:" << inserted << "]" << std::endl;
}

void test_es_23()
{
    test_es_23_exception();

#if (0)
//    74:14: error: narrowing conversion of ‘6.2999999999999998e+0’ from ‘double’ to ‘int’ [-Wnarrowing]
//       74 |     int x{6.3};
//          |              ^
    int x{6.3};
#endif

    auto x0 {6};        //int
    auto x1 = {6};      //initialize_list
//  auto x2 {6, 2};     //direct-list-initialization of ‘auto’ requires exactly one element [-fpermissive]
    auto x3 = {6, 2};   //initialize_list

    Z z0{};             //direct initialization
//  Z z1 = {};          //copy initialization
}
