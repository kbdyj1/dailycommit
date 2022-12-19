#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>
#include <numeric>

namespace { //=================================================================

auto all_of_collection = [](const auto& collection, auto pred) {
    return std::all_of(collection.begin(), collection.end(), pred);
};

auto any_of_collection = [](const auto& collection, auto pred) {
    return std::any_of(collection.begin(), collection.end(), pred);
};

template <typename Dest>
auto transformAll = [](auto const source, auto pred) {
    Dest result;
    result.reserve(source.size());
    std::transform(source.begin(), source.end(), std::back_inserter(result), pred);
    return result;
};

auto concatenate = [](auto first, const auto second) {
    auto result(first);
    result.insert(result.end(), std::make_move_iterator(second.begin()), std::make_move_iterator(second.end()));
    return result;
};
auto concatenate3 = [](auto first, const auto second, const auto third) {
    return concatenate(concatenate(first, second), third);
};

auto accumulateAll = [](auto source, auto pred) {
    return std::accumulate(source.begin(), source.end(), typename decltype(source)::value_type(), pred);
};

auto findInCollection = [](const auto& collection, auto pred) {
    auto result = std::find_if(collection.begin(), collection.end(), pred);
    return (result == collection.end()) ? std::nullopt : std::optional(*result);
};

namespace _1 {

auto allTrue = [](auto x){ return true; };
auto allFalse= [](auto x){ return false; };
auto equalA = [](auto x){ return x == 'a'; };
auto notCard= [](auto x){ return x != 'd'; };

void test()
{
    auto v = std::vector<char>{ 'a', 'b', 'c' };

    std::cout << all_of_collection(v, allTrue) << "\n";
    std::cout << all_of_collection(v, allFalse) << "\n";
    std::cout << all_of_collection(v, equalA) << "\n";
    std::cout << all_of_collection(v, notCard) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

auto lineFilledWithX = [](const auto& collection, const auto tokenToCheck) {
    return all_of_collection(collection, [tokenToCheck](const auto& token){
        return token == tokenToCheck;
    });
};

void test_fill_x()
{
    auto v0 = std::vector<char>{'X', 'X', 'X'};
    std::cout << lineFilledWithX(v0, 'X') << " == true\n";

    auto v1 = std::vector<char>{'X', ' ', 'X'};
    std::cout << lineFilledWithX(v1, 'X') << " == false\n";
}

void test()
{
    test_fill_x();
}

} //_2 --------------------------------------------------------------

namespace _3 {

auto makeCaps = [](auto x) {
    return std::toupper(x);
};

void test()
{
    auto v = std::vector<char>{'a', 'b', 'c'};
    auto result = transformAll<std::vector<char>>(v, makeCaps);
    std::cout << result.size() << "\n";
    std::copy(result.begin(), result.end(), std::ostream_iterator<char>(std::cout));
    std::cout << "\n";
}

} //_3 --------------------------------------------------------------

namespace _4 {

struct Pos {
    int x;
    int y;
};
typedef std::vector<char> Line;

auto accessAtCoord = [](const auto& board, const Pos& pos) {
    return board[pos.x][pos.y];
};
auto projectCoords = [](const auto& board, const auto& coords) {
    auto fromCoord = std::bind(accessAtCoord, board, std::placeholders::_1);
    return transformAll<Line>(coords, fromCoord);
};
auto rowCoords = [](const auto& board, auto rowIndex) {
    auto range = std::vector<int>{0, 1, 2};
    return transformAll<std::vector<Pos>>(range, [rowIndex](auto index){
        return Pos{rowIndex, index};
    });
};
auto row = [](const auto& board, int rowIndex) {
    return projectCoords(board, rowCoords(board, rowIndex));
};
auto columnCoords = [](const auto& board, auto columnIndex) {
    auto range = std::vector<int>{0, 1, 2};
    return transformAll<std::vector<Pos>>(range, [columnIndex](auto index){
        return Pos{index, columnIndex};
    });
};
auto column = [](const auto& board, int columnIndex) {
    return projectCoords(board, columnCoords(board, columnIndex));
};
auto toRange = [](const auto& collection) {
    std::vector<int> range(collection.size());
    std::iota(std::begin(range), std::end(range), 0);
    return range;
};
auto mainDiagonalCoords = [](const auto& board) {
    return transformAll<std::vector<Pos>>(std::vector<int>{0, 1, 2}, [](const auto index){
        return Pos{index, index};
    });
};
auto mainDiagonal = [](const auto& board) {
    return projectCoords(board, mainDiagonalCoords(board));
};
auto subDiagonalCoords = [](const auto& board) {
    return transformAll<std::vector<Pos>>(std::vector<int>{0, 1, 2}, [](const auto index){
        auto diff = (1 - index) * 2;
        return Pos{index + diff, index};
    });
};
auto subDiagonal = [](const auto& board) {
    return projectCoords(board, subDiagonalCoords(board));
};
typedef std::vector<char> Line;
typedef std::vector<Line> Lines;

auto allRows = [](auto board) {
    return transformAll<Lines>(std::vector<int>{0,1,2}, [board](auto index){
        return row(board, index);
    });
};
auto allColumns = [](auto board) {
    return transformAll<Lines>(std::vector<int>{0,1,2}, [board](auto index){
        return column(board, index);
    });
};
auto allDiagonals = [](auto board) -> Lines {
    return {mainDiagonal(board), subDiagonal(board)};
};
auto all = [](const auto board) {
    return concatenate3(allRows(board), allColumns(board), allDiagonals(board));
};

auto lineFilledWith = [](const auto& collection, const auto tokenToCheck) {
    return all_of_collection(collection, [tokenToCheck](const auto& token){
        return token == tokenToCheck;
    });
};
auto lineFilledWithX = std::bind(lineFilledWith, std::placeholders::_1, 'X');
auto lineFilledWithO = std::bind(lineFilledWith, std::placeholders::_1, 'O');

auto xWins = [](const auto& board) {
    return any_of_collection(all(board), lineFilledWithX);
};
auto oWins = [](const auto& board) {
    return any_of_collection(all(board), lineFilledWithO);
};

auto playerWins = [](const auto& board, const auto& player) {
    return any_of_collection(all(board), [player](auto line){
        return lineFilledWith(line, player);
    });
};

auto full = [](const auto& board) {
    return all_of_collection(allRows(board), [](const auto& row){
        return std::none_of(row.begin(), row.end(), [](char c){
            return c == '-';
        });
    });
};

void test_x_wins()
{
    char board[3][3] = {
#if (0)
        {'O', '-', 'O' },
        {'X', 'O', '-' },
        {'X', '-', 'O' }
#else
        {'X', 'O', 'X' },
        {'X', 'X', 'O' },
        {'O', 'O', 'O' }
#endif
    };

    auto linesDescription = std::map<std::string, Line>{
        {"1st row", row(board, 0)},
        {"2nd row", row(board, 1)},
        {"3rd row", row(board, 2)},
        {"1st column", column(board, 0)},
        {"2nd column", column(board, 1)},
        {"3rd column", column(board, 2)},
        {"main diagonal", mainDiagonal(board)},
        {"sub diagonal", subDiagonal(board)}
    };
    if (playerWins(board, 'X')) {
        auto result = findInCollection(linesDescription, [](auto value){
            return lineFilledWithX(value.second);
        });
        if (result.has_value()) {
            std::cout << "x win : " << result->first << "\n";
        }
    } else if (playerWins(board, 'O')) {
        auto result = findInCollection(linesDescription, [](auto value){
            return lineFilledWithO(value.second);
        });
        if (result.has_value()) {
            std::cout << "o win : " << result->first << "\n";
        }
    } else if (full(board)) {
        std::cout << "draw game !\n";
    }
}

void test_lambda()
{
    auto printAll = [](const auto& container){
        std::copy(container.begin(), container.end(), std::ostream_iterator<char>(std::cout, " "));
        std::cout << "\n";
    };
    char board[3][3] = { {'X', '-', '-'}, {'X', 'X', '-'}, {'-', '-', 'X'}};

    std::cout << "row\n";
    printAll(row(board, 0));
    printAll(row(board, 1));
    printAll(row(board, 2));

    std::cout << "\ncolumn\n";
    printAll(column(board, 0));
    printAll(column(board, 1));
    printAll(column(board, 2));

    std::cout << "\nmain diagonal\n";
    printAll(mainDiagonal(board));

    std::cout << "\nsub diagonal\n";
    printAll(subDiagonal(board));
}

auto lineToString = [](const auto& line) {
    return transformAll<std::string>(line, [](const auto c) -> char {
        return c;
    });
};

auto boardToLineString = [](const auto board) {
    return transformAll<std::vector<std::string>>(board, lineToString);
};

auto boardToString = [](const auto board) {
    return accumulateAll(boardToLineString(board), [](const std::string& l, const std::string& r){
        return l + r + "\n";
    });
};

void test_line_to_string() {
    auto line = Line{'X', '-', 'O'};
    std::cout << lineToString(line) << "\n";
};

void test_board_to_string() {
    Lines board = {
        {'X', '-', 'O' },
        {'X', 'X', 'O' },
        {'O', 'X', 'O' }
    };
    std::cout << "boardToString(board):\n";
    std::cout << boardToString(board) << "\n";
}

auto True = []{
    return true;
};

enum Result {
    XWins,
    OWins,
    GameNotOverYet,
    Draw
};

auto gameNotOverYet = [](auto board) {
    return !full(board);
};

typedef Lines Board;

using Rule = std::pair<std::function<bool()>, Result>;

auto condition = [](auto rule) {
    return rule.first();
};

auto result = [](auto rule) {
    return rule.second;
};

auto findTheRule = [](const auto& rules) {
    return *std::find_if(rules.begin(), rules.end(), [](auto rule){
        return condition(rule);
    });
};

auto resultForFirstRuleThaApplies = [](auto rules) {
    return result(findTheRule(rules));
};

Result winner(Board board) {
    auto gameNotOverYetOnBoard = std::bind(gameNotOverYet, board);
    auto xWinsOnBoard = std::bind(xWins, board);
    auto oWinsOnBoard = std::bind(oWins, board);

    std::vector<Rule> rules = {
        {xWinsOnBoard, XWins},
        {oWinsOnBoard, OWins},
        {gameNotOverYetOnBoard, GameNotOverYet},
        {True, Draw}
    };
#if (0)
    auto result = std::find_if(rules.begin(), rules.end(), [](auto pair){
        return pair.first();
    });

    return result->second;
#else
    return resultForFirstRuleThaApplies(rules);
#endif
};

void test_winner() {
    auto board = Board {
        { 'X', 'O', 'O' },
        { 'O', 'X', 'X' },
        { 'O', 'X', 'O' }
    };

    auto result = winner(board);
    auto s = std::string{};
    switch (result) {
    case XWins: s = "x wins"; break;
    case OWins: s = "o wins"; break;
    case GameNotOverYet: s = "game not over"; break;
    case Draw: s = "draw game"; break;
    }

    std::cout << "result: " << s << "\n";
}

void test() {
    //test_x_wins();
    //test_line_to_string();
    //test_board_to_string();
    test_winner();
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test() {
    auto v = std::vector<std::string>{"Hello", "Qt" , "6.0"};
    auto concatenate = [](const std::string& l, const std::string& r) {
        return l + r + " ";
    };

#if (0)
    auto concatenated = std::accumulate(v.begin(), v.end(), std::string{}, concatenate);
#else

    auto concatenated = accumulateAll(v, concatenate);
#endif

    std::cout << "concatenated: " << concatenated << "\n";
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_06()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif
    _4::test();

    //_5::test();
}
