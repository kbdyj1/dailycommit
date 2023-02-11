#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <type_traits>

namespace { //=================================================================

namespace _1 {

template <typename T, std::size_t N>
class StaticVector
{
    std::aligned_storage_t<sizeof(T), alignof(T)> data[N];
    std::size_t mSize = 0;

public:
    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if (N <= mSize)
            throw std::bad_alloc{};

        ::new(&data[mSize]) T(std::forward<Args>(args)...);
        ++mSize;
    }
    const T& operator[](std::size_t pos) const
    {
        return *std::launder(reinterpret_cast<const T*>(&data[pos]));
    }
    ~StaticVector()
    {
        for (std::size_t pos = 0; pos < mSize; ++pos) {
            std::destroy_at(std::launder(reinterpret_cast<T*>(&data[pos])));
        }
    }
};

template <typename T>
struct MoveConstructor
{
    static void call(T* lhs, T* rhs)
    {
#if 201402L <__cplusplus
        if constexpr(std::is_move_constructible_v<T>) {
            new (lhs) T(std::move(*rhs));
        } else if constexpr(std::is_default_constructible_v<T> && std::is_move_assignable_v<T>) {
            new (lhs) T();
            *lhs = std::move(*rhs);
        } else {
            []<bool flag = false>() {
                static_assert(flag, "move constructor disabled");
            }();
        }
#else
        new (lhs)T(std::move(*rhs));
#endif
    }
};

struct Foo
{
    int i;
    float f;
    char c;
};

struct Empty
{};

void test_alignof()
{
    std::cout << "char : " << alignof (char) << "\n";
    std::cout << "int* : " << alignof (int*) << "\n";
    std::cout << "Foo  : " << alignof (Foo) << "\n";
    std::cout << "Empty: " << alignof (Empty) << "\n";
}

void test_static_vector()
{
    auto v0 = StaticVector<std::string, 10>{};

    v0.emplace_back(5, '*');
    v0.emplace_back(10, '-');

    std::cout << v0[0] << ", " << v0[1] << '\n';
}

void test()
{
    //test_alignof();
    test_static_vector();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_static_memory()
{
    _1::test();
}
