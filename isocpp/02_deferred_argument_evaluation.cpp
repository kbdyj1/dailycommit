#include <unordered_map>
#include <memory>
#include <iostream>

namespace { //=================================================================

struct Object {
    Object(int id)
    {
        std::cout << "Object(" << id << ")\n";
    }
};

namespace _1 {

Object* retrieve_or_create(int id)
{
    static std::unordered_map<int, std::unique_ptr<Object>> m;

#if 201703L <= __cplusplus
    auto [it, b] = m.try_emplace(id, std::make_unique<Object>(id));
#else
    auto [it, b] = m.emplace(id, nullptr);
    if (b) {
        try {
            it->second = std::make_unique<Object>(id);
        } catch(...) {
            m.erase(it);
            throw;
        }
    }
#endif
    return it->second.get();
}

void test()
{
    auto* obj0 = retrieve_or_create(0);
    auto* obj1 = retrieve_or_create(0);
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename F>
struct DeferredCall {
    using ResultType = decltype(std::declval<const F>()());

    DeferredCall(const F& f) : f(f)
    {}

    operator ResultType() const {
        return f();
    }

    F f;
};

Object* retrieve_or_create(int id)
{
    static std::unordered_map<int, std::unique_ptr<Object>> m;

    auto [it, b] = m.try_emplace(id, DeferredCall([&]{
        return std::make_unique<Object>(id);
    }));
    return it->second.get();
}

void test()
{
    auto* obj0 = retrieve_or_create(0);
    auto* obj1 = retrieve_or_create(0);
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_deferred_argument_evaluation()
{
#if (0)
    _1::test();
#endif

    _2::test();
}
