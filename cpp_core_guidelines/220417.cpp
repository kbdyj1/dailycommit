#include <iostream>
#include <span>
#include <algorithm>
#include <memory>

namespace { //=================================================================

// C49. Prefer initialization to assignment in constructors

class Good
{
    std::string s;

public:
    Good(const char* zstr) : s(zstr)
    {}
};

class Bad
{
    std::string s;

public:
    Bad(const char* zstr)
    {
        s = zstr;
    }
};

class Best
{
    std::string s;

public:
    Best(std::string_view v) : s(v)
    {}
    Best(std::span<char> p) : s(p.data())
    {}

    friend void print(const Best&);
};

void print(const Best& o)
{
    std::cout << "Best(" << o.s << ")" << std::endl;
}
void toUpper(std::span<char> s)
{
    std::transform(std::begin(s), std::end(s), std::begin(s), [](char c){
        return std::toupper(c);
    });
}

// C50. Use a factory function if you need "virtual behavior" during initialization

class BadBase
{
public:
    BadBase()
    {
        f(); // Bad. call in constructors or destructors
    }
    virtual void f() = 0;
};

class GoodBase
{
protected:
    class Token{};

public:
    explicit GoodBase(Token)
    {}
    virtual void f() = 0;

    template <typename T>
    static std::shared_ptr<T> create()
    {
        auto p = std::make_shared<T>(typename T::Token{});
        p->postInitialize();
        return p;
    }

protected:
    virtual void postInitialize()
    {
        f();
    }
};

class D : public GoodBase
{
protected:
    class Token{};

public:
    D(Token) : GoodBase(GoodBase::Token{})
    {}

    void f() override
    {
        std::cout << "D::f()" << std::endl;
    }

protected:
    template <class T>
    friend std::shared_ptr<T> GoodBase::create();
};

// C51. Use delegating constructors to represent common actions for all constructors of a class

class Date
{
protected:
    int mYear;
    int mMonth;
    int mDay;

public:
    Date(int y, int m, int d)
        : mYear(y)
        , mMonth(m)
        , mDay(d)
    {}
    Date(int m, int d)
#if (0) // Bad
        : mYear(currentYear())
        , mMonth(m)
        , mDay(d)
#else   // Good
        : Date(currentYear(), m, d)
#endif
    {}

    int currentYear() const
    {
        return 2022;
    }
};

// C52. Use inheriting constructors to import constructors into a derived class that does not need further explicit initialization
class DateHelper : public Date
{
public:
    using Date::Date;

    void print()
    {
        std::cout << "Date(" << mYear << ", " << mMonth << ", " << mDay << ")" << std::endl;
    }
};

} // namespace ================================================================

void test_c_49()
{
    auto good = Good("Qt6");
    auto bad = Bad("Qt6");

    char str[]  = "Qt6";
    auto v = std::string_view(str);

    std::cout << "std::string_view() : " << v.data() << ", len: " << v.length() << std::endl;
    auto best1 = Best(v);

    std::cout << "Best(std::string_view)" << std::endl;
    print(best1);

    std::cout << std::endl;

    auto s = std::span<char>(str, 3);
    toUpper(s);
    auto best2 = Best(s);

    std::cout << "Best(std::span<char>)" << std::endl;
    print(best2);
}

void test_c_50()
{
    auto p = D::create<D>();
}

void test_c_52()
{
    auto d = DateHelper(2022, 4, 17);
    d.print();
}
