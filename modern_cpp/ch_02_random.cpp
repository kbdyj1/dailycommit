#include <iostream>
#include <random>
#include <chrono>
#include <functional>
#include <map>
#include <iomanip>
#include <array>

namespace { //=================================================================

//#define USE_UNIFORM_DISTRIBUTION
#define GENERATE_MT19937_SEED

std::mt19937 getRandGenerator()
{
    auto rd = std::random_device();

#ifdef GENERATE_MT19937_SEED
    std::array<int, std::mt19937::state_size> states;

    std::generate(std::begin(states), std::end(states), std::ref(rd));
    std::seed_seq seq(std::begin(states), std::end(states));
    return std::mt19937{seq};
#else
#   if (0)
        auto seed = rd();
#   else
        auto seed = std::chrono::system_clock().now().time_since_epoch().count();
#   endif

    return std::mt19937(seed);
#endif
}

void test_mt19937()
{
    auto mt = getRandGenerator();
    auto ud = std::uniform_int_distribution<>(1, 10);
    for (auto i=0; i<10; i++) {
#ifdef USE_UNIFORM_DISTRIBUTION
        auto value = ud(mt);
#else
        auto value = mt() % 10;
#endif

        std::cout << "rand(" << i << "): " << value << "\n";
    }
}

void print_random_graph(std::function<int(void)> gen, const int iteration=10000)
{
    std::map<int, int> m;
    for (auto i=0; i<iteration; i++) {
        m[gen()]++;
    }

    auto max = std::max_element(m.begin(), m.end(), [](const auto& l, const auto& r){
        return l.second < r.second;
    });
    std::cout << std::setprecision(1) << std::fixed << std::setw(3);
    for (auto i=max->second/200; 0<i; --i) {
        for (auto elem : m) {
            std::cout << std::setprecision(1) << std::fixed << std::setw(3) << (elem.second/200 >= i ? "*" : " ");
        }
        std::cout << "\n";
    }
    for (auto elem : m) {
        std::cout << std::setw(3) << elem.first;
    }
    std::cout << "\n";
}

void test_graph()
{
    auto mt = getRandGenerator();

#if (0)
    auto dist = std::uniform_int_distribution<>{1, 6};
#else
    auto dist = std::normal_distribution<>{5, 2};
#endif
    print_random_graph([&mt, &dist](){
        return dist(mt);
    });
}

} //namespace =================================================================

void test_random()
{
    //test_mt19937();
    test_graph();
}
