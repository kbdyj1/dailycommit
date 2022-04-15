#include <iostream>
#include <vector>

namespace { //=================================================================

// C43. Ensure that a copyable class has a default constructor

class Date
{
    int mDay;
    int mMonth;
    int mYear;
public:
    Date() : Date(1, 1, 1970)
    {}
    Date(int dd, int mm, int yy) : mDay(dd), mMonth(mm), mYear(yy)
    {
        if (!isValid(dd, mm, yy)) {
            // throw BadDate();
        }
    }

    bool isValid(int dd, int mm, int yy)
    {
        return true;
    }

    friend void print(const Date& date);
};

void print(const Date& date)
{
    std::cout << "Date(" << date.mDay << ", " << date.mMonth << ", " << date.mYear << ")" << std::endl;
}

struct A
{
    std::string s;
    int i{};
};

// C44. Prefer default constructors to be simple and non-throwing

template <typename T>
class Vec0
{
    T* mElem;
    T* mSpace;
    T* mLast;

public:
    Vec0() : Vec0{0}
    {}
    Vec0(int size) : mElem(new T[size]), mSpace(mElem + size), mLast{mElem}
    {}
};

template <typename T>
class Vec1
{
    T* mElem = nullptr;
    T* mSpace = nullptr;
    T* mLast = nullptr;

public:
    Vec1() noexcept
    {}
    Vec1(int size) : mElem{new T[size]}, mSpace{mElem + size}, mLast{mElem}
    {}
};

// C45. Don't define a default constructor that only initializes data members
//      Use in-class member initializers instead

class Bad
{
    std::string mName;
    int mAge;

public:
    Bad() : mName{"unnamed"}, mAge{0}
    {}
};

class Good
{
    std::string mName{"unnamed"};
    int mAge = 0;

    friend void print(const Good& o);
};
void print(const Good& o)
{
    std::cout << "Good(name: " << o.mName << ", age: " << o.mAge << ")" << std::endl;
}

// C46. By default, declare single-argument constructors explicit

class String
{
public:
    String(int n)
    {}
};

class Complex
{
public:
    explicit Complex(double d)
    {}
};

} // namespace ================================================================

void test_c_43()
{
    std::vector<Date> v0(1000); // need default constructor
    print(v0.at(0));

    std::vector<Date> v1(1000, Date{1,1,1970});
    print(v1.at(0));

    auto a = A{};

    std::cout << "a.i : " << a.i << std::endl;
}

void test_c_45()
{
    auto g = Good{};
    print(g);
}

void test_c_46()
{
    String s = 10;
#if (0)
    Complex c = 10.5;
#endif
}
