#include <iostream>
#include <array>
#include <iterator>
#include <exception>
#include <type_traits>

//container
#include <vector>
#include <list>
#include <deque>
#include <forward_list>

//adapter
#include <stack>
#include <queue>

using namespace std;

namespace { //=============================================================

template <typename T>
auto printContainer = [](const auto& l){
    std::copy(l.begin(), l.end(), std::ostream_iterator<T>(std::cout, " "));
    cout << "\n";
};

namespace _1 {

void test()
{
    auto array = std::array<int, 4>{1, 2, 3, 4};
    copy(array.begin(), array.end(), ostream_iterator<int>(cout, " "));

    cout << "front: " << array.front() << "\n";
    cout << "back : " << array.back() << "\n";

    auto array2 = std::array<int, 4>{1, 2, 3, 5};

    std::cout << "array < array2: " << (array < array2) << "\n";

    try {
        auto i = array[4];
        auto iAt = array.at(4);
    } catch (exception& e) {
        cerr << e.what() << endl;
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename... Args>
auto buildArray(Args&&... args) -> std::array<typename std::common_type<Args...>::type, sizeof...(args)>
{
    using commonType = typename std::common_type<Args...>::type;

    std::cout << "commonType: " << typeid(commonType).name() << "\n";

    return {std::forward<commonType>((Args&&)args)...};
}

void test()
{
    auto array = buildArray(1, 0u, 'a', 3.2f, false);

    for (auto item : array) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

} //_2 --------------------------------------------------------------

namespace _3 { //vector

class Student {
    int id;
    string name;
public:
    Student() : id(-1)
    {
        cout << "Student()\n";
    }
    Student(int id, string name) : id(id), name(name)
    {
        cout << "Student(" << id << ", " << name << ")\n";
    }
    Student(const Student& other)
    {
        cout << "Student(const Student&)\n";
        id = other.id;
        name = other.name;
    }
};

void test()
{
    auto printCapacity = [](const auto& v) {
        std::cout << "vector size: " << v.size() << ", capacity: " << v.capacity() << "\n";
    };
    auto v = std::vector<int>{};

    printCapacity(v);

    v.push_back(1); //capacity: 1
    v.push_back(2); //capacity: 2
    v.push_back(3); //capacity: 4

    printCapacity(v);

    auto students = std::vector<Student>();

    auto student = Student{5, "Noname"};

    students.push_back(student);

    cout << "Newjeans members...\n";

    students.emplace_back(0, "Minji");
    students.emplace_back(1, "Hani");
    students.emplace_back(2, "Daniel");
    students.emplace_back(3, "Haerin");
    students.emplace_back(4, "Hyein");
}

} //_3 --------------------------------------------------------------

namespace _4 { //forward_list

void test()
{
    auto printForwardList = [](const auto& l){
        std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
        cout << "\n";
    };
    auto l = std::forward_list<int>{1, 2, 3, 4, 5};

    l.push_front(1);
    l.insert_after(l.begin(), 2);

    printForwardList(l);

    cout << "\nremove 2 !!!\n";
    l.remove(2);

    printForwardList(l);

    l = std::forward_list<int>{ 23, 0, 1, -3, 34, 32 };

    l.reverse();

    printForwardList(l);

    l.sort(std::greater<int>());

    printForwardList(l);

    l = std::forward_list<int>{ 1, 2, 1, 1, 2, 2, 2, 1, 3 };

    l.unique();

    printForwardList(l);    //1,2,1,2,1,3

    cout << endl;
}

} //_4 --------------------------------------------------------------

namespace _5 { //list

void test()
{
    auto l = std::list<int>{1, 2, 3, 4, 5};

    l.push_back(6);
    l.insert(next(l.begin()), 0);

    printContainer<int>(l);

    l.reverse();

    printContainer<int>(l);

    l.remove_if([](int value){
        return value % 2 == 0;
    });

    printContainer<int>(l);
}

} //_5 --------------------------------------------------------------

namespace _6 {

void test()
{
    auto v = std::vector<int>{ 1, 2, 3, 4 };
    auto iter3 = v.begin() + 3;
    std::cout << "iter3: " << *iter3 << "\n";

    v.insert(v.begin() + 2, 0);
    std::cout << "iter3: " << *iter3 << "\n";
}

} //_6 --------------------------------------------------------------

namespace _7 {

void test()
{
    auto dq = std::deque<int>{ 1, 2, 3, 4, 5 };

    dq.push_front(0);
    dq.push_back(6);
    dq.insert(dq.begin()+2, 10);

    printContainer<int>(dq);
}

} //_7 --------------------------------------------------------------

namespace _8 { //stack

void test()
{
    auto s = std::stack<int>{};

    s.push(1);
    s.push(2);
    s.push(3);

    std::cout << "stack.top(): " << s.top() << ", stack.empty(): " << s.empty() << "\n";
}

} //_8 --------------------------------------------------------------

} //==========================================================================

void test_ch_01()
{
    cout << boolalpha;

#if (0)
    _1::test();
    _2::test();
    _3::test(); //vector
    _4::test(); //forward_list
    _5::test(); //list
    _6::test(); //iterator
#endif

    _7::test(); //deque
}
