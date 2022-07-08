#include <iostream>
#include <QCache>
#include <QContiguousCache>
#include <QScopedPointer>

namespace { //=================================================================

class A
{
    int mId;
public:
    explicit A(int id) : mId(id)
    {}
    ~A()
    {
        std::cout << "~A(" << mId << ")\n";
    }
    int id() const
    {
        return mId;
    }
};

void test0()
{
    auto cache = QCache<int, A>(5);
    for (auto i=0; i<10; i++) {
        auto* a = new A(i);
        std::cout << "cache.insert(" << a->id() << ")\n";
        cache.insert(a->id(), a);
    }

    auto a = QScopedPointer<A>(cache.take(5));
    if (a) {
        std::cout << "cache.take(5) : " << a->id() << "\n";
    } else {
        std::cout << "cache.take(5) : nullptr\n";
    }
}

void test1()
{
    auto cache = QContiguousCache<A>(5);
    for (auto i=0; i<5; i++) {
        auto a = A(i);
        std::cout << "cache.append(" << a.id() << ")\n";
        cache.append(a);
    }
    for (auto i=0; i<5; i++) {
        auto a = A(10+i);
        std::cout << "cache.prepend(" << a.id() << ")\n";
        cache.prepend(a);
    }
    std::cout << "********** test1() end.\n";
}

} // namespace ================================================================

void test_qcache()
{
    test1();
}
