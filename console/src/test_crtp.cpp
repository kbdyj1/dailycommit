#include <iostream>

namespace { //=================================================================

template <typename T>
class Singleton
{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& instance()
    {
        static T t;
        return t;
    }

protected:
    Singleton() = default;
};

template <typename T>
struct Counter
{
    static int count;

    Counter()
    {
        ++count;
    }
   ~Counter()
    {
        --count;
    }
};
template <typename T>
int Counter<T>::count = 0;

class Mouse
        : public Singleton<Mouse>
        , public Counter<Mouse>
{
};

} // namespace ================================================================

void test_crtp()
{
    Mouse& m0 = Mouse::instance();
    Mouse& m1 = Mouse::instance();

    std::cout << "Mouse.count : " << m0.count << std::endl;
}
