#include <iostream>
#include <vector>
#include <iterator>

namespace { //=================================================================

template <typename Derived, typename Value, typename Category, typename Reference = Value&, typename Distance = std::ptrdiff_t>
class IteratorBase {
public:
    using value_type = typename std::remove_const<Value>::type;
    using reference = Reference;
    using pointer = Value*;
    using difference_type = Distance;
    using iterator_category = Category;

    reference operator*() const {
        return asDerived().dereference();
    }
    pointer operator->() const;
    Derived& operator++() {
        asDerived().increment();
        return asDerived();
    }
    Derived operator++(int) {
        Derived ret(asDerived());
        asDerived().increment();
        return ret;
    }

    friend bool operator==(const IteratorBase& l, const IteratorBase& r) {
        return l.asDerived().equals( r.asDerived() );
    }

    Derived& operator--();
    Derived operator--(int);

    reference operator[](difference_type n) const;
    Derived& operator+=(difference_type n);

    Derived& asDerived() { return *static_cast<Derived*>(this); }
    const Derived& asDerived() const { return *static_cast<const Derived*>(this); }

    friend difference_type operator-(const IteratorBase& l, const IteratorBase& r) {
        return 0;
    }
    friend bool operator<(const IteratorBase& l, const IteratorBase& r) {
        return true;
    }
};

class IteratorBaseAccess {
    template<typename Derived, typename Value, typename Category, typename Reference, typename Difference>
    friend class IteratorBase;

    template<typename Reference, typename Iterator>
    static Reference dereference(const Iterator& iter) {
        return iter.dereference();
    }

    template<typename Iterator>
    static void decrement(Iterator& iter) {
        iter.decrement();
    }

    template<typename Iterator, typename Distance>
    static Iterator& advance(Iterator& iter, Distance n) {
        return iter.advance(n);
    }
};

namespace _1 { //----------------------------------------------------

template <typename T>
class ListNode {
public:
    T value;
    ListNode<T>* next = nullptr;
    ~ListNode() { delete next; }
};

template <typename T>
class ListNodeIterator : public IteratorBase<ListNodeIterator<T>, T, std::forward_iterator_tag>
{
    ListNode<T>* current = nullptr;

public:
    ListNodeIterator(ListNode<T>* current = nullptr) : current(current)
    {}

    T& dereference() const {
        return current->value;
    }
    void increment() {
        current = current->next;
    }
    bool equals(const ListNodeIterator& other) const {
        return current == other.current;
    }
};

} //_1 --------------------------------------------------------------

namespace _2 {

struct Person {
    std::string firstName;
    std::string lastName;

    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        return os << person.firstName << ", " << person.lastName << "\n";
    }
};

template <typename Iterator, typename T>
class ProjectionIterator : public IteratorBase<ProjectionIterator<Iterator, T>,
                                               T,
                                               typename std::iterator_traits<Iterator>::iterator_category,
                                               T&,
                                               typename std::iterator_traits<Iterator>::difference_type>
{
    using Base = typename std::iterator_traits<Iterator>::value_type;
    using Distance = typename std::iterator_traits<Iterator>::difference_type;
    Iterator iter;
    T Base::* member;

    friend class IteratorBaseAccess;

public:
    ProjectionIterator(Iterator iter, T Base::* member) : iter(iter), member(member)
    {}

    T& dereference() const {
        return (*iter).*member;
    }
    void increment() {
        ++iter;
    }
    void decrement() {
        --iter;
    }
    bool equals(const ProjectionIterator& other) const {
        return iter == other.iter;
    }
};

template <typename Iterator, typename Base, typename T>
auto project(Iterator iter, T Base::* member) {
    return ProjectionIterator<Iterator, T>(iter, member);
}

void test()
{
    auto authors = std::vector<Person>{
        { "A", "aaa" },
        { "B", "bbb" },
        { "C", "ccc" }
    };
    std::copy(project(authors.begin(), &Person::firstName), project(authors.end(), &Person::firstName), std::ostream_iterator<std::string>(std::cout, "\n"));
}

} //_2 --------------------------------------------------------------

namespace _3 { //Mixin

struct Point {
    double x;
    double y;

    Point() : x(0.), y(0.)
    {}
    Point(double x, double y) : x(x), y(y)
    {}
};

struct Point3 : public Point {
    double z;
    Point3() : z(0.)
    {}
    Point3(double x, double y, double z) : Point(x, y), z(z)
    {}
};

} //_3 --------------------------------------------------------------

namespace _4 { //Mixin #2

template <typename... Mixins>
class Point : public Mixins...
{
public:
    double x;
    double y;
    Point() : Mixins()..., x(0.), y(0.)
    {}
    Point(double x, double y) : Mixins()..., x(x), y(y)
    {}
};

class Label {
public:
    std::string label;
};
using LabeledPoint = Point<Label>;

class Color {
public:
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
};
using ColoredPoint = Point<Color>;

template <typename... Mixins>
class Polygon {
    std::vector<Point<Mixins...>> points;

public:
};

} //_4 --------------------------------------------------------------

namespace _5 {

class NonVirtual
{};
class Virtual
{
public:
    virtual void foo() {
        std::cout << "Virtual::foo()\n";
    }
};

template <typename... Mixins>
class Base : public Mixins...
{
public:
    void foo() {
        std::cout << "Base::foo()\n";
    }
};

template <typename... Mixins>
class Derived : public Base<Mixins...> {
public:
    void foo() {
        std::cout << "Derived::foo()\n";
    }
};

void test()
{
    Base<NonVirtual>* nv = new Derived<NonVirtual>;
    nv->foo();

    Base<Virtual>* v = new Derived<Virtual>;
    v->foo();
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_21_facade()
{
#if (0) //done
    _2::test();
#endif

    _5::test();
}
