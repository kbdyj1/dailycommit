#include <iostream>
#include <vector>
#include <fstream>
#include <span>
#include <memory>

namespace { //=================================================================

// R.10 Avoid malloc and free

class A
{
    int id;
    std::string name;
};

void use()
{
    auto p0 = static_cast<A*>(malloc(sizeof(A)));
    auto p1 = new A;
    auto p2 = new (std::nothrow) A;
    if (!p2) {
        std::cerr << "bad alloc." << std::endl;
    } else {
        delete p2;
    }

    delete p0;
    free(p1);
}

// R11. Avoid calling new and delete explicitly
// perfer std::make_unique

// R12. Immediately give the result of an explicit resource allocation to a manager object

namespace bad
{

void f(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "r");
    auto buf = std::vector<char>(1024);
    // some work...
    fclose(fp);
}

} // bad

namespace good
{

void f(const std::string& filename)
{
    std::ifstream f{filename};
    auto buf = std::vector<char>(1024);
}

} // good

// R.13 Perform at most one explicit resource allocation in a single expression statement

void fun(std::shared_ptr<A> a0, std::shared_ptr<A> a1)
{

}

void callFun()
{
#if (0) // bad
    fun(std::shared_ptr<A>(new A), std::shared_ptr<A>(new A)); // new, new, constructor, constructor
#else
    fun(std::make_shared<A>(), std::make_shared<A>());
#endif
}

// R.14 Avoid [] parameters, prefer span

void g(int array[])
{
    std::cout << "array[]" << std::endl;
}
void g(std::span<int> array)
{
    std::cout << "array.size: " << array.size() << std::endl;
}

// R.15 Always overload matched allocation/deallocation pairs

class B
{
public:
    void* operator new(size_t);
    void operator delete(void*);
};
} // namespace ================================================================

void test_r_10()
{
    use();
}

void test_r_14()
{
    int array[] = { 1, 2, 3, 4, 5 };

    g(array);
    g(std::span<int>(array));
}
