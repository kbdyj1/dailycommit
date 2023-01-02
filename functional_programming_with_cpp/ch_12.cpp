#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <functional>
#include <numeric>
#include <fstream>

namespace { //=================================================================

namespace _1 {

struct Employee
{
    int level;
    std::string name;
};

void printEmployee(std::ostream& os, const Employee& employee, int salary)
{
    os << "level: " << employee.level << ", name: " << employee.name << ", salary: " << salary << "\n";
}

void test()
{
    auto employees = std::vector<Employee>{
        { 0, "haerin" },
        { 1, "hani"}
    };

    for (const auto& employee : employees) {
        printEmployee(std::cout, employee, 1000);
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

auto baseSalaryForPosition = [](const std::string& position){
    int baseSalary = 0;

    if (position == "Tester")
        baseSalary = 1500;
    else if (position == "Analyst")
        baseSalary = 1600;
    else if (position == "Developer")
        baseSalary = 2000;
    else if (position == "Team Leader")
        baseSalary = 3000;
    else if (position == "Manager")
        baseSalary = 4000;

    return baseSalary;
};

auto factorForSeniority = [](const std::string& seniorityLevel){
    double factor = 1.0;

    if (seniorityLevel == "Entry")
        factor = 1.0;
    else if (seniorityLevel == "Junior")
        factor = 1.2;
    else if (seniorityLevel == "Senior")
        factor = 1.5;

    return factor;
};

auto factorForContinuity = [](int continuity){
    double continuityFactor;

    if (continuity < 3)
        continuityFactor = 1.0;
    else if (continuity < 5)
        continuityFactor = 1.2;
    else if (continuity < 10)
        continuityFactor = 1.5;
    else if (continuity < 20)
        continuityFactor = 1.7;
    else
        continuityFactor = 2.0;

    return continuityFactor;
};

auto bonusLevel = [](const std::string& level) {
    return stoi(level);
};
auto specialBonusFactor = [](auto bonusLevel) {
    return bonusLevel() * 0.03;
};

auto computeSalary = [](auto baseSalaryForPosition, auto factorForSeniority, auto factorForContinuity, auto bonusFactor) {
#if (0)
    int baseSalary = baseSalaryForPosition();
    double factor = factorForSeniority();
    double continuityFactor = factorForContinuity();

    double currentSalary = baseSalary * factor * continuityFactor;
    double salary = (1 + bonusFactor()) * currentSalary;

    int roundedSalary = ceil(salary);

    return roundedSalary;
#else
    return ceil(
        (1 + bonusFactor()) * baseSalaryForPosition() * factorForSeniority() * factorForContinuity()
    );
#endif
};

void test()
{
    auto position = std::string{"Developer"};
    auto seniorityLevel = std::string{"Senior"};
    auto yearsWorkedContinuously = 20;

    auto roundedSalary = computeSalary(
        std::bind(baseSalaryForPosition, position),
        std::bind(factorForSeniority, seniorityLevel),
        std::bind(factorForContinuity, yearsWorkedContinuously),
        std::bind(bonusLevel, "0.03")
    );

    std::cout << "roundedSalary: " << roundedSalary << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

class BaseSalaryForPosition {
    const std::string& position;

public:
    BaseSalaryForPosition(const std::string& position) : position(position)
    {}

    int baseSalaryForPosition() const
    {
        int baseSalary = 0;

        if (position == "Tester")
            baseSalary = 1500;
        else if (position == "Analyst")
            baseSalary = 1600;
        else if (position == "Developer")
            baseSalary = 2000;
        else if (position == "Team Leader")
            baseSalary = 3000;
        else if (position == "Manager")
            baseSalary = 4000;

        return baseSalary;
    }
};

} //_3 --------------------------------------------------------------

namespace _4 { //Strategy pattern

auto drinkPrices = std::map<std::string, double>{
    {"Cas", 15.00},
    {"Orange", 25.20}
};
auto happyHourBilling = [](auto price) {
    return price / 2;
};
auto normalBilling = [](auto price) {
    return price;
};
auto computeBill = [](auto drinks, auto billing) {
    auto v = std::vector<double>(drinks.size());
    std::transform(drinks.begin(), drinks.end(), v.begin(), [](auto drink){
        return drinkPrices[drink];
    });
    auto sum = std::accumulate(v.begin(), v.end(), 0.0);

    return billing(sum);
};

void test()
{
    auto v = std::vector<std::string>{"Cas", "Orange"};

    std::cout << "normal prices: " << computeBill(v, normalBilling) << "\n";
    std::cout << "happy hour prices: " << computeBill(v, happyHourBilling) << "\n";
}

} //_4 --------------------------------------------------------------

namespace _5 {

auto readAndAddTowNumbers = [](auto firstNumberReader, auto secondNumberReader){
    auto first = firstNumberReader();
    auto second = secondNumberReader();
    return first + second;
};

auto readFirstFromFile = [](){
    int number;
    std::ifstream is("number.txt");
    is >> number;
    is.close();
    return number;
};

auto readSecondFromFile = [](){
    int number;
    std::ifstream is("number.txt");
    is >> number;
    is >> number;
    is.close();
    return number;
};

auto makeNumberFile = []{
    std::ofstream os("number.txt");
    int number = 1;
    os << number;
    os << " ";
    number = 2;
    os << number;
    os.close();
};

void test()
{
#if (0)
    makeNumberFile();
#endif

    auto result = readAndAddTowNumbers(readFirstFromFile, readSecondFromFile);
    std::cout << "result: " << result << "\n";
}

} //_5  -------------------------------------------------------------

namespace _6 {

auto initAutoInc = [](const int initValue) {
    auto next = std::function<int(const int)>{[](const int value){
        return value + 1;
    }};

    return std::make_pair(initValue, next);
};

auto value = [](const auto prev) {
    return prev.first;
};

auto computeNextAutoInc = [](const auto prev) {
    auto prevValue = prev.first;
    auto nextValue = prev.second(prevValue);

    return initAutoInc(nextValue);
};

void test()
{
    const auto autoInc = initAutoInc(0);
    std::cout << "value: " << autoInc.first << "\n";

    const auto nextInc = computeNextAutoInc(autoInc);
    std::cout << "value: " << nextInc.first << "\n";
}

} //_6 --------------------------------------------------------------

namespace _7 {

template <typename ValueType>
struct State {
    const ValueType value;
    const std::function<ValueType(const ValueType)> computeNext;

    State<ValueType> nextState() const {
        return State<ValueType>{computeNext(value), computeNext};
    }
};

auto increment = [](const int current) {
    return current + 1;
};

void testSimple()
{
    const auto autoIncrementIndex = State<int>{1, increment};
    std::cout << "value: " << autoIncrementIndex.value << "\n";

    const auto nextIncrementIndex = autoIncrementIndex.nextState();
    std::cout << "value: " << nextIncrementIndex.value << "\n";
}

enum Token {
    Blank,
    X,
    O
};

typedef std::vector<Token> Row;
typedef std::vector<Row> Board;

struct Move {
    const Token token;
    const int x;
    const int y;
};

auto makeMove(const Board board, const Move move) -> Board
{
    auto nextBoard = Board(board);
    nextBoard[move.x][move.y] = move.token;
    return nextBoard;
}

auto printBoard = [](const auto& board) {
    auto token = Token::Blank;
    auto c = ' ';
    for (auto y=0; y<3; y++) {
        for (auto x=0; x<3; x++) {
            token = board[x][y];
            switch (token) {
            case Token::Blank: c = '-'; break;
            case Token::X: c = 'x'; break;
            case Token::O: c = 'o'; break;
            }
            std::cout << c;
        }
        std::cout << "\n";
    }
    std::cout << "**********\n";
};

void testTicTacToe()
{
    const auto board = Board{
        { Token::Blank, Token::Blank, Token::Blank },
        { Token::Blank, Token::Blank, Token::Blank },
        { Token::Blank, Token::Blank, Token::Blank }
    };

    auto firstMove = makeMove(board, {Token::X, 0, 0});
    printBoard(firstMove);

    auto secondMove = makeMove(firstMove, {Token::O, 1, 1});
    printBoard(secondMove);
}

void test()
{
    //testSimple();

    testTicTacToe();
}

} //_7 --------------------------------------------------------------

namespace _8 {

auto plusOptional = [](std::optional<int> l, std::optional<int> r) -> std::optional<int>
{
    return (l == std::nullopt || r == std::nullopt)
        ? std::nullopt
        : std::make_optional(l.value() + r.value());
};

auto makeOptional = [](const std::function<int(int,int)> op)
{
    return [op](const std::optional<int> l, const std::optional<int> r) -> std::optional<int> {
        if (l == std::nullopt || r == std::nullopt)
            return std::nullopt;
        return std::make_optional(op(l.value(), r.value()));
    };
};

std::function<std::optional<int>(const int, const int)> divide0 = [](const int l, const int r) -> std::optional<int> {
    return (r == 0)
        ? std::nullopt
        : std::make_optional(l / r);
};

void testPlus()
{
    auto plusOptional = makeOptional(std::plus<int>());

    std::cout << "plusOptional(1,2): " << plusOptional(std::optional{1}, std::optional{2}).value() << "\n";

    auto result = plusOptional(std::nullopt, std::optional{2});

    if (result.has_value()){
        std::cout << result.value();
    } else {
        std::cout << "nullopt";
    }
    std::cout << std::endl;
}

void testDiv()
{
    auto divOptional = makeOptional(std::divides<int>());

    auto result = divOptional(std::optional{2}, std::optional{0});

    std::cout << "divOptional(2, 0) : " << result.value_or(0) << "\n";
}

void test()
{
    //testPlus();

    testDiv();
}

} //_8 --------------------------------------------------------------

namespace _9 { //Monad

template <typename ValueType>
struct Maybe {
    typedef std::function<std::optional<ValueType>(const ValueType, const ValueType)> OperationType;

    const std::optional<ValueType> value;
    std::optional<ValueType> apply(const OperationType operation, const std::optional<ValueType> second)
    {
        if (value == std::nullopt || second == std::nullopt)
            return std::nullopt;

        return operation(value.value(), second.value());
    }
};

void test()
{
    std::function<std::optional<int>(const int, const int)> divide0 = [](const int l, const int r)
    -> std::optional<int> {
        return (r == 0) ? std::nullopt : std::make_optional(l / r);
    };
    {
        auto value = Maybe<int>{1}.apply(std::plus<int>(), 2);
        std::cout << "Maybe{1}.apply(plus<int>(), 2): " << value.value_or(0) << "\n";
    }
    {
        //auto value = Maybe<int>{std::nullopt}.apply(divide0, 2);
        //std::cout << "Maybe{nullopt}.apply(divide0, 2): " << value.value_or(-1) << "\n";
        auto value = Maybe<int>{1}.apply(std::divides<int>(), 0);
        std::cout << "Maybe{1}.apply(std::divides<int>, 0): " << value.value_or(-1) << "\n";
    }
}

} //_9 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_12()
{
#if (0)
    _1::test();
    _2::test();
    _4::test();
    _5::test();
    _6::test();
    _7::test();
    _8::test();
#endif

    _9::test();
}
