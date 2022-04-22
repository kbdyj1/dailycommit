#include <iostream>
#include <functional>
#include <iomanip>
#include <QHash>
#include <QDebug>
#include <QDate>

namespace { //=================================================================

// 1. Accepts a single parameter of type Key
// 2. Returns a value of type std::size_t that represents the hash value of the parameter
// 3. Does not throw exceptions when called
// 4. For two parameters k1 and k2 that are equal, std::hash<Key>()(k1) == std::hash<Key>()(k2)
// 5. For two different parameter k1 and k2 that are not equal, the probability that
//    std::hash<Key>()(k1) == std::hash<Key>()(k2) should be very small

struct S
{
    std::string firstName;
    std::string secondName;
};
bool operator==(const S& l, const S& r)
{
    return l.firstName == r.firstName && l.secondName == r.secondName;
}

struct Hash
{
    std::size_t operator()(S const& s) const noexcept
    {
        std::size_t h0 = std::hash<std::string>{}(s.firstName);
        std::size_t h1 = std::hash<std::string>{}(s.secondName);

        return h0 ^ (h1 << 1);
    }
};
#if (0)
template<>
struct std::hash<S>
{
    std::size_t operator()(S const& s) const noexcept
    {
        std::size_t h0 = std::hash<std::string>{}(s.firstName);
        std::size_t h1 = std::hash<std::string>{}(s.secondName);

        return h0 ^ (h1 << 1);
    }
};
#endif

// QHash's key and value data types must be assignable data types.

void test_qt()
{
    auto hash = QHash<QString, int>{};

    hash["one"] = 1;
    hash["two"] = 2;
    hash["three"] = 3;
    hash["four"] = 4;
    hash["five"] = 5;

    for (auto iter = hash.constBegin(); iter != hash.constEnd(); ++iter) {
        qDebug() << "hash[" << iter.key() << "] : " << iter.value();
    }
}

// 1. assignable
// 2. operator==
// 3. qHash

class A
{
    QString name;
    QDate date;

public:
    A() = default;
    A(const QString& s, const QDate& d = QDate{}) : name(s), date(d)
    {}
    A(const A&) = default;
    A(A&&) = default;
    A& operator=(const A&) = default;
    A& operator=(A&&) = default;
   ~A() = default;

    QString s() const
    {
        return name;
    }

    bool operator==(const A& other) const
    {
        return name == other.name && date == other.date;
    }
    uint hash(uint seed) const
    {
        return qHash(name, seed) ^ date.day();
    }
};

uint qHash(const A& key, uint seed)
{
    return key.hash(seed);
}

void test_custom_class()
{
    auto hash = QHash<A, int>{};

    hash[A("one")] = 1;
    hash[A("two")] = 2;
    hash[A("three")] = 3;
    hash[A("four")] = 4;
    hash[A("five")] = 5;

    for (auto iter = hash.constBegin(); iter != hash.constEnd(); iter++) {
        qDebug() << "hash[" << iter.key().s() << "] : " << iter.value();
    }
}

} // namespace ================================================================

void test_hash()
{
    std::string str = "Hello, Qt6";
    std::size_t h0 = std::hash<std::string>{}(str);
    std::cout << "hash(" << std::quoted(str) << ") : " << h0 << std::endl;

    S s{"Hello, ", "Qt6"};
    std::cout << "hash(" << std::quoted(s.firstName) << ", " << std::quoted(s.secondName) << ") : "
              << Hash{}(s) << std::endl;
}

void test_qt_hash()
{
    qDebug() << "nbuiltin data ------------------------";
    test_qt();

    qDebug() << "\n";
    qDebug() << "custom class -------------------------";
    test_custom_class();
}
