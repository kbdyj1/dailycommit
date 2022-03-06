#include <utility>
#include <vector>
#include <cmath>
#include <iostream>

namespace { //=================================================================

struct A
{
    [[nodiscard("implicit destroying of temporary A.")]] A(int, bool)
    {}
};

template <typename T, typename ... Args>
[[nodiscard]]
T* create(Args&& ... args)
{
    return new T(std::forward<Args>(args)...);
}

enum class [[nodiscard]] ErrorCode {
    Ok,
    Warning,
    Critical,
    Fatal
};

ErrorCode errorProneFunction() {
    return ErrorCode::Fatal;
}

void test1()
{
    auto *val = create<int>(5);
    delete val;

    create<int>(5);

    errorProneFunction();

    A(5, true);
}

void test2()
{
    auto fun = [](int a){ return a * a; };
    auto sum = int{0};
    auto vec = std::vector{ 1, 2, 3, 4, 5, -1, -2, 6, 7, 8, 9 };
    for (auto v : vec) {
        if (0 < v) [[likely]] {
            sum += fun(v);
        } else {
            sum -= fun(v);
        }
    }

    std::cout << "sum: " << sum << std::endl;
}

struct Empty
{};

struct NoUniqueAddress {
    int d{};
    [[no_unique_address]] Empty e{};
};

struct UniqueAddress {
    int d{};
    Empty e{};
};

void test3()
{
    std::cout << std::endl;

    //sizeof(NoUniqueAddress) : 4
    std::cout << "sizeof(NoUniqueAddress) : " << sizeof(NoUniqueAddress) << std::endl;
    auto n = NoUniqueAddress{};
    std::cout << "NoUniqueAddress &d : " << &n.d << ", &e : " << &n.e << std::endl;

    //sizeof(UniqueAddress) : 8
    std::cout << "sizeof(UniqueAddress) : " << sizeof(UniqueAddress) << std::endl;
    auto u = UniqueAddress{};
    std::cout << "UniqueAddress &d : " << &u.d << ", &e : " << &u.e << std::endl;
}

} // namespace ================================================================

void test_ch3_8()
{
    //test1();
    //test2();
    test3();
}
