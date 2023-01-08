#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <string_view>

namespace { //================================================================

namespace _1 {

struct Median {
    std::priority_queue<int> maxHeap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    void insert(int data)
    {
        if (maxHeap.size() == 0) {
            maxHeap.push(data);
            return;
        }
        if (maxHeap.size() == minHeap.size()) {
            if (data < get()) {
                maxHeap.push(data);
            } else {
                minHeap.push(data);
            }
            return;
        }
        if (maxHeap.size() < minHeap.size()) {
            if (data > get()) {
                maxHeap.push(minHeap.top());
                minHeap.pop();
                minHeap.push(data);
            } else {
                maxHeap.push(data);
            }

            return;
        }
        if (data < get()) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
            maxHeap.push(data);
        } else {
            minHeap.push(data);
        }
    }

    double get()
    {
        if (maxHeap.size() == minHeap.size()) {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }

        if (maxHeap.size() < minHeap.size())
            return minHeap.top();

        return maxHeap.top();
    }
};

void test()
{
    auto median = Median{};

    median.insert(1);
    std::cout << "median(1): " << median.get() << std::endl;

    median.insert(5);
    std::cout << "median(5): " << median.get() << std::endl;

    median.insert(2);
    std::cout << "median(2): " << median.get() << std::endl;

    median.insert(10);
    std::cout << "median(10): " << median.get() << std::endl;

    median.insert(40);
    std::cout << "median(40): " << median.get() << std::endl;
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
void print(std::string_view name, T const& q)
{
    std::cout << name << ": \t";
    for (auto const& n : q) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}

template <typename Q>
void print_queue(std::string_view name, Q q)
{
    for (std::cout << name << ": \t"; !q.empty(); q.pop()) {
        std::cout << q.top() << ' ';
    }
    std::cout << '\n';
}

void test()
{
    const auto data = {1, 8, 5, 6, 3, 4, 0, 9, 7, 2};
    print("data", data);

    auto q0 = std::priority_queue<int>{};
    for (auto n : data) {
        q0.push(n);
    }
    print_queue("q0", q0);  //q0: 	9 8 7 6 5 4 3 2 1 0

    auto q1 = std::priority_queue<int, std::vector<int>, std::greater<int>>(data.begin(), data.end());
    print_queue("q1", q1);  //q1: 	0 1 2 3 4 5 6 7 8 9
}

} //_2 --------------------------------------------------------------

} //==========================================================================

void test_ch_02_heap()
{
#if (0)
    _1::test();
#endif

    _2::test();
}
