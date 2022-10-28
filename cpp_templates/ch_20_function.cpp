#include <iostream>
#include <type_traits>
#include <iterator>
#include <vector>
#include <list>

#include "EnableIf.hpp"

namespace { //=================================================================

template <typename Iter>
constexpr bool IsRandomAccessIterator =
    std::is_convertible_v<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>;

template <typename Iter>
constexpr bool IsBidirectionalIterator =
    std::is_convertible_v<typename std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>;

namespace _1 { //tag dispatch ---------------------------------------

template <typename Iterator, typename Distance>
void advanceImpl(Iterator& iter, Distance n, std::input_iterator_tag)
{
    std::cout << "advanceImpl( , , input_iterator_tag)\n";
    while (0 < n) {
        n--;
        iter++;
    }
}

template <typename Iterator, typename Distance>
void advanceImpl(Iterator& iter, Distance n, std::random_access_iterator_tag)
{
    std::cout << "advanceImpl( , , random_access_iterator_tag)\n";
    iter += n;
}

template <typename Iterator, typename Distance>
void advanceIter(Iterator& iter, Distance n)
{
    advanceImpl(iter, n, typename std::iterator_traits<Iterator>::iterator_category());
}

} //_1 --------------------------------------------------------------

namespace _2 { //function template enable/disable

template <typename Iter, typename Distance>
EnableIf<IsRandomAccessIterator<Iter>> advanceIter(Iter& iter, Distance n)
{
    std::cout << "advanceIter IsRandomAccessIterator? true\n";
    iter += n;
}

template <typename Iter, typename Distance>
EnableIf<IsBidirectionalIterator<Iter> && !IsRandomAccessIterator<Iter>> advanceIter(Iter& iter, Distance n)
{
    std::cout << "advanceIter IsBidirectionalIterator? true\n";
    if (0 < n) {
        for ( ; 0 < n; --n)
            ++iter;
    } else {
        for ( ; n < 0; ++n)
            --iter;
    }
}

template <typename Iter, typename Distance>
EnableIf<!IsBidirectionalIterator<Iter>> advanceIter(Iter& iter, Distance n)
{
    std::cout << "advanceIter other\n";
    while (0 < n) {
        n--;
        iter++;
    }
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename Iter>
constexpr bool IsInputIterator =
    std::is_convertible_v<typename std::iterator_traits<Iter>::iterator_category(), std::input_iterator_tag>;

template <typename T>
class Container {
public:
    template<typename Iter, typename = EnableIf<IsInputIterator<Iter>>>
    Container(Iter begin, Iter end);

    template<typename U, typename = EnableIf<std::is_convertible_v<T, U>>>
    operator Container<U>() const;
};

} //_3 --------------------------------------------------------------

namespace _4 { //c++17 constexpr if

template <typename Iter, typename Distance>
void advanceIter(Iter& iter, Distance n)
{
    if constexpr(IsRandomAccessIterator<Iter>) {
        std::cout << "advanceIter() random access.\n";
        iter += n;
    } else if constexpr(IsBidirectionalIterator<Iter>) {
        std::cout << "advanceIter() bidirectional.\n";
        if (0 < n) {
            for ( ; 0 < n; --n)
                ++iter;
        } else {
            for ( ; n < 0; ++n)
                --iter;
        }
    } else {
        std::cout << "advanceIter() other.\n";
        while (0 < n) {
            n--;
            iter++;
        }
    }
}

} //_4 --------------------------------------------------------------

void testIterAdvance()
{
#if (1)
    using namespace _4;
#else
    using namespace _1;
    using namespace _2;
#endif

    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto vi = v.begin();
    advanceIter(vi, 3);

    auto l = std::list<int>{ 1, 2, 3, 4, 5 };
    auto li = l.begin();
    advanceIter(li, 3);
}

} //namespace =================================================================

void test_ch_20_function()
{
#if (0) //done
#endif

    testIterAdvance();
}
