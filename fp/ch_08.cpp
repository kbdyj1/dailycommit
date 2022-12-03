#include <iostream>
#include <memory>

namespace { //=================================================================

namespace _1 {

template <typename T>
class list
{
    struct node {
        T value;
        std::shared_ptr<node> tail;

        ~node()
        {
            auto next = std::move(tail);
            while (next) {
                if (!next.unique())
                    break;

                std::shared_ptr<node> tail;
                std::swap(tail, next->tail);
                next.reset();

                next = std::move(tail);
            }
        }
    };
    std::shared_ptr<node> head;
};

}

} //===========================================================================

void test_ch_08()
{

}
