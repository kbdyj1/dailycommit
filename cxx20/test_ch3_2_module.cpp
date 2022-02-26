#ifdef USE_CXX20_FEATURE_MODULE

module;

#include <numeric>
#include <vector>

export module math;

export int add(int a, int b)
{
    return a + b;
}

export int getProduct(const std::vector<int>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>{});
}

#endif
