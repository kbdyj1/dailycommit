#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <atomic>
#include <map>
#include <string.h>

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

} // user_type_range_loop -------------------------------------------

namespace avoid_implicit_conversion {

class Handle {
    int handle = 0;
public:
    explicit Handle(int h) : handle(h)
    {}
    explicit operator bool() const {
        return handle != 0;
    }
};

//#define USE_INITIALIZE_LIST

struct F {
    F() {
        std::cout << "F()\n";
    }
    F(const int a) {
        std::cout << "F(" << a << ")\n";
    }
    F(const int a, const double b) {
        std::cout << "F(" << a << ", " << b << ")\n";
    }
#ifdef USE_INITIALIZE_LIST
    F(const std::initializer_list<int>& l) {
        std::cout << "F(std::initialize_list<int>&)\n";
    }
#endif
    operator bool() const {
        return true;
    }
};

void bar(const F f)
{}

void test_implicit_conversion()
{
    F f0;
    F f1{};
    F f2(1);
    F f3 = 1;
    F f4{1};
    F f5 = {1};
    F f6(1, 2.2);
#if !defined(USE_INITIALIZE_LIST)
    F f7{2, 4.4};
    F f8 = { 3, 6.6 };
#endif

    std::cout << "\n********** bar() **********\n";
    bar({});
    bar(1);
#if !defined(USE_INITIALIZE_LIST)
    bar({1, 2.4});
#endif

    bool flag = f1;
    std::cout << std::boolalpha;
    std::cout << "flag = f1: " << flag << "\n";

    if (f2) {
        std::cout << "if(f2){}\n";
    }

    std::cout << "f2 + f3: " << f2 + f3 << "\n";
}

#if (1)
#   define EXPLICIT    explicit
#   define USE_EXPLICIT
#else
#   define EXPLICIT
#endif

class StringBuffer {
    size_t mSize;
    char* mData;
public:
    EXPLICIT StringBuffer() : mSize(0), mData(nullptr) {
        std::cout << "StringBuffer()\n";
    }
    EXPLICIT StringBuffer(const size_t size) : mSize(size), mData(new char[size]) {
        std::cout << "StringBuffer(" << size << ")\n";
    }
    EXPLICIT StringBuffer(const char* p) : StringBuffer(strlen(p) + 1) {
        strncpy(mData, p, mSize-1);
        mData[mSize-1] = '\0';
    }
    size_t size() const {
        return mSize;
    }
    EXPLICIT operator bool() const {
        return mData != nullptr;
    }
    EXPLICIT operator const char* () const {
        return mData;
    }
};

enum ItemSize {
    MinSize = 8,
    MaxSize = 256
};

void test_string_buffer()
{
    StringBuffer s0;
    StringBuffer s1{32};

#if !defined(USE_EXPLICIT)
    StringBuffer s2 = 'a';
    StringBuffer s3 = "Hello, Qt6";
    StringBuffer s4 = MaxSize;
#else
    StringBuffer s2 = StringBuffer{'a'};
    StringBuffer s3 = StringBuffer{"Hello, Qt6"};
    StringBuffer s4 = StringBuffer(MaxSize);
#endif

    std::cout << (const char*)s1 << "\n";
#if !defined(USE_EXPLICIT)
    std::cout << "s2 + s3: " << s2 + s3 << "\n";
#else
    std::cout << "s2 + s3: " << static_cast<bool>(s2) + static_cast<bool>(s3) << "\n";
#endif
}

void test()
{
    //test_implicit_conversion();
    test_string_buffer();
}

} // avoid_implicit_conversion --------------------------------------

const int UnnamedSize = 32;

namespace use_namespace {

template <size_t Size>
class Test {};

static int StaticSize = 16;

void test()
{
#if (0)
//error: the value of ‘{anonymous}::use_namespace::StaticSize’ is not usable in a constant expression
//  562 |     Test<StaticSize> t0;
//      |                    ^
    Test<StaticSize> t0;
#endif
    Test<UnnamedSize> t1;
}

} // use_namespace --------------------------------------------------

} //namespace =================================================================

void test_ch_01()
{
    //test_auto();
    //test_using();
    //test_union_initializer();
    //test_alignof();
    //test_scoped_enum();

    //range_loop::test();
    //user_type_range_loop::test();
    avoid_implicit_conversion::test();
}
