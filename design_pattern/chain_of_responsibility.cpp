#include <iostream>
#include <string>

using namespace std;

namespace { //=================================================================

struct Creature
{
    string name;
    int attack;
    int defense;
};

ostream& operator<<(ostream& os, const Creature& c)
{
    os << "Creature{ name: " << c.name << ", attack: " << c.attack << ", defense: " << c.defense << " }";
    return os;
}

class CreatureModifier
{
    CreatureModifier* next = nullptr;

protected:
    Creature& creature;

public:
    explicit CreatureModifier(Creature& c) : creature(c)
    {}

    void add(CreatureModifier* cm)
    {
        if (next)
            next->add(cm);
        else
            next = cm;
    }

    virtual void handle()
    {
        if (next)
            next->handle();
    }
};

class DoubleAttackModifier : public CreatureModifier
{
public:
    explicit DoubleAttackModifier(Creature& c) : CreatureModifier(c)
    {}

    void handle() override
    {
        creature.attack *= 2;
        CreatureModifier::handle();
    }
};

class IncreaseDefenseModifier : public CreatureModifier
{
public:
    explicit IncreaseDefenseModifier(Creature& c) : CreatureModifier(c)
    {}

    void handle() override
    {
        if (creature.attack < 2)
            creature.defense += 1;
        CreatureModifier::handle();
    }
};

void test()
{
    auto goblin = Creature{"Goblin", 1, 1};
    auto root = CreatureModifier{goblin};
    auto doubleAttack = DoubleAttackModifier{goblin};
    auto incDefense = IncreaseDefenseModifier{goblin};

    root.add(&incDefense);
    root.add(&doubleAttack);

    root.handle();

    cout << goblin << "\n";
}

} // namespace ================================================================

void test_chain_of_responsibility()
{
    test();
}
