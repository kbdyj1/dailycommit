#include <iostream>

namespace { //=================================================================

struct Weapon {
    int mPower;
};
struct Armor {
    int mDefence;
};
class Character {
    Weapon* mWeapon = nullptr;
    Armor* mArmor = nullptr;

    friend class CharacterDirector;

public:
    void print() const
    {
        std::cout << "power: " << (mWeapon ? mWeapon->mPower : 0);
        std::cout << ", armor: " << (mArmor ? mArmor->mDefence : 0) << "\n";
    }
};
class CharacterBuilder {
public:
    virtual Weapon* createWeapon() = 0;
    virtual Armor* createArmor() = 0;
};
class CharacterDirector
{
    CharacterBuilder* mBuilder = nullptr;
public:
    void setBuilder(CharacterBuilder* builder)
    {
        mBuilder = builder;
    }
    Character* createChracter()
    {
        auto character = new Character();
        character->mWeapon = mBuilder->createWeapon();
        character->mArmor = mBuilder->createArmor();
        return character;
    }
};
class ArcherBuilder : public CharacterBuilder
{
public:
    Weapon* createWeapon() override
    {
        auto* weapon = new Weapon();
        weapon->mPower = 50;
        return weapon;
    }
    Armor* createArmor() override
    {
        auto* armor = new Armor();
        armor->mDefence = 200;
        return armor;
    }
};
class WorriorBuilder : public CharacterBuilder
{
public:
    Weapon* createWeapon() override
    {
        auto* weapon = new Weapon();
        weapon->mPower = 80;
        return weapon;
    }
    Armor* createArmor() override
    {
        auto* armor = new Armor();
        armor->mDefence = 250;
        return armor;
    }
};

} //namespace =================================================================

void test_builder()
{
    auto director = CharacterDirector();
    auto archerBuilder = ArcherBuilder();
    director.setBuilder(&archerBuilder);
    auto* archer = director.createChracter();

    std::cout << "archer ---> ";
    archer->print();

    auto worriorBuilder = WorriorBuilder();
    director.setBuilder(&worriorBuilder);
    auto* worrior = director.createChracter();

    std::cout << "worrior ---> ";
    worrior->print();
}
