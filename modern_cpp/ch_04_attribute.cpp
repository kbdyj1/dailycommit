#include <iostream>

namespace { //=================================================================

// __declspec() MSVC
// __attribute__((...)) GCC

[[nodiscard]] int getValue()
{
    return 6;
}

enum class [[nodiscard]] ReturnCode {
    OK,
    NoData,
    Error
};

ReturnCode lastResult()
{
    return ReturnCode::OK;
}

struct [[nodiscard]] Pos {
    int x;
    int y;
};

Pos initialPos()
{
    return {0, 0};
}

void test_nodiscard()
{
    //warning: ignoring return value of ‘int {anonymous}::getValue()’, declared with attribute ‘nodiscard’ [-Wunused-result]
    //   15 |     getValue();
    //            ~~~~~~~~^~
    getValue();

    lastResult();

    initialPos();
}

[[deprecated("use format_safe()")]]
void format()
{
    std::cout << "legacy formate()\n";
}

class [[deprecated("use PlayerNew")]] Player {
    std::string name;
public:
    Player(const std::string& n) : name(n)
    {
        std::cout << "Player(" << name << ")\n";
    }
};

void test_deprecated()
{
    //warning: ‘void {anonymous}::format()’ is deprecated: use format_safe() [-Wdeprecated-declarations]
    //   54 |     format();
    //      |     ~~~~~~^~
    format();

    auto p = Player{"NewJeans"};
}

double run([[maybe_unused]]int a, double b)
{
    return 2 * b;
}

void test_maybe_unused()
{
    std::cout << run(2, 4) << "\n";
}

} //namespace =================================================================

void test_ch_04_attribute()
{
#if (0) // done
    test_nodiscard();
    test_deprecated();
#endif

    test_maybe_unused();

    auto n = 0;

    switch (n) {
    case 0:
        std::cout << "0\n";
        [[fallthrough]];
    case 1:
        std::cout << "1\n";
        break;
    default:
        std::cout << "unknown\n";
        break;
    }
}
