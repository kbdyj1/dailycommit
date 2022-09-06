#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

namespace { //=================================================================

using namespace std::chrono_literals;

//#define USE_ATOMIC_COUNTER

void test_atomic_counter()
{
#if defined(USE_ATOMIC_COUNTER)
    auto counter = std::atomic<int>{0};
#else
    auto counter = 0;   // counter: 9963
#endif
    auto threads = std::vector<std::thread>{};
    for (auto i=0; i<10; i++) {
        threads.emplace_back([&counter](){
            for (int i=0; i<1000; i++) {
                ++counter;
                std::this_thread::yield();
            }
        });
    }
    for (auto& t : threads)
        t.join();

    std::cout << "counter: " << counter << std::endl;
}

void test_atomic_flag()
{
    auto lock = std::atomic_flag{ATOMIC_FLAG_INIT};

    int counter = 0;
    auto threads = std::vector<std::thread>{};
    for (auto i=0; i<10; ++i) {
        threads.emplace_back([&](){
            while (lock.test_and_set(std::memory_order_acquire))
                ;
            ++counter;
            lock.clear(std::memory_order_release);
        });
    }

    for (auto& t : threads)
        t.join();

    std::cout << "counter: " << counter << std::endl;
}

std::vector<int> generateRandom()
{
    const auto SIZE = 1000;

    auto ret = std::vector<int>(SIZE);

    auto rd = std::random_device{};
    auto mt = std::mt19937{rd()};
    auto dist = std::uniform_int_distribution<int>{1, 99};
    for (auto i=0; i<SIZE; i++) {
        ret.push_back(dist(mt));
    }

    return ret;
}

void test_atomic_fetch()
{
    auto sum = std::atomic<int>{0};
    auto numbers = generateRandom();// std::vector{ 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };
    auto size = numbers.size();
    auto threads = std::vector<std::thread>{};

    for (auto i=0; i<10; i++) {
        threads.emplace_back([&sum, &numbers](const size_t start, const size_t end){
            for (auto i=start; i<end; i++) {
#if (0)
                std::atomic_fetch_add_explicit(&sum, numbers[i], std::memory_order_acquire);
#else
                sum.fetch_add(numbers[i], std::memory_order_acquire);
#endif
            }
        },
        i * (size/10),
        (i+1) * (size/10));
    }

    for (auto& t : threads)
        t.join();

    std::cout << "sum: " << sum << std::endl;
}

namespace memory_order {

void producer(std::atomic<bool>* isReady, int* data)
{
    *data = 10;
    isReady->store(true, std::memory_order_release);
}

void consume(std::atomic<bool>* isReady, int* data)
{
    while (!isReady->load(std::memory_order_acquire))
        ;
    std::cout << "Data: " << *data << std::endl;
}

void test_release_acquire()
{
    auto threads = std::vector<std::thread>{};
    auto isReady = std::atomic<bool>{false};
    int data = 0;

    threads.push_back(std::thread(consume, &isReady, &data));
    threads.push_back(std::thread(producer, &isReady, &data));

    for (auto& t : threads)
        t.join();
}

std::atomic<bool> x(false);
std::atomic<bool> y(false);
std::atomic<int> z(0);

void writeX() { x.store(true, std::memory_order_release); }
void writeY() { y.store(true, std::memory_order_release); }

void readXThenY() {
    while (!x.load(std::memory_order_acquire))
        ;

    if (y.load(std::memory_order_acquire))
        z++;
}

void readYThenX() {
    while (!y.load(std::memory_order_acquire))
        ;
    if (x.load(std::memory_order_acquire))
        z++;
}

void test_seq_cst()
{
    auto t0 = std::thread(writeX);
    auto t1 = std::thread(writeY);
    auto t2 = std::thread(readXThenY);
    auto t3 = std::thread(readYThenX);

    std::this_thread::sleep_for(50ms);

    t0.join();
    t1.join();
    t2.join();
    t3.join();

    std::cout << "z: " << z << std::endl;
}

} //memory_order ----------------------------------------------------

template <typename T, typename I = typename std::enable_if<std::is_integral<T>::value>::type>
class AtomicCounter {
    std::atomic<T> mCounter {0};

public:
    T inc() {
        return mCounter.fetch_add(1);
    }

    T dec() {
        return mCounter.fetch_sub(1);
    }

    T get() {
        return mCounter.load();
    }
};

void test_custom_atomic_counter()
{
    AtomicCounter<int> counter;
    auto threads = std::vector<std::thread>{};

    for (auto i=0; i<10; i++) {
        threads.emplace_back([&counter]{
            counter.inc();
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "AtomicCounter.get(): " << counter.get() << std::endl;
}

} //namespace =================================================================

void test_ch_08_atomic()
{
#if (0) // done
    test_atomic_counter();
    test_atomic_flag();
    test_atomic_fetch();
    memory_order::test_release_acquire();
    memory_order::test_seq_cst();
#endif

    test_custom_atomic_counter();
}
