#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <random>
#include <algorithm>
#include <array>

namespace { //=================================================================

using namespace std::chrono_literals;

namespace simple {

std::condition_variable gCond;
std::mutex gMutex;
std::mutex gIoMutex;
int gValue = 0;

void test()
{
    auto p = std::thread([&](){
        std::this_thread::sleep_for(2s);
        {
            auto lock = std::unique_lock(gMutex);
            gValue = 62;
        }
        {
            auto lock = std::lock_guard<std::mutex>(gIoMutex);
            std::cout << "produced: " << gValue << std::endl;
        }
        gCond.notify_one();
    });

    auto c = std::thread([&](){
        {
            auto lock = std::unique_lock(gMutex);
            std::cout << "gCond.wait(lock) start..." << std::endl;
            gCond.wait(lock);
            std::cout << "gCond.wait(lock) end." << std::endl;
        }
        {
            auto lock = std::lock_guard<std::mutex>(gIoMutex);
            std::cout << "consume: " << gValue << std::endl;
        }
    });

    c.join();
    p.join();
}

} //simple ----------------------------------------------------------

namespace multiple {

std::mutex gIoMutex;
std::mutex gQueueMutex;
std::condition_variable gCond;
std::queue<int> gBuffer;
bool gComplete = false;

void producer(const int id,
             std::mt19937& generator,
             std::uniform_int_distribution<int>& dsleep,
             std::uniform_int_distribution<int>& dcode)
{
    for (auto i=0; i<5; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(dsleep(generator)));
        {
            auto lock = std::unique_lock(gQueueMutex);
            auto value = id * 100 + dcode(generator);
            gBuffer.push(value);
            {
                auto lock = std::unique_lock(gIoMutex);
                std::cout << "[produced(" << (id*10 + i) << ")]: " << value << std::endl;
            }
        }
        gCond.notify_one();
    }
}

void consumer()
{
    auto skipped = 0;
    while (!gComplete) {
        auto lock = std::unique_lock(gQueueMutex);
        gCond.wait_for(lock, std::chrono::milliseconds(100), [&](){
            return !gBuffer.empty();
        });

        skipped++;
        while (!gComplete && !gBuffer.empty()) {
            auto lock = std::unique_lock(gIoMutex);
            std::cout << "[comsumed]: " << gBuffer.front() << std::endl;
            gBuffer.pop();
            skipped--;
        }
    }
    std::cout << "skipped: " << skipped << std::endl;
}

void test()
{
    auto seed = std::array<int, std::mt19937::state_size>{};
    auto rd = std::random_device{};
    std::generate(std::begin(seed), std::end(seed), std::ref(rd));
    auto seq = std::seed_seq(std::begin(seed), std::end(seed));
    auto generator = std::mt19937{seq};
    auto dsleep = std::uniform_int_distribution<>{100, 500};
    auto dcode = std::uniform_int_distribution<>{1, 99};

    std::cout << "start producing & consuming..." << std::endl;

    auto t0 = std::thread{consumer};

    auto producers = std::vector<std::thread>{};
    for (auto i=0; i<5; i++) {
        producers.emplace_back(producer, i+1, std::ref(generator), std::ref(dsleep), std::ref(dcode));
    }

    for (auto& t : producers)
        t.join();

    gComplete = true;

    t0.join();

    std::cout << "done producing & consuming." << std::endl;
}

} //multiple --------------------------------------------------------

} //namespace =================================================================

void test_ch_08_condition()
{
#if (0) // done
    simple::test();
#endif

    multiple::test();
}
