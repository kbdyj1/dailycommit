//=============================================================================
//  A tour of C++
//  Ch.13  Utility
//
//  22/09/18
//=============================================================================

#include <vector>
#include <list>
#include <forward_list>
#include <memory>
#include <algorithm>
#include <cmath>

#include "shape.h"

namespace { //=================================================================

void draw_shape(const std::vector<std::unique_ptr<Shape>>& shapes)
{
#if (0)
    std::for_each(std::begin(shapes), std::end(shapes), [](const std::unique_ptr<Shape>& shape){
        shape->draw();
    });
#else
    std::for_each(std::begin(shapes), std::end(shapes), std::mem_fn(&Shape::draw));
#endif
}

void test_draw_shape()
{
    auto v = std::vector<std::unique_ptr<Shape>>{};
    v.push_back(std::make_unique<Rectangle>());
    v.push_back(std::make_unique<Circle>());
    draw_shape(v);
}

template <typename RandIter>
void sort_helper(RandIter b, RandIter e, std::random_access_iterator_tag)
{
    std::cout << "sort_helper(RandIter b, RandIter e, std::random_access_iterator_tag)\n";
    std::sort(b, e);
}

template <typename FwdIter>
void sort_helper(FwdIter b, FwdIter e, std::forward_iterator_tag)
{
    std::cout << "sort_helper(FwdIter b, FwdIter e, std::forward_iterator_tag)\n";
    auto v = std::vector<typename FwdIter::value_type>{b, e};
    std::sort(v.begin(), v.end());
    std::copy(v.begin(), v.end(), b);
}

template <typename C>
void sort(C& c)
{
    using Iter = typename C::iterator;
    sort_helper(std::begin(c), std::end(c), typename std::iterator_traits<Iter>::iterator_category());
}

void test_sort()
{
    auto v = std::vector<int>{ 1, 9, 8, 4, 7, 2 };
    auto l = std::forward_list<int>{ 1, 9, 8, 4, 7, 2 };

    sort(v);
    sort(l);
}

void test_math()
{
    sqrt(-1);

    if (errno == EDOM) {
        std::cerr << "EDOM\n";
    }

    errno = 0;

    pow(std::numeric_limits<double>::max(), 2);
    if (errno == ERANGE) {
        std::cerr << "ERANGE\n";
    }
}

} //namespace =================================================================

void test_ch_13()
{
#if (0) // done
    test_draw_shape();
    test_sort();
#endif

    test_math();
}
