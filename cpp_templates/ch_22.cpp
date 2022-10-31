#include <iostream>
#include <vector>
#include <functional>

namespace { //=================================================================

namespace _1 {

#if (0)
template <typename F>
void run(F func, int n) {
#else
void run(std::function<void(int)> func, int n) {
#endif
    for (int i=0; i<n; i++)
        func(i);
}

void print(int i)
{
    std::cout << "(" << i << ")\n";
}

void test()
{
    auto values = std::vector<int>{};
    run([&](int i){
        values.push_back(i);
    }, 5);
    run([&](int i){
        print(i);
    }, 5);
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename Signature>
class FunctionPtr;

template <typename Ret, typename... Args>
class FunctionImpl {
public:
    virtual ~FunctionImpl()
    {}

    virtual FunctionImpl* clone() const = 0;
    virtual Ret invoke(Args... args) const = 0;
    virtual bool equals(const FunctionImpl*) const = 0;
};

template <typename Ret, typename... Args>
class FunctionPtr<Ret(Args...)> {
    FunctionImpl<Ret, Args...>* impl;

public:
    FunctionPtr() : impl(nullptr)
    {}

    FunctionPtr(const FunctionPtr& other) {
        impl = other.impl;
    }
    FunctionPtr(FunctionPtr& other) : FunctionPtr(static_cast<const FunctionPtr&>(other))
    {}

    FunctionPtr(FunctionPtr&& other) : impl(nullptr) {
        if (other.impl) {
            impl = other.impl->clone();
        }
    }

    template<typename F>
    FunctionPtr(F&& f);

    FunctionPtr& operator=(const FunctionPtr& other) {
        auto tmp = FunctionPtr(other);
        std::swap(*this, tmp);
        return *this;
    }
    FunctionPtr& operator=(FunctionPtr&& other) {
        delete impl;
        impl = other.impl;
        other.impl = nullptr;
        return *this;
    }

    template<typename F>
    FunctionPtr& operator=(F&& other) {
        auto tmp = FunctionPtr(std::forward<F>(other));
        swap(*this, tmp);
        return *this;
    }
    ~FunctionPtr() {
        delete impl;
    }

    friend void swap(FunctionPtr& f0, FunctionPtr& f1) {
        std::swap(f0.impl, f1.impl);
    }

    explicit operator bool() const {
        return impl != nullptr;
    }

    Ret operator()(Args... args) const {
        return impl->invoke(std::forward<Args>(args)...);
    }

    friend bool operator==(const FunctionPtr& f0, const FunctionPtr& f1) {
        if (!f0 || !f1) {
            return !f0 && !f1;
        }
        return f0.impl->equals(f1.impl);
    }
};

template <typename Functor, typename Ret, typename... Args>
class SpecialFunctorImpl : public FunctionImpl<Ret, Args...> {
    Functor functor;

public:
    template<typename FunctorFwd>
    SpecialFunctorImpl(FunctorFwd&& functor) : functor(std::forward<FunctorFwd>(functor))
    {}

    SpecialFunctorImpl* clone() const override {
        return new SpecialFunctorImpl(functor);
    }
    Ret invoke(Args... args) const override {
        return functor(std::forward<Args>(args)...);
    }
    bool equals(const FunctionImpl<Ret, Args...>* f) const override {
        if (auto sp = dynamic_cast<SpecialFunctorImpl*>(f)) {
            return functor == sp->functor;
        }
        return false;
    }
};

template <typename Ret, typename... Args>
template <typename F>
FunctionPtr<Ret(Args...)>::FunctionPtr(F&& f) : impl(nullptr)
{
    using Functor = std::decay_t<F>;
    using Impl = SpecialFunctorImpl<Functor, Ret, Args...>;

    impl = new Impl(std::forward<F>(f));
}

template <typename T>
class IsEqualityComparable {
    static void* conv(bool);

    template<typename U>
    static std::true_type test(decltype(conv(std::declval<const U&>() == std::declval<const U&>())),
                               decltype(conv(!(std::declval<const U&>() == std::declval<const U&>()))));

    template<typename U>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template <typename T, bool Eq = IsEqualityComparable<T>::value>
struct TryEquals {
    static bool equals(const T& t0, const T& t1) {
        return t0 == t1;
    }
};

struct NotEqualityCompariable : std::exception {
};

template <typename T>
struct TryEquals<T, false> {
    static bool equals(const T& t0, const T& t1) {
        throw NotEqualityCompariable();
    }
};

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_22()
{
    _1::test();
}
