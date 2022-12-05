#include <iostream>
#include <tuple>
#include <set>
#include <memory>
#include <cassert>
#include <variant>

namespace { //=================================================================

namespace _1 {

struct person_t {
    int age;
    std::string surname;
    std::string name;
};

bool operator<(const person_t& l, const person_t& r)
{
    // tie creates a tuple of references to the passed value
    return std::tie(l.age, l.surname, l.name) < std::tie(r.age, r.surname, r.name);
}

void test()
{
    auto p0 = person_t{10, "b", "d"};
    auto p1 = person_t{12, "a", "d"};

    std::cout << "p0 < p1 : " << (p0 < p1) << "\n";

    auto s = std::set<person_t>{};
    auto iter = std::set<person_t>::iterator{};
    bool inserted;

    //tie may be used to unpack a std::pair because std::tuple has a converting assignment from pairs
    std::tie(iter, inserted) = s.insert(p0);
    std::cout << "inserted: " << inserted << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

class State
{
protected:
    State(int type) : type(type)
    {}

public:
    virtual ~State()
    {}
    int type;
};

class Init : public State
{
public:
    enum { Id = 0 };

    Init() : State(Id)
    {
        std::cout << "Init()\n";
    }
};

class Running : public State
{
    unsigned int count = 0;

public:
    enum { Id = 1 };
    Running() : State(Id)
    {
        std::cout << "Running()\n";
    }

    unsigned int getCount() const
    {
        return count;
    }
};

class Finish : public State
{
    unsigned int count = 0;

public:
    enum { Id = 2 };
    Finish(unsigned int count) : State(Id), count(count)
    {
        std::cout << "Finish()\n";
    }
};

#define USE_VARIANT

class Program
{
#if defined(USE_VARIANT)
    std::variant<Init, Running, Finish> state;

public:
    Program() : state(Init())
    {}

    void run()
    {
        auto* s = std::get_if<Init>(&state);

        assert(s != nullptr);

        state = Running();
    }

    void finished()
    {
        auto* s = std::get_if<Running>(&state);

        assert(s != nullptr);

        state = Finish(s->getCount());
    }

    int index() const
    {
        return state.index();
    }
#else
    std::unique_ptr<State> state;
public:
    Program() : state(std::make_unique<Init>())
    {}

    void finished()
    {
        assert(state->type == Running::Id);

        auto s = static_cast<Running*>(state.get());

        state = std::make_unique<Finish>(s->getCount());
    }
#endif
};

void test()
{
    auto program = Program();

    std::cout << program.index() << "\n";
    program.run();
    std::cout << "run: " << program.index() << "\n";
    program.finished();
    std::cout << "finish: " << program.index() << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T, typename E>
class Expected
{
    union {
        T mValue;
        E mError;
    };
    bool mValid;

public:
    Expected(const Expected& other) : mValid(other.mValid)
    {
        if (mValid) {
            new (&mValue) T(other.mValue);
        } else {
            new (&mError) E(other.mError);
        }
    }
    Expected(Expected&& other) : mValid(other.mValid)
    {
        if (mValid) {
            new (&mValue) T(std::move(other.mValue));
        } else {
            new (&mError) E(std::move(other.mError));
        }
    }
    ~Expected()
    {
        if (mValid) {
            mValue.~T();
        } else {
            mError.~E();
        }
    }
    operator bool() const
    {
        return mValid;
    }
    operator std::optional<T>() const
    {
        if (mValid) {
            return mValue;
        } else {
            return std::optional<T>();
        }
    }

    T& get()
    {
        if (!mValid) {
            throw std::logic_error("invalid");
        }
        return mValue;
    }
    E& error()
    {
        if (mValid) {
            throw std::logic_error("there is no error");
        }
        return mError;
    }

    template<typename... Args>
    static Expected success(Args&&... params)
    {
        Expected result;

        result.mValid = true;

        new(&result.mValue) T(std::forward<Args>(params)...);

        return result;
    }

    template<typename... Args>
    static Expected error(Args&&... params)
    {
        Expected result;

        result.mValid = false;

        new(&result.mError) E(std::forward<Args>(params)...);

        return result;
    }
    void swap(Expected& other)
    {
        if (mValid) {
            if (other.mValid) {
                std::swap(mValue, other.mValue);
            } else {
                auto temp = std::move(other.mError);
                other.mError.~E();
                new (&other.mValue) T(std::move(mValue));

                mValue.~T();
                new (&mError) E(std::move(temp));
                std::swap(mValid, other.mValid);
            }
        } else {
            if (other.mValid) {
                other.swap(*this);
            } else {
                std::swap(mError, other.mError);
            }
        }
    }
};

template <typename T, typename Variant, typename Exp = Expected<T, std::string>>
Exp get_if(const Variant& v)
{
    T* p = std::get_if<T>(v);

    if (p) {
        return Exp::success(*p);
    } else {
        return Exp::error("Variant doesn't contain the desired type");
    }
}

} //_3 --------------------------------------------------------------

} //===========================================================================

void test_ch_09()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
#endif

    _2::test();
}
