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

} //===========================================================================

void test_ch_09()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
#endif

    _2::test();
}
