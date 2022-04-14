#include <iostream>
#include <fstream>

namespace { //=================================================================

// C36. A destructor must not fail

class A
{
public:
    ~A() noexcept // completes normally or terminate
    {
        // if can't release a resource ... terminate()
    }
};

// C37. Make a destructors noexcept

// C40. Define a constructor if a class has an invariant

class Date
{
    int mDay;
    int mMonth;
    int mYear;
public:
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
};

struct Rec
{
    std::string str;
    int iValue{0};

public:
    Rec(const std::string& s) : str(s)
    {}
    Rec(int i) : iValue(i)
    {}
};

struct Rec2
{
    std::string str;
    int iValue;
public:
    Rec2(const std::string& s, int i=0) : str(s), iValue(i)
    {}
};

template <typename R>
void helper_printRec(const R& r)
{
    std::cout << "(" << r.str << " " << r.iValue << ")" << std::endl;
}

// C41. Constructor should create a fully initialized object

class X
{
    std::ifstream file;

public:
    X(const std::string& filename) : file(filename, std::ios::binary | std::ios::ate)
    {
        if (!file) {
            throw std::runtime_error{"could not open " + filename};
        }
    }
    int size()
    {
        int ret = 0;

        if (file.is_open()) {
            ret = file.tellg();
        }
        return ret;
    }
};

// C42. If a constructor cannot constuct a valid object, throw an exception

} // namespace ================================================================

void test_c_40()
{
    auto r0 = Rec2 {"Qt", 6 };
    helper_printRec(r0);

    auto r1 = Rec {"Qt"};
    helper_printRec(r1);
}

void test_c_41()
{
#if (1)
    auto filename = std::string{"/etc/timezone"};
#else
    auto filename = std::string{"~/.bashrc"};
#endif
    try {
        auto x = X(filename);
        std::cout << filename << " : " << x.size() << " bytes." << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
