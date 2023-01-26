#include <iostream>
#include <memory>
#include <utility>
#include <unordered_map>

namespace  { //================================================================

namespace _1 {


class Strategy {
public:
    virtual ~Strategy()
    {}

    virtual void execute() = 0;
};

class Context {
    std::unique_ptr<Strategy> mStrategy{};

public:
    void setStrategy(std::unique_ptr<Strategy> s) {
        mStrategy = std::move(s);
    }
    void execute() {
        if (mStrategy) {
            mStrategy->execute();
        }
    }
};

class Strategy1 : public Strategy {
public:
    void execute() override { std::cout << "Strategy1::execute()\n"; }
};

class Strategy2 : public Strategy {
public:
    void execute() override { std::cout << "Strategy2::execute()\n"; }
};

void test()
{
    auto c = Context{};

    c.setStrategy(std::make_unique<Strategy1>());
    c.execute();

    c.setStrategy(std::make_unique<Strategy2>());
    c.execute();
}

} //_1 --------------------------------------------------------------

namespace _2 {

struct Int {
    explicit Int(int v) : mValue(v)
    {}

    int mValue;
};

struct Hash {
    std::size_t operator()(Int m) const {
        std::hash<int> hashValue;
        return hashValue(m.mValue);
    }
};

struct Equal {
    bool operator()(const Int& l, const Int& r) const {
        return l.mValue == r.mValue;
    }
};

std::ostream& operator<<(std::ostream& os, const Int& i)
{
    os << "Int(" << i.mValue << ")";
    return os;
}

void test()
{
    using IntMap = std::unordered_map<Int, int, Hash, Equal>;

    std::cout << "IntMap: ";
    auto iMap = IntMap{{Int(-2), -2}, {Int(-1), -1}, {Int(0), 0}, {Int(1), 1}};

    for (auto m : iMap) {
        std::cout << "{" << m.first << ", " << m.second << "}";
    }
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_strategy_pattern()
{
    std::cout << "\n";

#if (0) //done
    _1::test();
#endif

    _2::test();

    std::cout << "\n";
}
