#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <memory>
#include <mutex>

#include "estl_util.h"
#include "a.h"

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

namespace item3 {

void test()
{
    std::vector<A> va;
    va.reserve(8);
    for (auto i=1; i<=6; i++) {
        va.push_back(A(i));
    }

    for (auto iter=va.begin(); iter!=va.end(); iter++) {
        if (iter->getValue() == 2) {
            iter = va.erase(iter);
        }
    }

    //A aa[4];
}

} // item3 ----------------------------------------------------------

namespace item4 {

void test()
{
    std::list<int> l0 = { 1, 2, 3, 4, 5 };
    std::list<int> l1 = { 6, 7, 8, 9, 10};

    print_elements(l0, "l0 : ");
    print_elements(l1, "l1 : ");

    std::cout << "l0.splice()\n";
    l0.splice(l0.end(), l1, std::find(l1.begin(), l1.end(), 6), std::find(l1.rbegin(), l1.rend(), 10).base());

    print_elements(l0, "l0 : ");
    print_elements(l1, "l1 : ");

    if (l1.size() == 0) {
        std::cout << "l1.size() == 0\n";
    }
    if (l1.empty()) {
        std::cout << "l1.empty() == true\n";
    }
}

} // item4 ----------------------------------------------------------

namespace item5 {

void test()
{
    std::vector<int> v0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> v1;

    //v1.assign(v0.begin() + v0.size()/2, v0.end());
    //std::copy(v0.begin() + v0.size()/2, v0.end(), std::back_inserter(v1));
    v1.insert(v1.end(), v0.begin() + v0.size()/2, v0.end());

    print_elements(v1, "v1 : ");
}

void test_insert()
{
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    std::vector<int> v1 = { 6, 7, 8, 9, 10};
    v1.insert(v1.begin()+v1.size()/2, v0.begin(), v0.end());

    print_elements(v1, "v1 : ");

    std::set<int> s0 = { 2, 4, 1, 3, 5 };
    std::set<int> s1 = { 8, 7, 9, 10, 6};
    s1.insert(s0.begin(), s0.end());

    print_elements(s1, "s1 : ");
}

void test_erase()
{
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    v0.erase(v0.begin()+v0.size()/2, v0.end());

    print_elements(v0, "v0 : ");
    {
        std::set<int> s0 = { 2, 4, 1, 3, 5 };
        auto iter = s0.find(3);
        iter = s0.erase(iter, s0.end());

        print_elements(s0, "s0 : ");
    }
    {
        std::map<int, std::string> m = {{0, "zero"}, {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};
        auto iter = m.find(3);
        m.erase(iter, m.end());

        for (iter = m.begin(); iter != m.end(); iter++) {
            std::cout << "{" << iter->first << ", " << iter->second << "} ";
        }
        std::cout << "\n";
    }
}

} // item5 ----------------------------------------------------------

namespace item6 {

void make_test_file()
{
    auto os = std::ofstream("data.bin");
    if (os.is_open()) {
        int value;
        for (auto i=0; i<5; i++) {
            value = i + '0';
            os.write((const char*)&value, sizeof(int));
        }
        os.close();
    }
}

void test_istream_iterator()
{
    auto is = std::ifstream("data.bin");
    if (is.is_open()) {
#if (0)
        std::list<int> data(std::istream_iterator<int>(is), std::istream_iterator<int>());
#else
        auto begin = std::istream_iterator<int>(is);
        auto end = std::istream_iterator<int>();
        std::list<int> data(begin, end);
#endif
        is.close();
        std::cout << "data.size() : " << data.size() << "\n";
        for (auto iter = data.begin(); iter != data.end(); iter++) {
            std::cout << *iter << " ";
        }
    }
}

void test()
{
    //make_test_file();
    test_istream_iterator();
}

} // item6 ----------------------------------------------------------

namespace item7 {

#if (0)
template <typename T>
struct Deleter : public std::unary_function<const T*, void>
{
    void operator()(const T* p) const {
        delete p;
    }
};
#else
struct Deleter
{
    template <typename T>
    void operator()(const T* p) const {
        delete p;
    }
};
#endif

void test_pointer_cotainer()
{
    std::vector<A*> v;
    for (auto i=0; i<4; i++) {
        v.push_back(new A(i));
    }

    // clean up
#if (0)
    std::for_each(v.begin(), v.end(), [](A* a){
        delete a;
    });
#else
    std::for_each(v.begin(), v.end(), Deleter());
#endif
}

void test_shared_ptr_container()
{
    auto v = std::vector<std::shared_ptr<A>>();
    for (auto i=0; i<4; i++) {
        v.push_back(std::shared_ptr<A>(new A(i)));
    }
}

void test()
{
    test_shared_ptr_container();
}

} // item7 ----------------------------------------------------------

namespace item8 { // Do not use auto_ptr's container

//#define SUPPORT_AUTO_PTR

#ifdef SUPPORT_AUTO_PTR
bool compare(const std::auto_ptr<A>& l, const std::auto_ptr<A>& r)
{
    return l->getValue() < r->getValue();
}

void test_auto_ptr()
{
    std::auto_ptr<A> pa0(new A);
    std::auto_ptr<A> pa1(pa0);

    std::cout << "pa0: " << pa0.get() << ", pa1: " << pa1.get() << "\n";

    std::cout << "\npa0 = pa1\n";

    pa0 = pa1;
    std::cout << "pa0: " << pa0.get() << ", pa1: " << pa1.get() << "\n";
}

void print_auto_ptr_container(const std::vector<std::auto_ptr<A>>& v)
{
    std::for_each(v.begin(), v.end(), [](const std::auto_ptr<A>& item) {
        std::cout << item->getValue() << " ";
    });
    std::cout << "\n";
}

void test_auto_ptr_container()
{
    std::vector<std::auto_ptr<A>> v;
    int data[] = { 8, 3, 2, 4 };
    int value;
    for (auto i=0; i<4; i++) {
        value = data[i];
        v.push_back(std::auto_ptr<A>(new A(value)));
    }
    print_auto_ptr_container(v);

    std::sort(v.begin(), v.end(), compare);
    print_auto_ptr_container(v);
}

#else
void test_auto_ptr_container() {}
#endif

} // item8 ----------------------------------------------------------

namespace item9 {

bool badValue(int value)
{
    return value == 100;
}

// vector, deque, string
void test_sequential_container_remove()
{
    int data[] = { 1, 100, 3, 5, 7, 9, 100, 2, 4, 6, 8, 100, 10 };
    std::vector<int> v(data, data+std::size(data));

    print_elements(v, "before erase: ");

    //v.erase(std::remove(v.begin(), v.end(), 100), v.end());
    v.erase(std::remove_if(v.begin(), v.end(), badValue), v.end());

    print_elements(v, "after erase: ");
}

void test_list_container_remove()
{
    int data[] = { 1, 100, 3, 5, 7, 9, 100, 2, 4, 6, 8, 100, 10 };
    std::list<int> l(data, data+std::size(data));

    print_elements(l, "before remove: ");
#if (0)
    l.erase(std::remove(l.begin(), l.end(), 100), l.end());
#else
    //l.remove(100);
    l.remove_if(badValue);
#endif
    print_elements(l, "after remove: ");
}

template <typename T>
void print_associate_container(const std::multiset<T>& s)
{
    std::for_each(s.begin(), s.end(), [](int value){
        std::cout << value << " ";
    });
    std::cout << "\n";
}

void test_associate_container_erase()
{
    int data[] = { 1, 100, 3, 5, 7, 9, 100, 2, 4, 6, 8, 100, 10 };
    std::multiset<int> s(data, data+std::size(data));
    print_associate_container<int>(s);
    std::cout << "before erase: ";
    s.erase(100);

    std::cout << "after erase: ";
    print_associate_container<int>(s);
}

void test_associate_container_remove_if()
{
    int data[] = { 1, 100, 3, 5, 7, 9, 100, 2, 4, 6, 8, 100, 10 };
    std::multiset<int> s(data, data+std::size(data));
    print_associate_container<int>(s);
    std::cout << "before erase: ";
#if (0)
    std::multiset<int> goodValues;

    std::remove_copy_if(s.begin(), s.end(), std::inserter(goodValues, goodValues.end()), badValue);
    s.swap(goodValues);
#else
    for (auto iter = s.begin(); iter != s.end(); ) {
        if (badValue(*iter)) {
            iter = s.erase(iter);
            //s.erase(iter++);
        } else {
            iter++;
        }
    }
#endif

    std::cout << "after erase: ";
    print_associate_container<int>(s);
}

} // item9 ----------------------------------------------------------

namespace item11 {

class Heap1 {
public:
    static void* alloc(size_t bytes, const void* memoryBlockToBeNear) {
        (void)memoryBlockToBeNear;

        std::cout << "malloc(" << bytes << ")\n";

        return malloc(bytes);
    }
    static void dealloc(void* p) {
        std::cout << "free(" << p << ")\n";

        free(p);
    }
};

template <typename T, typename Heap>
class SpecialHeapAllocator {
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <typename U>
    struct rebind {
        typedef std::allocator<U> other;
    };

    pointer allocate(size_t objects, const void* localityHint = 0) {
        return static_cast<pointer>(Heap::alloc(objects * sizeof(T), localityHint));
    }
    void deallocate(void* p, size_t objects) {
        (void)objects;

        Heap::dealloc(p);
    }
};

void test_string()
{
    std::vector<std::string, SpecialHeapAllocator<std::string, Heap1>> v;

    v.push_back("Hello, Qt 6");
    v.push_back("Effective STL chapter 1.");
}

void test_class()
{
    std::vector<A, SpecialHeapAllocator<A, Heap1>> v;

    v.push_back(A(0));
    v.push_back(A(1));
}

void test_list()
{
    std::list<A, SpecialHeapAllocator<A, Heap1>> l;

    l.push_back(A(0));
    l.push_front(A(1));
    l.push_back(A(2));
}

} // item11 ----------------------------------------------------------

namespace item12 {

void test()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    {
        auto m = std::mutex();
        std::lock_guard<std::mutex> lock(m);

        auto iter = std::find(v.begin(), v.end(), 5);
        if (iter != v.end()) {
            *iter = 0;
        }

        print_elements(v, "v: ");
    }
}

} // item12 ----------------------------------------------------------

} // namespace ================================================================

void test_ch_1()
{
    //item5::test_erase();
    //item6::test();
    //item7::test();
    //item8::test_auto_ptr_container();
    //item9::test_associate_container_remove_if();
    //item11::test_list();
    item12::test();
}
