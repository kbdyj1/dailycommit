#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <atomic>
#include <map>

namespace { //=================================================================

using namespace std::literals;

template <typename F, typename T>
auto apply(F&& f, T value)
{
    return f(value);
}

class Widget {
    int mValue;
public:
    Widget(int value = 0) : mValue(value)
    {
        std::cout << "Widget(" << value << ")\n";
    }
    Widget(const Widget& other)
    {
        mValue = other.mValue;
        std::cout << "Widget(Widget: " << mValue << ")\n";
    }

    int& value() {
        return mValue;
    }
};

#if (0)
auto widgetValue(Widget& w)
#else
decltype (auto) widgetValue(Widget& w)
#endif
{
    return w.value();
}

// C++11
auto func1(int const i) -> int {
    return 2 * i;
}
//C++14~
auto func2(int const i) {
    return 2 * i;
}

void test_auto()
{
    auto i = 40;
    auto d = 40.0;
    auto s = "40.0";
    auto v = { 1, 2, 3, 4, 5 };

    std::cout << typeid(i).name() << "\n";
    std::cout << typeid(d).name() << "\n";
    std::cout << typeid(s).name() << "\n";
    std::cout << typeid(v).name() << "\n";

    std::cout << "\n";

    auto add = [](auto const a, auto const b) {
        return a + b;
    };

    auto iSum = add(1, 40);
    auto dSum = add(1.0, 40.0);
    auto sSum = add("Hello, "s, "Qt6"s);

    std::cout << typeid(iSum).name() << "\n";
    std::cout << typeid(dSum).name() << "\n";
    std::cout << typeid(sSum).name() << ", sSum: " << sSum << "\n";

    auto result = apply(sqrt, 4.0);
    std::cout << "apply(sqrt, 4.0): " << result << ", " << typeid(result).name() << "\n";

    auto vec = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto size = int( vec.size() );

    Widget w(10);
#if (0)
    auto value = w.value();
#else
    auto& value = w.value();
#endif
    value = 20;
    std::cout << "value: " << w.value() << "\n";

    auto aValue = std::atomic<int>(10); // ???

    typedef long long llong;

//    auto l0 = long long{ 10 };    // compile-error
    auto l1 = llong{ 20 };
    auto l2 = 30LL;

    auto& x = widgetValue(w);

    auto mix = add(10, 20.0);
    std::cout << typeid(mix).name() << "\n";
}

void test_using()
{
    using byte = unsigned char;
    using func = double(double);

    func* f = std::sqrt;
    std::cout << "f(" << 16.0 << "): " << f(16.0) << "\n";

    std::cout << "typeid(unsigned char).name(): " << typeid(unsigned char).name() << "\n";
    std::cout << "typeid(byte).name(): " << typeid(byte).name() << "\n";
}

void test_union_initializer()
{
    Widget w0{10};
    Widget w1 = {20};

    auto i { static_cast<int>(1.2) };
    auto a = { 1.2, 2.3, 3.4, 4.5 };
    std::cout << typeid (a).name() << "\n";

    auto b = { 1 };
    std::cout << typeid (b).name() << "\n";

    auto c { 1 };
    std::cout << typeid (c).name() << "\n";
}

struct F0 {
    char c;
};
struct F1 {
    char a;
    char b;
};
struct F2 {
    char a;
    //[3]
    int b;
};
struct F3 {
    int a;
    char b;
    //[3]
    float c;
    //[4]
    double d;
    bool e;
    //[7]
};
struct alignas(4) F4 {
    char a;
    char b;
};
//error: requested alignment is less than minimum alignment of 8 for type '(anonymous namespace)::F5'
struct alignas(4) F5 {
    alignas(2) char a;
    alignas(8) int b;
};

template <typename T>
void print(const char* name)
{
    auto a = alignof(T);
    auto s = sizeof(T);
    std::cout << "alignof(" << name << "): " << a << ", sizeof(" << name << "): " << s << "\n";
}

void test_alignof()
{
    print<Widget>("Widget");
    print<F0>("F0");
    print<F1>("F1");
    print<F2>("F2");
    print<F3>("F3");
    print<F4>("F4");
    print<F5>("F5");

    alignas(8) int a;
    alignas(256) long b[4];

    std::cout << "a: " << &a << ", " << "b: " << &b << "\n";
}

enum class Status {
    Unknown,
    Created,
    Connected,
    Destructed
};
std::ostream& operator<<(std::ostream& os, Status status)
{
    switch (status) {
    case Status::Unknown:
        os << "Unknown";
        break;
    case Status::Created:
        os << "Created";
        break;
    case Status::Connected:
        os << "Connected";
        break;
    case Status::Destructed:
        os << "Destructed";
        break;
    }
    return os;
}



enum class Codes : unsigned int;

void printCode(Codes code);

enum class Codes : unsigned int {
    Cpp,
    C,
    Javascript
};
void printCode(Codes code) {
    switch (code) {
    case Codes::Cpp:
        std::cout << "Cpp\n";
        break;
    case Codes::C:
        std::cout << "C\n";
        break;
    case Codes::Javascript:
        std::cout << "Javascript\n";
        break;
    }
}

void test_scoped_enum()
{
    auto s = Status::Created;
    std::cout << s << "\n";

    auto code = Codes::Cpp;
    printCode(code);

    int js = static_cast<int>(Codes::Javascript);
}

class Base {
public:
    virtual void func() = 0;
};
class Derived final : public Base {
public:
    void func() override {
        std::cout << "Derived::func()\n";
    }
};

#if (0)
//error: cannot derive from ‘final’ base ‘{anonymous}::Derived’ in derived type ‘{anonymous}::Error’
//  262 | class Error : public Derived {
//      |       ^~~~~
class Error : public Derived {
public:
    void func() override {

    }
};
#endif

void test_virtual()
{
    auto final = 0;
    auto override = false;
}

namespace range_loop {

std::map<int, bool> getRates()
{
    return std::map<int, bool> {
        { 1, true },
        { 2, true },
        { 3, false },
        { 4, true },
        { 5, false },
        { 6, false }
    };
}

void test()
{
    std::cout << std::boolalpha;
    for (auto&& [rate, flag] : getRates()) {
        std::cout << "[rate: " << rate << ", flag: " << flag << "]\n";
    }
}

} // range_loop -----------------------------------------------------

namespace user_type_range_loop {

template <typename T, size_t const Size>
class Array {
    T data[Size];
public:
    Array(const std::initializer_list<T>& list) {
        if (list.size() < Size) {
            size_t index = 0;
            for (auto i : list) {
                data[index] = i;
                index++;
            }
            for ( ; index < Size; index++ ) {
                data[index] = 0;
            }
        }
    }
    T const& at(size_t const index) const {
        if (index < Size) {
            return data[index];
        } else {
            throw std::out_of_range("index out of range");
        }
    }
    void set(size_t const index, const T& value) {
        if (index < Size) {
            data[index] = value;
        } else {
            throw std::out_of_range("index out of range");
        }
    }
    size_t size() const {
        return Size;
    }
};
template <typename T, typename C, const size_t Size>
class ArrayIteratorType {
    C& container;
    size_t index;
public:
    ArrayIteratorType(C& container, const size_t index) : container(container), index(index)
    {}
    bool operator!=(const ArrayIteratorType& other) const {
        return index != other.index;
    }
    const T& operator*() const {
        return container.at(index);
    }
    const ArrayIteratorType& operator++() {
        ++index;
        return *this;
    }
};

template <typename T, const size_t Size>
using ArrayIterator = ArrayIteratorType<T, Array<T, Size>, Size>;

template <typename T, const size_t Size>
using ConstArrayIterator = ArrayIteratorType<T, const Array<T, Size>, Size>;

template <typename T, size_t Size>
inline ArrayIterator<T, Size> begin(Array<T, Size>& container)
{
    return ArrayIterator<T, Size>(container, 0);
}

template <typename T, size_t Size>
inline ArrayIterator<T, Size> end(Array<T, Size>& container)
{
    return ArrayIterator<T, Size>(container, container.size());
}

template <typename T, size_t Size>
inline ArrayIterator<T, Size> begin(const Array<T, Size>& container)
{
    return ArrayIterator<T, Size>(container, 0);
}

template <typename T, size_t Size>
inline ArrayIterator<T, Size> end(const Array<T, Size>& container)
{
    return ArrayIterator<T, Size>(container, container.size());
}

void test()
{
/*
 * auto&& __range = range_expression;
 * auto __begine = begin_expr
 * auto __end = end_expr
 * for ( ; __begin != __end; ++__begin) {
 *      range_declaration = *__begin;
 *      loop_statement
 * }
*/
    auto array = Array<int, 16>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    for (auto i : array) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

}

} //namespace =================================================================

void test_ch_01()
{
    //test_auto();
    //test_using();
    //test_union_initializer();
    //test_alignof();
    //test_scoped_enum();

    //range_loop::test();
    user_type_range_loop::test();
}
