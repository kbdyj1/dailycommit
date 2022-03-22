#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <chrono>

namespace
{

void test1()
{
    int&& i = 2;
    int a = 2;
    int b = 3;
    int&& c = a + b;
}

void inc(int& value)
{
    std::cout << "inc(int& " << value << ") : ";
    value += 1;
    std::cout << value << std::endl;
}

void inc(int&& value)
{
    std::cout << "inc(int&& " << value << ") : ";
    value += 1;
    std::cout << value << std::endl;
}

void test2()
{
    int a = 10;
    int b = 20;
    inc(a);
    inc(a + b);
    inc(5);
    inc(std::move(b));
}

class A
{
public:
    A() : A("", 0)
    {}

    A(const char *name, int id)
    {
        auto len = strlen(name);
        this->name = new char[len + 1];
        strncpy(this->name, name, len);
        this->name[len] = '\0';
        this->id = id;

        std::cout << "A(const char* name, int id)" << std::endl;
    }
    A(A&& a) {
//        delete [] name;
        name = a.name;
        id = a.id;
        a.id = 0;
        a.name = nullptr;

        std::cout << "A(A&&)" << std::endl;
    }
    A& operator=(A&& a) {
        if (this == &a) {
            return *this;
        }
        delete [] name;
        name = a.name;
        id = a.id;
        a.id = 0;
        a.name = nullptr;

        std::cout << "A& operator=(A&& a)" << std::endl;

        return *this;
    }

private:
    char *name;
    int id;
};

A createObject()
{
    return A("unnamed", 0);
}

void test3()
{
    auto vec = std::vector<A>();
    for (auto i=0; i<3; i++) {
        vec.push_back(A("test", i));
    }
    A("hello, Qt", 5);
    auto s1 = A("aaa", 10);
    s1 = createObject();
    auto s2 = A("aaa", 10);
    s2 = std::move(s1);
}

class B
{
public:
    B(int a, int b) {
        std::cout << "B(a, b)" << std::endl;
    }
    B(B&& rhs) {
        std::cout << "B(&&)" << std::endl;
    }
    B& operator=(const B&) {
        std::cout << "B::operator=(const B&)" << std::endl;
    }
    B& operator=(B&& rhs) {
        std::cout << "B::operator=(B&&)" << std::endl;
    }

    static B createObject() {
        return B(3, 2);
    }
};

void test4()
{
    std::vector<B> v;
    for (auto i=0; i<2; i++) {
        std::cout << "v.push_back(" << i << ") :" << std::endl;
        v.push_back(B(100, 100));
    }
    std::cout << "#1" << std::endl;
    auto s = B(2, 3);
    std::cout << "#2" << std::endl;
    s = B::createObject();
    std::cout << "#3" << std::endl;
    auto s2 = B(5, 6);
    std::cout << "#4" << std::endl;
    s2 = s;
}

class Annotation {
public:
    Annotation(const std::string text) : mText(std::move(text))
    {}
private:
    std::string mText;
};

class Widget
{
};

void process(const Widget &w)
{
    std::cout << "process(const Widget&)" << std::endl;
}
void process(Widget &&w)
{
    std::cout << "process(Widget&&)" << std::endl;
}

template <typename T>
void logAndProcess(T&& param)
{
    process(std::forward<T>(param));
}

void test5()
{
    Widget w;
    logAndProcess(w);
    logAndProcess(std::move(w));
}

template <typename T>
void f(T&& param)
{
}

template <typename T>
void g(std::vector<T>&& param)
{
}

void test6()
{
    Widget w;
    f(w);
    f(std::move(w));

    auto v = std::vector<Widget>{};
    //g(v);
}

class Matrix
{
public:
    Matrix() { std::cout << "Matrix()" << std::endl; }
    Matrix(const Matrix &rhs) { std::cout << "Matrix(const Matrix&)" << std::endl; }
    Matrix(Matrix &&rhs) { std::cout << "Matrix(Matrix&&)" << std::endl; }

    Matrix& operator+=(const Matrix &rhs)
    {
        std::cout << "operator+=(const Matrix&)" << std::endl;
        return *this;
    }
};
Matrix operator+(Matrix &&l, const Matrix &r)
{
    l += r;
    return std::move(l);
}
class Fraction
{
public:
    Fraction() { std::cout << "Fraction()" << std::endl; }
    Fraction(const Fraction &) { std::cout << "Fraction(const Fraction&)" << std::endl; }
    Fraction(Fraction&&) { std::cout << "Fraction(Fraction&&)" << std::endl; }

    void reduce() { std::cout << "Fraction.reduce()" << std::endl; }
};

template <typename T>
Fraction reduceAndCopy(T&& frac)
{
    frac.reduce();
    return std::forward<T>(frac);
}

void test7()
{
    Matrix m0, m1;
    Matrix m2 = std::move(m0) + m1;

    auto frac = reduceAndCopy(Fraction{});
}

std::multiset<std::string> gNames;

void log(std::chrono::time_point<std::chrono::system_clock> t, std::string str)
{
    std::cout << "log(t, " << str << ")" << std::endl;
}
#if (0)
void logAndAdd(const std::string& name)
{
    auto now = std::chrono::system_clock::now();

    log(now, name);

    gNames.emplace(name);
}
#else
template <typename T>
void logAndAdd(T&& name)
{
    auto now = std::chrono::system_clock::now();

    log(now, name);

    gNames.emplace(std::forward<T>(name));
}
#endif

std::string nameFromIndex(int index)
{
    return std::string{"[index]"};
}
void logAndAdd(int index)
{
    auto now = std::chrono::system_clock::now();

    log(now, "logAndAdd");

    gNames.emplace(nameFromIndex(index));
}
void test8()
{
    auto petName = std::string{"dog"};

    logAndAdd(petName);
    logAndAdd(std::string{"cat"});
    logAndAdd("bird");

    logAndAdd(0);

//    short index = 2;
//    logAndAdd(index);
}

class Person
{
public:
#if (0)
    template <typename T>
    explicit Person(T&& n) : mName(std::forward<T>(n))
    {
        std::cout << "Person(T&&)" << std::endl;
    }
#else
    explicit Person(std::string n)
        : mName(std::move(n))
    {}
#endif

    explicit Person(int index)
    {
        std::cout << "Person(int)" << std::endl;
    }

private:
    std::string mName;
};
#if (0)
class SpecialPerson : public Person
{
public:
    SpecialPerson(const SpecialPerson& rhs)
        : Person(rhs)
    {}
    SpecialPerson(SpecialPerson&& rhs)
        : Person(std::move(rhs))
    {}
    SpecialPerson()
        : Person("noname")
    {}
};
#endif

void test9()
{

#if (0)
    //error: no matching function for call to ‘std::__cxx11::basic_string<char>::basic_string({anonymous}::Person&)’
    //  298 |     explicit Person(T&& n) : mName(std::forward<T>(n))
    //      |                              ^~~~~~~~~~~~~~~~~~~~~~~~~

    auto p = Person{"Nancy"};
#else
    const auto p = Person{"Nancy"};
#endif
    auto p2{p};

#if (0)
    //error: no matching function for call to ‘std::__cxx11::basic_string<char>::basic_string(const {anonymous}::SpecialPerson&)’
    //  298 |     explicit Person(T&& n) : mName(std::forward<T>(n))
    //      |                              ^~~~~~~~~~~~~~~~~~~~~~~~~
    auto s = SpecialPerson{};

    auto s2{s};
#endif
}

template <typename T>
void logAndAddImpl(T&& name, std::false_type) // non integral type
{
    std::cout << "logAndAddImpl(T&&, std::false_type)" << std::endl;
    auto now = std::chrono::system_clock::now();
    log(now, name);
}

void logAndAddImpl(int index, std::true_type) // integral type
{
    std::cout << "logAndAddImpl(int, std::true_type)" << std::endl;
    logAndAdd(nameFromIndex(index));
}

template <typename T>
void logAndAdd2(T&& name)
{
    logAndAddImpl(std::forward<T>(name), std::is_integral<typename std::remove_reference_t<T>>());
}

void test10()
{
    logAndAdd2("Hello, Qt");
    short index = 0;
    logAndAdd2(index);
}

} // namespace

void test_rreference()
{
    //test2();
    //test3();
    //test4();
    //test5();
    //test7();
    //test9();
    test10();
}
