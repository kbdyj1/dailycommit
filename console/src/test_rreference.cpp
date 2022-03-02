#include <iostream>
#include <string>
#include <cstring>
#include <vector>

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
        delete [] name;
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

} // namespace

void test_rreference()
{
    //test2();
    test3();
}
