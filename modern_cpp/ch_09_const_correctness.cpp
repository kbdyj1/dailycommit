#include <iostream>
#include <map>
#include <vector>
#include <mutex>

namespace { //=================================================================

void bad() {
#if (0)
    const int c = 62;
    int* pX;
    const int** ppX = &pX;
    *ppX = &c;
    *pX = 0;
#endif
}

class A {
    mutable std::map<double, double> mCache;

    double internalCompute(const double value) const {
        return value;
    }

public:
    double compute(const double value) const {
        auto iter = mCache.find(value);
        if (iter != mCache.end()) {
            return iter->second;
        }

        auto result = internalCompute(value);
        mCache.insert(std::pair<double,double>(value, result));

        return result;
    }
};

template <typename T>
class Container {
    std::vector<T> mData;
    mutable std::mutex mMutex;

public:
    void add(const T& value) {
        auto lock = std::lock_guard<std::mutex>(mMutex);
        mData.push_back(value);
    }
    bool contains(const T& value) const {
        auto lock = std::lock_guard<std::mutex>(mMutex);
        return std::find(std::begin(mData), std::end(mData), value) != mData.end();
    }
};

constexpr unsigned int factorial(const unsigned int n)
{
    return n > 1 ? n * factorial(n-1) : 1;
}

class Point3 {
    const double mX;
    const double mY;
    const double mZ;

public:
    constexpr Point3(const double x, const double y, const double z)
        : mX(x)
        , mY(y)
        , mZ(z)
    {}

    constexpr double x() const { return mX; }
    constexpr double y() const { return mY; }
    constexpr double z() const { return mZ; }
};

void test_constexpr()
{
    constexpr auto size = factorial(5);
    char buf[size] = {0, };
    constexpr auto p0 = Point3(0, 1, 0);
    constexpr auto x = p0.x();
}

} //namespace =================================================================

void test_ch_09_const_correctness()
{

}
