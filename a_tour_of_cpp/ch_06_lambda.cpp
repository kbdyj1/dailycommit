#include <iostream>
#include <vector>
#include <memory>
#include <map>

#include "shape.h"

namespace { //=================================================================

template <typename C, typename Func>
void for_all(C& c, Func func)
{
    for (auto& x : c) {
        func(x);
    }
}

template <typename T>
void rotate_and_draw(std::vector<T>& v, double r) {
    for_all(v, [=](auto& s){
        s->rotate(r);
        s->draw();
    });
}

void test_shape()
{
    auto v = std::vector<std::unique_ptr<Shape>>{};
    v.push_back(std::make_unique<Rectangle>());
    v.push_back(std::make_unique<Circle>());
    double rotate = 0.0;
#if (0)
    for_all(v, [&rotate](std::unique_ptr<Shape>& shape){
        shape->draw();
        shape->rotate(rotate);

        rotate += 1.0;
    });
#else
    rotate_and_draw(v, rotate);
#endif
}

template <typename T>
using StringMap = std::map<std::string, T>;

void test_string_map()
{
    StringMap<int> m = {
        { "one", 1 }
    };
}

} //namespace =================================================================

void test_ch_06_lambda()
{
    test_shape();
}
