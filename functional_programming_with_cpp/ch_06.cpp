#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>

namespace { //=================================================================

auto all_of_collection = [](const auto& collection, auto pred) {
    return std::all_of(collection.begin(), collection.end(), pred);
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
    return transformAll<Lines>(std::vector<int>{0,1,2}, row);
};
auto allColumns = [](auto board) {
    return transformAll<Lines>(std::vector<int>{0,1,2}, column);
};
auto allDiagonals = [](auto board) -> Lines {
    return {mainDiagonal(board), subDiagonal(board)};
};
auto all = [](auto board) {
    concatenate3(allRows(board), allColumns(board), allDiagonals);
};

void test()
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

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_06()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
