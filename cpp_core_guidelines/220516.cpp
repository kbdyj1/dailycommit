#include <iostream>
#include <regex>
#include <future>

namespace { //=================================================================

// Concurrency and parallelism

// CP.1 Assume that your code will run as part of a multi-threaded program

double computation(int x)
{
    return x;
}

double cached_computation(int x)
{
    static int cachedX = 0;
    static double cachedResult = 0;

    if (cachedX != x) {
        cachedX = x;
        cachedResult = computation(x);
    }
    return cachedResult;
}

struct ComputationCache
{
    int cachedX = 0;
    double cachedResult = 0;

    double compute(int x) {
        if (cachedX != x) {
            cachedX = x;
            cachedResult = computation(x);
        }
        return cachedResult;
    }
};

// CP.2 Avoid data races
//  - avoid global data
//  - avoid static variables
//  - more use of concrete types on the stack (and don't pass pointers around too much)
//  - more use of immutalbe data (literals, constexpr, and const)

int getId()
{
    static int sId = 1;

    return sId++;
}

constexpr int Max = 64;

int read(std::fstream& fs, std::array<double, Max>& buf, int max)
{
    return 0;
}

void f(std::fstream& fs, std::regex pattern)
{
#if (0)
    auto buf = std::array<double, Max>();
    auto size = read(fs, buf, Max);
    gsl::span<double> s{buf};
    auto h0 = std::async([&]{ std::sort(std::execution::par, s);});
    auto h1 = std::async([&]{ return findAll(buf, size, pattern);});
#endif
}

// CP.3 Minimize explicit sharing of writable data

struct Reading {};
struct InvalidData {};
bool validate(const std::vector<Reading>&)
{
    return true;
}
void temperatureGradients(const std::vector<Reading>&)
{
}
void altitudeMap(const std::vector<Reading>&)
{
}

void processReading(const std::vector<Reading>& readings)
{
    auto h0 = std::async([&]{ if (!validate(readings))
            throw InvalidData{};
    });
    auto h1 = std::async([&]{
        temperatureGradients(readings);
    });
    auto h2 = std::async([&]{
        altitudeMap(readings);
    });
}

// CP.4 Think in terms of tasks, rather than threads

// CP.8 Don't try to use volatile for synchronization

struct Pool {};

constexpr int MaxSlots = 256;
volatile int freeSlots = MaxSlots;
std::vector<Pool> pool;

} //===========================================================================

void test_cp_4()
{
    std::thread publisher([=]{ std::cout << "Hello, thread"; });
    auto task = std::async([=]{ std::cout << "Hello, async"; });

    publisher.join();
}
