#include <memory>
#include <iostream>

namespace { //=================================================================

struct Widget
{};

namespace _1 {

void test()
{
    auto spw = std::make_shared<Widget>();
    std::weak_ptr<Widget> wpw(spw);

    std::cout << "spw.use_count() : " << spw.use_count() << std::endl;
    spw = nullptr;
    std::cout << "spw.use_count() : " << spw.use_count() << std::endl;

    std::cout << "wpw.expired() ? " << wpw.expired() << std::endl;

#if (0)
    auto spw2 = wpw.lock();
    if (spw2 == nullptr) {
        std::cout << "spw2 is null" << std::endl;
    }
#else
    try {
        std::shared_ptr<Widget> spw2(wpw);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

#endif
}

} //_1 --------------------------------------------------------------

namespace _2 {

auto widgetDeleter = [](Widget* w) {
    std::cout << "delete w" << std::endl;
    delete w;
};

void test()
{
    std::unique_ptr<Widget, decltype(widgetDeleter)> w(new Widget, widgetDeleter);
}

}  //_2 --------------------------------------------------------------

namespace _3 {

struct A
{
    int a = 1;
    ~A()
    {
        std::cout << "~A()" << std::endl;
    }
};

void test()
{
    auto a = std::make_shared<A>();
    std::weak_ptr<A> w(a);
    a = nullptr;
    std::cout << "end of _3::test()" << std::endl;
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_weak_ptr()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
