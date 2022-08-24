#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <string>
#include <numeric>

namespace { //=================================================================

using namespace std::literals;

template <typename F, typename C>
C mapf(F&&f, C c)
{
    std::transform(std::begin(c), std::end(c), std::begin(c), std::forward<F>(f));
    return c;
}

template <typename F, typename K, typename V>
std::map<K, V> mapf(F&& f, const std::map<K, V>& m)
{
    std::map<K, V> ret;
    for (auto p: m) {
        ret.insert(f(p));
    }
    return ret;
}

template <typename F, typename T>
std::queue<T> mapf(F&& f, std::queue<T> q)
{
    std::queue<T> ret;
    while (!q.empty()) {
        ret.push(f(q.front()));
        q.pop();
    }
    return ret;
}

void test_mapf()
{
    auto v = std::vector<int>{1, 2, 3, 4, 5};
    v = mapf([](const int n){ return n*n; }, v);
    std::copy(std::begin(v), std::end(v), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    auto l = std::list<int>{-1, 3, -6, 7, -9, 15};
    l = mapf([](const int n){ return std::abs(n); }, l);
    std::copy(std::begin(l), std::end(l), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    auto q = std::queue<int>({1, 3, 5, 7, 9});
    q = mapf([](const int n){ return 10 - n; }, q);
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << "\n";

    auto m = std::map<std::string, int>{ {"one", 1}, {"two", 2}, {"thres", 3}};
    m = mapf([](const std::pair<std::string,int> &p){
                 return std::make_pair(mapf(toupper, p.first), p.second);
             }, m);
    for (const auto &p: m) {
        std::cout << "{" << p.first << ", " << p.second << "} ";
    }
    std::cout << "\n";

    auto prio = std::queue<int>({10, 20, 30, 40, 50});
    prio = mapf([](const int n){ return n <= 30 ? 1 : 2; }, prio);
    while (!prio.empty()) {
        std::cout << prio.front() << " ";
        prio.pop();
    }
    std::cout << "\n";
}

template <typename F, typename C, typename T>
constexpr T foldl(F&& f, C&& c, T init)
{
    return std::accumulate(std::begin(c), std::end(c), std::move(init), std::forward<F>(f));
}

void test_fold()
{
    auto v = std::vector<int>{0, 2, -3, 5, -1, 6, 8, -4, 9};
    std::cout << foldl([](const int s, const int n){
        return s + n;
    }, v, 0) << "\n";

    std::cout << foldl(std::plus<>(), v, 0) << "\n";

    auto s = std::vector<std::string>{"Hello "s, "Qt "s, "6.0 "s, "!!!"s};
    std::cout << foldl([](const std::string& l, const std::string& r){
        return l + r;
    }, s, ""s) << "\n";
}


template <typename F, typename G>
auto compose(F&& f, G&& g)
{
    return [=](auto x){ return f(g(x)); };
}

template <typename F, typename... G>
auto compose(F&& f, G&&... g)
{
    return [=](auto x) { return f(compose(g...)(x)); };
}

void test_compose()
{
    auto n = -3;
    auto v = compose(
                [](const int n){ return std::to_string(n); },
                [](const int n) { return n*n; }
#if (1)
                ,
                [](const int n) { return n+n; },
                [](const int n) { return std::abs(n); }
#endif
    )(n);

    std::cout << "componse(" << n << "): " << v << "\n";
}

void test_complex_compose()
{
    auto vnums = std::vector<int>{ 1, -2, 3, -4, 5 };
    auto s = compose(
                [](const std::vector<int>& v) { return foldl(std::plus<>(), v, 0); },
                [](const std::vector<int>& v) { return mapf([](const int i){ return i+i; }, v); },
                [](const std::vector<int>& v) { return mapf([](const int i){ return std::abs(i); }, v); })(vnums);

    std::cout << s << "\n";
}

#if (0) // TODO. fix compile error
template <typename F, typename G>
auto operator*(F&&f, G&& g)
{
    return compose(std::forward<F>(f), std::forward<G>(g));
}

template <typename F, typename... G>
auto operator*(F&& f, G&&... g)
{
    return operator*(std::forward<F>(f), g...);
}

void test_complex_compose_2()
{
    auto vnums = std::vector<int>{ 1, -2, 3, -4, 5 };
    auto s = (
                [](const std::vector<int>& v) { return foldl(std::plus<>(), v, 0); } *
                [](const std::vector<int>& v) { return mapf([](const int i){ return i+i; }, v); } *
                [](const std::vector<int>& v) { return mapf([](const int i){ return std::abs(i); }, v); })(vnums);

    std::cout << s << "\n";
}
#endif

} //namespace =================================================================

void test_ch_03_highorder()
{
    //test_mapf();
    //test_fold();
    //test_simple_compose();
    //test_compose();
    test_complex_compose();
}
