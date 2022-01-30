#include <span>
//#include <coroutine>  //#error "the coroutine header requires -fcoroutines"
#include <vector>
#include <map>
#include <set>
#include <QDebug>

#ifndef Q_OS_MACOS
#define CXX20_SUPPORT_RANGES_FEATURE
#endif

#ifdef CXX20_SUPPORT_RANGES_FEATURE
#include <ranges>
#endif

template <class T, std::size_t N> [[nodiscard]]
constexpr auto slide(std::span<T,N> s, std::size_t offset, std::size_t width) {
    return s.subspan(offset, offset+width <= s.size() ? width : 0U);
}

namespace internal
{

/******************************************************************************
 * ranges
 *
 */
void test_cxx20_ranges()
{
    auto const list = { 0, 1, 2, 3, 4, 5 };
    auto even = [](int i){ return 0 == i % 2; };
    auto square = [](int i){ return i * i; };

    // std::view::filter
    //  -std=c++2a (OK)
    //  -std=c++20 (NG)

    // TODO : enable filter

//    for (auto i : list | std::views::filter(even) | std::views::filter(square)) {

//    }
}

/******************************************************************************
 * span
 *
 */
void print_span(const auto &list, const QString &message)
{
    auto str = QString{};
    for (auto elem : list) {
        str += QString{"%1 "}.arg(elem);
    }
    qDebug().noquote() << QString{"%1 %2"}.arg(message).arg(str);
}

void print_span(const auto &list)
{
    print_span(list, QString{});
}

void test_cxx20_span()
{
    constexpr int a[] { 0, 1, 2, 3, 4, 5, 6, 7, 8};

    auto span = std::span{a};
    qDebug() << "span.size:" << span.size();
    qDebug() << "span.size_bytes:" << span.size_bytes();

    auto first= span.first(std::size_t{4});
    print_span(first, "first:");

    auto last = span.last(std::size_t{4});
    print_span(last, "last");

    for (std::size_t offset{}; ; ++offset) {
        constexpr std::size_t width{6};
        auto s = slide(span, offset, width);
        if (s.empty())
            break;

        print_span(s);
    }
}

/******************************************************************************
 * coroutine
 *
 */
//#define COROUTINE_IMPLEMENTED

#ifdef COROUTINE_IMPLEMENTED
class Task
{
public:
    struct promise_type
    {
        // rule 1
        Task get_return_object()
        {
            return Task { std::coroutine_handle<promise_type>::from_promise(*this); }
        }

        auto initial_suspend()
        {
            return std::suspend_always{};
        }

        auto return_void()
        {
            return std::suspend_never{};
        }

        auto final_suspend()
        {
            return std::suspend_always{};
        }

        void unhandled_exception() { std::exit(1); }
    };

    // rule 2
    std::coroutine_handle<promise_type> co_handler;

    // rule 3
    Task(std::coroutine_handle<promise_type> handler) : co_handler(handler)
    {}

    // rule 4
    ~Taks()
    {
        if (true == (bool)co_handler)
        {
            co_handler.destroy();
        }
    }
};

Task foo()
{
    Task::promise_type promise;

    qDebug() << "foo1";
    //co_await std::suspend_always{};   // not implemented in g++-10 ?
    qDebug() << "foo2";
}
#endif

void test_cxx20_coroutine()
{

}

/******************************************************************************
 * 3-way comparison
 *
 */
void test_cxx20_3way_comparision()
{
    // #1
    {
        qDebug() << "int comparison";

        auto a = 91;
        auto b = 92;
        auto comp = a <=> b;
        if (comp < 0) {
            qDebug() << "a < b";
        } else if (comp == 0) {
            qDebug() << "a == b";
        } else if (comp > 0) {
            qDebug() << "a > b";
        }
    }

    // #2
    {
#ifndef Q_OS_MACOS
        qDebug() << "std::vector comparison";

        auto a = std::vector{ 1, 2, 3 };
        auto b = std::vector{ 1, 2, 3 };
        auto comp = a <=> b;
        if (comp < 0) {
            qDebug() << "a < b";
        } else if (comp == 0) {
            qDebug() << "a == b";
        } else if (comp > 0) {
            qDebug() << "a > b";
        }
#endif
    }
}

/******************************************************************************
 * map/set contains
 *
 */
void test_cxx20_map_set_contains()
{
    auto m = std::map<int, char>{
        {1, 'a' },
        {2, 'b' }
    };
    // old
    qDebug() << "m.find(2):" << (m.find(2) != m.end());
    // new
    qDebug() << "m.contains(2): " << m.contains(2);
}

/******************************************************************************
 * range based loop with initialization
 *
 */
void test_cxx20_range_base_loop_with_initialization()
{
    for (std::vector vlist{1, 2, 3}; auto &v : vlist) {
        qDebug() << v;
    }
}

}

void test_cxx20_features()
{
    //internal::test_cxx20_span();
    //internal::test_cxx20_3way_comparision();
    //internal::test_cxx20_map_set_contains();
    internal::test_cxx20_range_base_loop_with_initialization();
}
